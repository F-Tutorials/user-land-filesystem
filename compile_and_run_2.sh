#!/bin/bash

cd ./fs/myfs || exit
rm build -rf
mkdir build && cd build && cmake .. && make
cd ../tests || exit
echo E | ./test.sh
cd ../../../