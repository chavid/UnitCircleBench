
# Interprétation des résultats

Résultats attendus :
- pas de différence entre un std::transform() classique et celui
  avec une strategie seq. et à partir de 02, pas de différence
  avec les boucles brutes.

La politique `std::execution::par` s'avère payante quand on parallelise
une fonction complexe, ou bien qu'on utilise pas `-O3`.

Intéressant :
- la stratégie `par` est la plus payante en terme de
  temps réel, en bénéficiant à plein des 16 coeurs, à condition
  de mettre le std::transform sur la boucle exterieure, sinon
  elle est completement contre-productive, car elle amene la
  creation de millions de threads.
  en terme HPC, `power1` est superieur à `power2`, avec `par` et `-O`>0.
- seule l'implementation avec le std::transforme en boucle
  interieur (`power2.cpp`) permet d'observer ce qui est probablement
  une auto-vectorisation du code.
  en termes HTC, `power2`, sans `par` et en `-O3`.
- en divisant la collection globale en blocs de 4 `double`,
  on peut réunir le meilleur des deux mondes -> `power3`.

Déception :
- unseq et par_unseq n'ont pas de différence avec seq et par,
  donc la stratégie unseq n'est pas prise en charge.

Par contre, je n'arrive pas à trouver un scénario qui démontre l'intérêt
de `std::execution::unseq`. Avec ou sans `-O3`, cette politique ne fait
ni plus ni moins bien que la vectorisation automatique.


# 18 façons de calculer

Quoiqu'il arrive il faut une boucle sur les éléments du tableau,
et une boucle sur le nombre de fois ou il faut multiplier un
nombre par lui-même (pour l´elever à la puissance voulue)

3 variantes algorithmiques:
- programme `power1` : boucle exterieur sur le tableau, boucle interieure sur la puissance ;
- programme `power2` : boucle exterieure sur la puissance, boucle interieure sur le tableau.
- programme `power3` : `power1`... mais avec des blocs internes vectorisables.

6 options:
- `raw` : boucles for traditionnelles ;
- `alg`: boucle sur le tableau à l'aide d'un std::transform() C++11 ;
- `seq`, `par`, `unseq`, `unseq_par` : std::transform() avec police d'exécution.


# Méthodologie de micro-benchmark

Tout est compilé en g++ 11.2.0, avec les options `-std=c++20 -lpthread -ltbb -march=native -mtune=native`
et un niveau d'optimisation allant de 0 à 3.

Le temps affiché est le temps réel moyen sur 10 exécutions, mesuré par `time`.


# Résultats bruts sur ma machine, en 1024x1000000, -std=c++20

Intel® Core™ i7-10875H CPU @ 2.30GHz × 16
Intel® SSE4.1, Intel® SSE4.2, Intel® AVX2
                                                                                
power1 -O0                     power2 -O0                     power3 -O0               
- raw       :  7.28   7.28     - raw       :  7.71   7.71     - raw       : 15.37 15.37
- alg       :  3.18   3.18     - alg       : 24.39  24.39     - alg       : 12.81 12.81
- seq       :  3.13   3.13     - seq       : 27.60  27.59     - seq       : 12.63 12.63
- par       :  0.29   4.52     - par       : 47.73 739.25     - par       :  1.41 21.44
- unseq     :  3.11   3.11     - unseq     : 13.48  13.49     - unseq     : 12.26 12.26
- par_unseq :  0.30   4.58     - par_unseq : 47.44 733.98     - par_unseq :  1.41 21.43
                                                                                       
power1 -O1                     power2 -O1                     power3 -O1               
- raw       :  3.12   3.12     - raw       :  2.79   2.79     - raw       :  3.12  3.12
- alg       :  1.55   1.55     - alg       :  0.82   0.82     - alg       :  0.96  0.96
- seq       :  1.40   1.40     - seq       :  0.69   0.68     - seq       :  0.90  0.90
- par       :  0.10   1.58     - par       : 11.97 184.68     - par       :  0.10  1.51
- unseq     :  1.40   1.39     - unseq     :  0.86   0.86     - unseq     :  0.90  0.90
- par_unseq :  0.11   1.57     - par_unseq : 11.98 183.52     - par_unseq :  0.10  1.51
                                                                                       
power1 -O2                     power2 -O2                     power3 -O2               
- raw       :  1.44   1.44     - raw       :  0.77   0.77     - raw       :  0.90  0.90
- alg       :  1.40   1.40     - alg       :  0.69   0.69     - alg       :  0.95  0.95
- seq       :  1.44   1.44     - seq       :  0.69   0.69     - seq       :  0.91  0.91
- par       :  0.11   1.57     - par       : 11.74 180.49     - par       :  0.10  1.45
- unseq     :  1.42   1.42     - unseq     :  0.68   0.68     - unseq     :  0.92  0.92
- par_unseq :  0.11   1.57     - par_unseq : 11.71 180.81     - par_unseq :  0.10  1.47
                                                                                       
power1 -O3                     power2 -O3                     power3 -O3               
- raw       :  1.40   1.39     - raw       :  0.29   0.28     - raw       :  0.58  0.58
- alg       :  1.39   1.39     - alg       :  0.28   0.28     - alg       :  0.37  0.37
- seq       :  1.37   1.37     - seq       :  0.29   0.29     - seq       :  0.37  0.37
- par       :  0.10   1.57     - par       : 11.81 182.72     - par       :  0.03  0.40
- unseq     :  1.41   1.41     - unseq     :  0.26   0.26     - unseq     :  0.41  0.41
- par_unseq :  0.11   1.58     - par_unseq : 11.74 180.70     - par_unseq :  0.03  0.40


# Ressources

* [](https://coffeebeforearch.github.io/2020/04/23/vectorization.html)
* [Are C++17 Parallel Algorithms implemented already ?](https://stackoverflow.com/questions/51031060/are-c17-parallel-algorithms-implemented-already)
* [Bartek's coding blog](https://www.bfilipek.com/2018/11/pstl.html)
