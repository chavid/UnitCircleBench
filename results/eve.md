
## ls-cassidi, 28 juillet 2025

La version `transform` semble anormalement longue. Elle l'était encore beaucoup plus avant l'ajout de `[eve::algo::no_aligning]`. 

Avec gcc 15.1 on en est là :

- eve.exe raw       double 1024 100000 : 0.042 s, checksum: -1.3091659385215229
- eve.exe wide      double 1024 100000 : 0.027 s, checksum: -1.30916593852261
- eve.exe transform double 1024 100000 : 0.068 s, checksum: -1.30916593852261

Avec clang 20 :

- eve.exe raw       double 1024 100000 : 0.088 s, checksum: -1.3091659385216723
- eve.exe wide      double 1024 100000 : 0.032 s, checksum: -1.3091659385210535
- eve.exe transform double 1024 100000 : 0.077 s, checksum: -1.3091659385210535

Autres infos :
- Options de compil : `-g -march=native -mtune=native -funroll-loops -Wall -Wextra -Wfatal-errors -lpthread -ltbb`. 
- Valeur de `eve::current_api` : `X86 AVX512 (with FMA3 support)`.

Les choses déjà essayées sans succès pour faire baisser le temps de la variante `transform` :
- Ajouter `[eve::algo::no_unrolling]` ou `[eve::algo::unroll<2>]`.`
- Ajouter `-MNDEBUG`.
- Enlever `-funroll-loops`.
- Remplacer `-march=native -mtune=native` par `-mavx2`.
- Compiler avec clang.
