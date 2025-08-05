
On voit le SIMD sur axpy avec std::array :
- O2 aos double 1024 2000000 : 0.590 s
- O2 soa double 1024 2000000 : 0.590 s
- O3 aos double 1024 2000000 : 0.620 s
- O3 soa double 1024 2000000 : 0.240 s

Mais pas sur les complexes :
- O2 aos float 1024 100000 : 0.200 s
- O2 soa float 1024 100000 : 0.200 s
- O3 aos float 1024 100000 : 0.190 s
- O3 soa float 1024 100000 : 0.190 s
- Of aos float 1024 100000 : 0.210 s
- Of soa float 1024 100000 : 0.190 s

## Difficulté avec la conception

1) Pas trouvé de moyen simple de créer une table 1d
   d'une certaine taille et d'un certain value_type.
   Tous les exemples semblent avoir besoin d'une source.

2) Le fait que l'egalite entre deux vues swap ce sur
   quoi elles pointent me semble problematique. Et je
   n'ai pas trouvé de methode complementaire permettant
   une semantique de valeur... OK, il y a un algo de copie.

## Programme et resultats kiwaku-sycl

kiwaku-sycl cpu aos double 1024 10000 : real 0.800 s, user  0.330 s, sys 0.560 s, checksum: 0.8115612022238222
kiwaku-sycl cpu soa double 1024 10000 : real 2.590 s, user 65.810 s, sys 9.580 s, checksum: 0.8115612022238222
kiwaku-sycl gpu aos double 1024 10000 : real 0.530 s, user  0.090 s, sys 0.420 s, checksum: 0.8115612022238222
kiwaku-sycl gpu soa double 1024 10000 : real 1.090 s, user  0.580 s, sys 0.490 s, checksum: 0.8115612022238222

