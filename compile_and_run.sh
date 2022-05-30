#!/bin/bash

# ["cd", "./fs/fs"],
# ["make", "clean"],
# ["make"],
# ["cd", "./tests"],
# ["bash", "./test.sh"],
# ["E"]

cd ./fs/fs || exit
rm build -rf
mkdir build && cd build && cmake .. && make
cd ../tests || exit
./test.sh
