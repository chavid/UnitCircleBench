

#include <kwk/container.hpp>
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/algorithm/algos/copy.hpp>
#include <kwk/context/sycl/context.hpp>

#include <iostream>
#include <cassert> // for assert
#include <vector>
#include <algorithm>
#include <fstream>  // file
#include <cstdlib>  // rand
#include <cmath>    // cos, sin, sqrt, atan

struct chrono_t
{
  void Init()
  {
    elapsed_ = 0;
    ResumeTiming();
  }

  chrono_t()
  {
    Init();
  }


  void PauseTiming()
  {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> e = now - last_point_;
    elapsed_ += e.count();
  }

  void ResumeTiming()
  {
    last_point_ = std::chrono::system_clock::now();
  }

  std::size_t ElapsedTimeMS()
  {
    PauseTiming();
    ResumeTiming();
    return static_cast<std::size_t>(elapsed_ * 1000);
  }

  std::string Str()
  {
    std::string res = std::to_string(elapsed_) + "s";
    return res;
  }

private:
  std::chrono::time_point<std::chrono::system_clock> last_point_;
  double elapsed_;
};


// export KWK_RABERU_PATH=/home/data_evo/data_sync/academique/These/dependencies/raberu/include
// export KWK_KUMI_PATH=/home/data_evo/data_sync/academique/These/dependencies/kumi/include
// export KWK_PATH=/home/data_evo/data_sync/academique/These/kiwaku_2025-06/include

// icpx -fsycl david_2025-08-04.cpp -o e -O3 -Wall -Wextra -DNDEBUG -std=c++20 -I${KWK_RABERU_PATH} -I${KWK_KUMI_PATH} -I${KWK_PATH} && ./e

template <typename DATA_TYPE>
void no_proxy_version(std::ofstream& outfile, std::ofstream& fbench, std::size_t size, std::size_t degree)
{
  srand(1) ;

  std::vector<DATA_TYPE> v_re1(size), v_im1(size), v_re2(size), v_im2(size);
  for ( std::size_t i = 0 ; i < size ; ++i )
  {
    auto e1  = std::rand() ;
    v_re1[i] = std::cos(e1) ;
    v_im1[i] = std::sin(e1) ;

    auto e2  = std::rand() ;
    v_re2[i] = std::cos(e2) ;
    v_im2[i] = std::sin(e2) ;
  }

  std::vector<DATA_TYPE> v_tmp(size) ;

  auto k_tmp = kwk::view{ kwk::source = v_tmp.data(), kwk::of_size(size) } ; // temporary value
  auto k_re1 = kwk::view{ kwk::source = v_re1.data(), kwk::of_size(size) } ; // in/output real
  auto k_im1 = kwk::view{ kwk::source = v_im1.data(), kwk::of_size(size) } ; // in/output imaginary
  auto k_re2 = kwk::view{ kwk::source = v_re2.data(), kwk::of_size(size) } ; // in/output real
  auto k_im2 = kwk::view{ kwk::source = v_im2.data(), kwk::of_size(size) } ; // in/output imaginary

  // (A + iB)(C + iD) = (A*C - B*D + iA*D + iB*C)

  kwk::sycl::context context{::sycl::gpu_selector_v} ;

  chrono_t chrono;
  for (std::size_t d = 0; d + 1 < degree; ++d)
  {
    // Partie réelle
    kwk::transform( context
                  , [](auto re1, auto im1, auto re2, auto im2)
                    {
                      return re1 * re2 - im1 * im2 ;
                    }
                  , k_tmp
                  , k_re1
                  , k_im1
                  , k_re2
                  , k_im2
                  );

    // Partie imaginaire
    kwk::transform( context
                  , [](auto re1, auto im1, auto re2, auto im2)
                    {
                      return re1 * im2 + im1 * re2;
                    }
                  , k_im1
                  , k_re1
                  , k_im1
                  , k_re2
                  , k_im2
                  );

    kwk::copy(context, k_re1, k_tmp) ;

    // k_re1 et k_im1 ont donc une nouvelle valeur,
    // k_re2 et k_im2 restent inchangés.
    // std::cout<<'.'<<std::flush ;
  }
  std::size_t elapsed_ms = chrono.ElapsedTimeMS();
  std::cout << "NAIF - Temps écoulé (ms): " << elapsed_ms << std::endl;
  outfile << "NAIF - Temps écoulé (ms): " << elapsed_ms << std::endl;
  fbench << elapsed_ms << " ";

  // Pour vérification
  for (std::size_t i = 0; i < 10; ++i)
  {
    outfile << " (" << v_re1[i] << " " << v_im1[i] << ") ";
  }
  outfile << "\n";
}



template <typename DATA_TYPE>
void proxy_version(std::ofstream& outfile, std::ofstream& fbench, std::size_t size, std::size_t degree)
{
  srand(1) ;

  std::vector<DATA_TYPE> v_re1(size), v_im1(size), v_re2(size), v_im2(size);
  for ( std::size_t i = 0 ; i < size ; ++i )
  {
    auto e1  = std::rand() ;
    v_re1[i] = std::cos(e1) ;
    v_im1[i] = std::sin(e1) ;

    auto e2  = std::rand() ;
    v_re2[i] = std::cos(e2) ;
    v_im2[i] = std::sin(e2) ;
  }

  std::vector<DATA_TYPE> v_tmp(size) ;

  auto k_tmp = kwk::view{ kwk::source = v_tmp.data(), kwk::of_size(size) } ; // temporary value
  auto k_re1 = kwk::view{ kwk::source = v_re1.data(), kwk::of_size(size) } ; // in/output real
  auto k_im1 = kwk::view{ kwk::source = v_im1.data(), kwk::of_size(size) } ; // in/output imaginary
  auto k_re2 = kwk::view{ kwk::source = v_re2.data(), kwk::of_size(size) } ; // in/output real
  auto k_im2 = kwk::view{ kwk::source = v_im2.data(), kwk::of_size(size) } ; // in/output imaginary

  // (A + iB)(C + iD) = (A*C - B*D + iA*D + iB*C)
  // ((re1) + i(im1))((re2) + i(im2)) = ((re1)*(re2) - (im1)*(im2) + i(re1)*(im2) + i(im1)*(re2))

  kwk::sycl::context context{::sycl::gpu_selector_v} ;
  chrono_t chrono;

  // Nouveau scope pour la destruction automatique des proxies Kiwaku
  {
    auto p_tmp = context.inout(k_tmp);
    auto p_re1 = context.inout(k_re1);
    auto p_im1 = context.inout(k_im1);
    auto p_re2 = context.in(k_re2);
    auto p_im2 = context.in(k_im2);


    for (std::size_t d = 0; d + 1 < degree; ++d)

    
    {
      // Partie réelle
      kwk::transform_proxy( context
                          , [](auto re1, auto im1, auto re2, auto im2)
                            {
                              return re1 * re2 - im1 * im2 ;
                            }
                          , p_tmp
                          , p_re1
                          , p_im1
                          , p_re2
                          , p_im2
                          );

      // Partie imaginaire
      // Attention au inplace et à l'ordre des éléments !
      // SYCL ne permet pas d'avoir deux accesseurs in ET out sur le même buffer
      // J'ai donc créé une version "inplace" 
      // lorsqu'on a aussi besoin de la valeur de sortie en lecture
      // comme c'est le cas ici avec im1.
      kwk::transform_inplace_proxy( context
                                  , [](auto im1, auto re1, auto re2, auto im2)
                                    {
                                      return re1 * im2 + im1 * re2;
                                    }
                                  , p_im1
                                  , p_re1
                                  , p_re2
                                  , p_im2
                                  );

      kwk::copy_proxy(context, p_re1, p_tmp) ;

      // k_re1 et k_im1 ont donc une nouvelle valeur,
      // k_re2 et k_im2 restent inchangés.
      // std::cout<<'.'<<std::flush ;
    }
  }
  // Lors de la sortie du scope, les buffers proxies (donc les buffers SYCL)
  // sont automatiquement détruits.

  std::size_t elapsed_ms = chrono.ElapsedTimeMS();
  std::cout << "PROXY - Temps écoulé (ms): " << elapsed_ms << std::endl;
  outfile << "PROXY - Temps écoulé (ms): " << elapsed_ms << std::endl;
  fbench << elapsed_ms << " ";

  // Pour vérification
  for (std::size_t i = 0; i < 10; ++i)
  {
    outfile << " (" << v_re1[i] << " " << v_im1[i] << ") ";
  }
  outfile << "\n";
}


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[] )
{
  using DATA_TYPE = float;
  std::size_t size   = 200000;
  std::size_t degree = 1000;

  std::string fname = "output_check.txt";
  std::string fbench_name = "bench.txt";

  std::ofstream outfile(fname);
  std::ofstream fbench(fbench_name);

  // It's good practice to check if the file was opened successfully.
  if (!outfile.is_open()) {
      std::cerr << "Error: Could not open the file " << fname << " for writing." << std::endl;
      return 1;
  }

  if (!fbench.is_open()) {
      std::cerr << "Error: Could not open the file " << fbench_name << " for writing." << std::endl;
      return 1;
  }

  std::cout << "===== GPU VERSION =====\n\n";
  outfile   << "===== GPU VERSION =====\n\n";
  fbench    << "===== GPU VERSION =====\n\n";

  const std::size_t repeat = 3;

  fbench    << "=== NO PROXY ===\n";
  for (std::size_t i = 0; i < repeat; ++i)
  {
    no_proxy_version<DATA_TYPE>(outfile, fbench, size, degree);
  }
  fbench    << "\n";

  fbench    << "=== WITH PROXY ===\n";
  for (std::size_t i = 0; i < repeat; ++i)
  {
    proxy_version<DATA_TYPE>(outfile, fbench, size, degree);
  }

  return 0;
}