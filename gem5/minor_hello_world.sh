#!/bin/bash

build/X86/gem5.opt configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello --caches --cpu-type=MinorCPU --lct-entries=512  --lct-ctr-bits=2 --lvpt-entries=1024 --lvpt-hist-depth=1 --cvu-entries=8 --cvu-replacement=1 | tee run.log