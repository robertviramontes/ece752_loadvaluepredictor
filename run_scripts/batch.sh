#!/usr/bin/bash

./run_gem5_x86_spec06_benchmark.sh lbm "300 test.file 0 0"
./run_gem5_x86_spec06_benchmark.sh bzip2 $PWD/BMDATA/bzipinput.source
./run_gem5_x86_spec06_benchmark.sh specrand "324342 24239"
./run_gem5_x86_spec06_benchmark.sh libquantum "1397 8"
./run_gem5_x86_spec06_benchmark.sh sjeng
