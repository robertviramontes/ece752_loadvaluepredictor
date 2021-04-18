#!/usr/bin/bash
#scons build/X86/gem5.opt -j9 CPU_MODELS=AtomicSimpleCPU,TimingSimpleCPU,O3CPU,MinorCPU
scons build/X86/gem5.opt -j9 CPU_MODELS=O3CPU
