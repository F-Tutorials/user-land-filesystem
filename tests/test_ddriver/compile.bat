rm -rf build
mkdir build
gcc -o build/test.exe src/test.c -I ./include -lddriver -L../../driver/user_ddriver/lib