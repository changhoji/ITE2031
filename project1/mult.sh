cd assembler
make clean
make
./assembler mult.as mult.mc
cd ../simulator
make clean
make
./simulate ../assembler/mult.mc > mult.out

