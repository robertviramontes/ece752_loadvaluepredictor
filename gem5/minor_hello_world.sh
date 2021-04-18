#!/bin/bash

build/X86/gem5.opt --debug-flags=LVP,LVPT --debug-start=9999999 configs/example/se.py --cmd=tests/test-progs/hello/bin/x86/linux/hello --caches --cpu-type=MinorCPU 