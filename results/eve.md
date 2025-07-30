
## ls-cassidi, 28 juillet 2025

Nouveau tests avec eve.cpp et eve.bash. Version EVE de Sylvain.
- eve raw       double 1024 100000 : main 0.041 s
- eve wide      double 1024 100000 : main 0.019 s
- eve transform double 1024 100000 : main 0.290 s

Apres l'ajout du "no_aligning", ca s'arrange grandement pour la version transform :
- eve raw       double 1024 100000 : main 0.041 s
- eve wide      double 1024 100000 : main 0.019 s
- eve transform double 1024 100000 : main 0.067 s

L'ajout de [eve::algo::no_unrolling] ne change rien.

Idem [eve::algo::unroll<2>] :

Avec `-mavx2`, c'est l'usage des wide explicit qui y perd le plus :
raw double       1024 100000 : main 0.046 s
wide double      1024 100000 : main 0.030 s
transform double 1024 100000 : main 0.070 s

Apres avoir corrigé mon dernier bug et modifié la variante `wide` pour
qu'elle supporte toutes les tailles de tableau,  avec gcc 15.1 on en est là :

- eve.exe raw       double 1024 100000 : 0.042 s, checksum: -1.3091659385215229
- eve.exe wide      double 1024 100000 : 0.027 s, checksum: -1.30916593852261
- eve.exe transform double 1024 100000 : 0.068 s, checksum: -1.30916593852261

Avec clang 20 :

- eve.exe raw       double 1024 100000 : 0.088 s, checksum: -1.3091659385216723
- eve.exe wide      double 1024 100000 : 0.032 s, checksum: -1.3091659385210535
- eve.exe transform double 1024 100000 : 0.077 s, checksum: -1.3091659385210535


