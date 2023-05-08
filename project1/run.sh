cd assembler
make clean
make
./assembler $1.as $1.mc
cd ../simulator
make clean
make
./simulate ../assembler/$1.mc > $1.out

