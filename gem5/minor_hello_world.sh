#!/bin/bash

build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello --caches --cpu-type=MinorCPU --lct-entries=256  --lct-ctr-bits=2 --lvpt-entries=512 --lvpt-hist-depth=4 --cvu-entries=4 --cvu-replacement=2 | tee run.log