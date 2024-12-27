
## Comparatif

Divers comparatif sur les versions de gcc, les options -O0 -O2 -O3, les types de tableaux en AoS, et un SoA.
Temps d'exécution pour 10000x100000, en secondes.
Quand on rejoue plusieurs fois, variations de +-20% sur les chiffres, donc j'ai fait des arrondis violents, pour que les effets sautent au yeux.

AoS, double    |  9.1.0 -O0 |  9.3.0 -O0 | 10.2.0 -O0 ||  9.1.0 -O2 |  9.3.0 -O2 | 10.2.0 -O2 |
:------------- | ---------: | ---------: | ---------: || ---------: | ---------: | ---------: |
array          |       6.0  |        6.0 |        6.0 ||        1.0 |        1.0 |        1.0 |
valarray       |       6.0  |        6.0 |        6.0 ||        1.0 |        1.0 |        1.0 |
vector         |      17.0  |       17.0 |       17.0 ||        1.0 |        1.0 |        1.0 |

AoS, valarray  |  -O2 float | -O2 double |   -O2 long ||  -O3 float | -O3 double |   -O3 long |
:------------- | ---------: | ---------: | ---------: || ---------: | ---------: | ---------: |
gcc  9.3.0     |       1.0  |        1.0 |        4.0 ||        3.0 |        6.0 |        2.0 |
gcc 10.2.0     |       1.0  |        1.0 |        4.0 ||        3.0 |        6.0 |        2.0 |
clang 10.0.1   |       1.0  |        1.0 |        4.0 ||        1.0 |        1.0 |        4.0 |
clang 11.0.0   |       1.0  |        1.0 |        4.0 ||        1.0 |        1.0 |        4.0 |

SoA, valarray  |  -O2 float | -O2 double |   -O2 long ||  -O3 float | -O3 double |   -O3 long |
:------------- | ---------: | ---------: | ---------: || ---------: | ---------: | ---------: |
gcc  9.3.0     |       0.7  |        0.8 |        4.5 ||        0.3 |        0.6 |        4.5 |
gcc 10.2.0     |       0.7  |        0.8 |        4.5 ||        0.3 |        0.6 |        4.5 |
clang 10.0.1   |       0.3  |        0.6 |        4.5 ||        0.3 |        0.6 |        4.5 |
clang 11.0.0   |       0.3  |        0.6 |        4.5 ||        0.3 |        0.6 |        4.5 |

Tentatives d'explications
* en `-O0`, tous les types de tableaux ne sont pas équivalent : on paie le surcout des appels de fonctions pour accéder aux éléments des tableaux, ce qui disparait en `-O2` ou ces appels sont éliminés.
* `float` n'est pas plus rapide que `double` (hors effets de cache), car quoiqu'il arrive les données sont placées dans des registres matériels de taille `double` pour le calcul.
* en AoS, l'option `-O3` bénéficie à `long double` et nuit à `float` et `double`, sans doute à cause d'une tentative contre-productive de vectorisation !
* Clang ne souffre pas de ces travers de gcc sur l'AoS, et on dirait qu'il vectorise dès -O2.
* en SoA, les temps en `float` sont imbattables, et les temps en double ne s'améliorent pas ?!? VERIFIER AVEC GODBOLT
* `std::valarray` est assimilable à un gsl::dyn_array, plus des opérateurs termes à termes.

A propos de `-march=native` : permet de compiler un binaire qui exploite au mieux le hardware sous-jacent, notamment (ou seulement ?) sa capacité de vectorisation, au détriment de la portabilité du binaire. Par ailleurs, quand on passe par docker, les paramètres pris en compte pourraient être ceux de la machine sur laquelle l'image a été fabriquée. Vérifier en lançant `lscpu` dans le conteneur.
https://stackoverflow.com/questions/54039176/mtune-and-march-when-compiling-in-a-docker-image

En alternative aux flags -m, GCC (et seulement GCC, pas Clang ni ICC à ma connaissance) propose une extension rigolote, le "function multiversioning". Cela permet de compiler plusieurs versions d'une fonction avec différents flags d'optimisation, puis basculer entre l'une ou l'autre selon une détection du matériel au runtime, via un mécanisme analogue à une vtable (il faut donc utiliser cette fonctionnalité "à gros grain", pas sur l'équivalent d'une opération AVX unique).
Voilà la doc: https://gcc.gnu.org/onlinedocs/gcc/Function-Multiversioning.html


## Essai en 1024*1,000,000

Le choix de 1024 (ou 512) vise à tenir dans le cache L1, et à mieux voir les effets de la vectorisation. Ci-dessous, on a bien un facteur de l'ordre de 8 sur les float, et de l'ordre de 4 sur les double. Pour des tailles de tableau plus grandes, on soupconne que le programme devient IO-bound, c'est à dire limité par le début du cache L2 pour fournir les données.

SoA, valarray  |  -O2 float | -O2 double |   -O2 long ||  -O3 float | -O3 double |   -O3 long |
:------------- | ---------: | ---------: | ---------: || ---------: | ---------: | ---------: |
gcc 10.2.0     |       0.9  |        0.9 |        3.6 ||       0.12 |       0.24 |        2.3 |



## Reconnaitre soi-même les instructions vectorielles dans l'assembleur

A VOIR AVEC GODBOLT & "CUTTER"

Addition et multiplication
* 128 bits, SSE : `ADDPS`, `MULPS` XMM?
* 128 bits, SSE2 : `ADDPD`, `MULPD` XMM?
* 256 bits, AVX : avec registres YMM?
* 512 bits, AVX-512 : avec préfixe EVEX?

Mais attention, avec des niveaux plus élevé d'instructions,
peut apparaitre l'opportunité de faire du FMA, et là les
instructions seront plutôt de la forme `*MADD*`.


## Un essai d'analyse de la vectorisation avec [Maqao](http://www.maqao.org/)

Premiers problèmes rencontrés :
* pas de binaire MacOS déjà fair pour maqao
* pas d'accès aux sources
=> j'installe le binaire intel64 dans le conteneur,
en croisant les doigts.

La commande magique : `/opt/maqao.intel64.2.12.0/maqao.intel64 cqa /work/tmp.saxpy-soa.exe fct-loops=saxpy`

Mais :
* en -O3, la fonction est en ligne, et maqao ne la trouve pas :'(
* de toute façon le `for` n'est pas dans ma fonction, mais dans les opérateurs * et + de std::valarray...
