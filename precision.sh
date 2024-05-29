echo

opt=${1}
shift

rm -f tmp.precision.exe
g++ -O${opt} -Wconversion -march=native -std=c++23 tmp.precision.cpp -o tmp.precision.exe

./tmp.precision.exe $*

rm -f tmp.precision.py
echo "s = 0 ; m = 0" >> tmp.precision.py
for i in 0 1 2 3 4 5 6 7 8 9 ; do
    \time -f "s += %U ; m += %M" -a -o ./tmp.precision.py ./tmp.precision.exe $*  >> /dev/null
done
echo "print(\"{:.4} s. {} kB.\".format(s/10.,m/10.))" >> tmp.precision.py
python3 tmp.precision.py

echo
