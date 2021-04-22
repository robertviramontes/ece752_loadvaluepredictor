#!/bin/bash

############ DIRECTORY VARIABLES: MODIFY ACCORDINGLY #############
##GEM5_DIR=/nobackup/amittal/DEFAULT
GEM5_DIR=/u/v/b/vbaoni/prajyotg/ece752_loadvaluepredictor/gem5
SPEC_DIR=/p/prometheus/private/adarsh1/spec2006_apps                                    # Install location of your SPEC2006 benchmarks
##################################################################

ARGC=$# # Get number of arguments excluding arg0 (the script itself). Check for help message condition.
##prajyotg :: if [[ "$ARGC" != 3 ]]; then # Bad number of arguments.
if [[ "$ARGC" != 2 ]]; then # Bad number of arguments.
        echo "run_gem5_spec06_benchmark.sh  Copyright (C) 2014 Mark Gottscho"
   echo ""
        echo "Author: Prajyot Gupta"
        echo "pgupta54@wisc.edu"
        echo ""
        echo "This script runs a single gem5 simulation of a single SPEC CPU2006 benchmark for Alpha ISA."
        echo ""
        echo "USAGE: run_gem5_alpha_spec06_benchmark.sh <BENCHMARK> <CHECKPOINT_DIR>"
        echo "EXAMPLE: ./run_gem5_x86_spec06_benchmark.sh astar astar_VC_DSR_O3CPU_squashed_load_analysis"
        echo "The name of the checkpoint directory is taken from /p/prometheus/private/adarsh1/checkpoints/checkpoint_* "
        echo "A single --help help or -h argument will bring this message back."
        exit
fi

# Get command line input. We will need to check these.
BENCHMARK=$1                                    # Benchmark name, e.g. bzip2
#prajyotg :: OUTPUT_DIR=$2                                   # Directory to place run output. Make sure this exists!
OUTPUT_DIR=/u/v/b/vbaoni/prajyotg/benchmarks_w_lvp/"$BENCHMARK"  # Directory to place run output. Make sure this exists!
CHECKPOINT_DIR=$2
######################### BENCHMARK CODENAMES ####################


PERLBENCH_CODE=400.perlbench
BZIP2_CODE=401.bzip2
GCC_CODE=403.gcc
BWAVES_CODE=bwaves
GAMESS_CODE=416.gamess
MCF_CODE=429.mcf
MILC_CODE=433.milc
ZEUSMP_CODE=434.zeusmp
GROMACS_CODE=gromacs
CACTUSADM_CODE=436.cactusADM
LESLIE3D_CODE=437.leslie3d
NAMD_CODE=namd
GOBMK_CODE=445.gobmk
DEALII_CODE=447.dealII
SOPLEX_CODE=450.soplex
POVRAY_CODE=povray
CALCULIX_CODE=454.calculix
HMMER_CODE=456.hmmer
SJENG_CODE=sjeng
GEMSFDTD_CODE=459.GemsFDTD
LIBQUANTUM_CODE=libquantum
H264REF_CODE=464.h264ref
TONTO_CODE=465.tonto
LBM_CODE=lbm
OMNETPP_CODE=471.omnetpp
ASTAR_CODE=astar
WRF_CODE=481.wrf
SPHINX3_CODE=482.sphinx3
XALANCBMK_CODE=483.xalancbmk
SPECRAND_INT_CODE=998.specrand
SPECRAND_FLOAT_CODE=999.specrand
##################################################################

# Check BENCHMARK input
#################### BENCHMARK CODE MAPPING ######################
BENCHMARK_CODE="none"

if [[ "$BENCHMARK" == "perlbench" ]]; then
        BENCHMARK_CODE=$PERLBENCH_CODE
fi
if [[ "$BENCHMARK" == "bzip2" ]]; then
        BENCHMARK_CODE=$BZIP2_CODE
fi
if [[ "$BENCHMARK" == "gcc" ]]; then
        BENCHMARK_CODE=$GCC_CODE
fi
if [[ "$BENCHMARK" == "bwaves" ]]; then
        BENCHMARK_CODE=bwaves_base
fi
if [[ "$BENCHMARK" == "gamess" ]]; then
        BENCHMARK_CODE=$GAMESS_CODE
fi
if [[ "$BENCHMARK" == "mcf" ]]; then
        BENCHMARK_CODE=$MCF_CODE
fi
if [[ "$BENCHMARK" == "milc" ]]; then
        BENCHMARK_CODE=$MILC_CODE
fi
if [[ "$BENCHMARK" == "zeusmp" ]]; then
        BENCHMARK_CODE=$ZEUSMP_CODE
fi
if [[ "$BENCHMARK" == "gromacs" ]]; then
        BENCHMARK_CODE=$GROMACS_CODE
fi
if [[ "$BENCHMARK" == "cactusADM" ]]; then
        BENCHMARK_CODE=$CACTUSADM_CODE
fi
if [[ "$BENCHMARK" == "leslie3d" ]]; then
        BENCHMARK_CODE=$LESLIE3D_CODE
fi
if [[ "$BENCHMARK" == "namd" ]]; then
        BENCHMARK_CODE=$NAMD_CODE
fi
if [[ "$BENCHMARK" == "gobmk" ]]; then
        BENCHMARK_CODE=$GOBMK_CODE
fi
if [[ "$BENCHMARK" == "dealII" ]]; then # DOES NOT WORK
        BENCHMARK_CODE=$DEALII_CODE
fi
if [[ "$BENCHMARK" == "soplex" ]]; then
        BENCHMARK_CODE=$SOPLEX_CODE
fi
if [[ "$BENCHMARK" == "povray" ]]; then
        BENCHMARK_CODE="povray_base"
fi
if [[ "$BENCHMARK" == "calculix" ]]; then
        BENCHMARK_CODE=$CALCULIX_CODE
fi
if [[ "$BENCHMARK" == "hmmer" ]]; then
        BENCHMARK_CODE=$HMMER_CODE
fi
if [[ "$BENCHMARK" == "sjeng" ]]; then
        BENCHMARK_CODE=$SJENG_CODE
fi
if [[ "$BENCHMARK" == "GemsFDTD" ]]; then
        BENCHMARK_CODE=$GEMSFDTD_CODE
fi
if [[ "$BENCHMARK" == "libquantum" ]]; then
        BENCHMARK_CODE="libquantum_base"
fi
if [[ "$BENCHMARK" == "h264ref" ]]; then
        BENCHMARK_CODE="h264ref_base"
fi
if [[ "$BENCHMARK" == "tonto" ]]; then
        BENCHMARK_CODE=$TONTO_CODE
fi
if [[ "$BENCHMARK" == "lbm" ]]; then
        BENCHMARK_CODE=$LBM_CODE
fi
if [[ "$BENCHMARK" == "omnetpp" ]]; then
        BENCHMARK_CODE=$OMNETPP_CODE
fi
if [[ "$BENCHMARK" == "astar" ]]; then
        BENCHMARK_CODE=$ASTAR_CODE
fi
if [[ "$BENCHMARK" == "wrf" ]]; then
        BENCHMARK_CODE=$WRF_CODE
fi
if [[ "$BENCHMARK" == "sphinx_livepretend" ]]; then
        BENCHMARK_CODE=$SPHINX3_CODE
fi
if [[ "$BENCHMARK" == "xalancbmk" ]]; then # DOES NOT WORK
        BENCHMARK_CODE=$XALANCBMK_CODE
fi
if [[ "$BENCHMARK" == "specrand_i" ]]; then
        BENCHMARK_CODE=$SPECRAND_INT_CODE
fi
if [[ "$BENCHMARK" == "specrand_f" ]]; then
        BENCHMARK_CODE=$SPECRAND_FLOAT_CODE
fi

# Sanity check
if [[ "$BENCHMARK_CODE" == "none" ]]; then
        echo "Input benchmark selection $BENCHMARK did not match any known SPEC CPU2006 benchmarks! Exiting."
        exit 1
fi
##################################################################

# Check OUTPUT_DIR existence
if [[ !(-d "$OUTPUT_DIR") ]]; then
        echo "Output directory $OUTPUT_DIR does not exist! Exiting."
        exit 1
fi

RUN_DIR=$SPEC_DIR/$BENCHMARK/run/run_base_ref\_i386-m32-gcc42-nn.0000           # Run directory for the selected SPEC benchmark
SCRIPT_OUT=$OUTPUT_DIR/runscript.log                                                                                                                                    # File log for this script's stdout henceforth

################## REPORT SCRIPT CONFIGURATION ###################

echo "Command line:"                                                            | tee $SCRIPT_OUT
echo "$0 $*"                                                                            | tee -a $SCRIPT_OUT
echo "================= Hardcoded directories ==================" | tee -a $SCRIPT_OUT
echo "GEM5_DIR:                                     $GEM5_DIR" | tee -a $SCRIPT_OUT
echo "SPEC_DIR:                                     $SPEC_DIR" | tee -a $SCRIPT_OUT
echo "==================== Script inputs =======================" | tee -a $SCRIPT_OUT
echo "BENCHMARK:                                    $BENCHMARK" | tee -a $SCRIPT_OUT
echo "OUTPUT_DIR:                                   $OUTPUT_DIR" | tee -a $SCRIPT_OUT
echo "==========================================================" | tee -a $SCRIPT_OUT
##################################################################


#################### LAUNCH GEM5 SIMULATION ######################
echo ""
echo "Changing to SPEC benchmark runtime directory:     $RUN_DIR" | tee -a $SCRIPT_OUT
cd $RUN_DIR

echo "" | tee -a $SCRIPT_OUT
echo "" | tee -a $SCRIPT_OUT
echo "--------- Here goes nothing! Starting gem5! ------------" | tee -a $SCRIPT_OUT
echo "" | tee -a $SCRIPT_OUT
echo "" | tee -a $SCRIPT_OUT

# Actually launch gem5!
#//'''
#//BINARY="gem5.fast-original"
#//CHCKPT_INST_MAX=4000000000
#//
#//CHCKPT_CREATE=" --take-checkpoints="+str(CHCKPT_INST_MAX)+" --at-instruction "
#//
#//FULL_SCRIPT=GEM5_PATH+"/build/"+PROTOCOL+"/"+BINARY+" --remote-gdb-port=0 "+GEM5_PATH+"/configs/example/fs.py --cpu-type=atomic --kernel="+GEM5_PATH+"/dist/m5/system/binaries/"+KERNEL+" --mem-size="+MEM_SIZE+" --mem-type=SimpleMemory --cacheline_size=64 --l1i_assoc=8 --l1d_assoc=8 --num-cpus="+str(NUM_CORES)+" --l1i_size="+L1_SIZE+" --l1d_size="+L1_SIZE+" --l2_assoc=16 --l2_size=" + L2_SIZE + CHCKPT_CREATE + RUN_SCRIPT 
#//'''

$GEM5_DIR/build/X86/gem5.opt --outdir=$OUTPUT_DIR  $GEM5_DIR/configs/example/se.py --restore-with-cpu=AtomicSimpleCPU -c /p/prometheus/private/adarsh1/spec2006_apps/${BENCHMARK}_base.i386-m32-gcc42-nn  --checkpoint-dir=/p/prometheus/private/adarsh1/checkpoints/checkpoint_$CHECKPOINT_DIR --caches --l2cache --cpu-type=DerivO3CPU  -I 10000000  --mem-size=16GB --checkpoint-restore=1 --mem-type=DDR3_2133_8x8  --l1d_size=32kB  --l1i_size=32kB  --l2_size=256kB  --l3_size=8MB  --l1d_assoc=8 --l1i_assoc=8 --l2_assoc=8 --l3_assoc=16 --cpu-clock=3.6GHz


#$GEM5_DIR/build/X86/gem5.opt --outdir=$OUTPUT_DIR  $GEM5_DIR/configs/example/se.py --restore-simpoint-checkpoint -r 1 --checkpoint-dir /p/prometheus/private/adarsh1/simpoints//462.libquantum/x86-/ --restore-with-cpu=AtomicSimpleCPU -c /p/prometheus/private/adarsh1/spec2006_apps/${BENCHMARK}_base.i386-m32-gcc42-nn --caches --l2cache --cpu-type=DerivO3CPU  --mem-size=16GB --mem-type=DDR3_2133_8x8  --l1d_size=32kB  --l1i_size=32kB  --l2_size=256kB  --l3_size=8MB  --l1d_assoc=8 --l1i_assoc=8 --l2_assoc=8 --l3_assoc=16 --cpu-clock=3.6GHz

#$GEM5_DIR/build/X86/gem5.fast --outdir=$OUTPUT_DIR $GEM5_DIR/configs/example/se.py --take-checkpoints=4000000000 --at-instruction --caches --l2cache --cpu-type=AtomicSimpleCPU  -I 100 -c /p/prometheus/private/adarsh1/spec2006_apps/${BENCHMARK}_base.i386-m32-gcc42-nn --mem-size=16GB --mem-type=DDR3_2133_8x8  --l1d_size=32kB  --l1i_size=32kB  --l2_size=256kB  --l3_size=8MB  --l1d_assoc=8 --l1i_assoc=8 --l2_assoc=8 --l3_assoc=16 --cpu-clock=3.6GHz
#$GEM5_DIR/build/X86/gem5.opt --outdir=$OUTPUT_DIR  --debug-flags=LCB --debug-file=TRACE_small  $GEM5_DIR/configs/example/se.py --restore-with-cpu=AtomicSimpleCPU -c /p/prometheus/private/adarsh1/spec2006_apps/${BENCHMARK}_base.i386-m32-gcc42-nn  --checkpoint-dir=/p/prometheus/private/adarsh1/checkpoints/checkpoint_$CHECKPOINT_DIR --caches --l2cache --cpu-type=DerivO3CPU -I 10000000  --mem-size=16GB --checkpoint-restore=1

#$GEM5_DIR/build/X86/gem5.opt --outdir=$OUTPUT_DIR  --debug-flags=LCB --debug-file=TRACE_small  $GEM5_DIR/configs/example/se.py --restore-with-cpu=AtomicSimpleCPU -c  /p/prometheus/private/adarsh1/spec2006_apps/${BENCHMARK_CODE}.i386-m32-gcc42-nn   --checkpoint-dir=$CHECKPOINT_DIR/checkpoint_${BENCHMARK}_VC_DSR_O3CPU_squashed_load_analysis/ --caches --l2cache --cpu-type=DerivO3CPU -I 1000000  --mem-size=16GB --checkpoint-restore=1

#$GEM5_DIR/build/X86/gem5.opt --outdir=$OUTPUT_DIR --debug-flags=LSQUnit --debug-file=temp1  $GEM5_DIR/configs/example/se.py --restore-with-cpu=AtomicSimpleCPU -c  /p/prometheus/private/adarsh1/spec2007_apps/${BENCHMARK_CODE}.i386-m32-gcc42-nn  --checkpoint-dir=$CHECKPOINT_DIR/checkpoint_${BENCHMARK}_VC_DSR_O3CPU_squashed_load_analysis/ --caches --l2cache --cpu-type=DerivO3CPU -I 1000000  --mem-size=16GB --checkpoint-restore=1

#$GEM5_DIR/build/X86/gem5.opt --outdir=$OUTPUT_DIR --debug-file=trace_${BENCHMARK_CODE}  $GEM5_DIR/configs/example/se.py --restore-with-cpu=AtomicSimpleCPU -c  /p/prometheus/private/adarsh1/spec2006_apps/${BENCHMARK_CODE}.i386-m32-gcc42-nn  --checkpoint-dir=$CHECKPOINT_DIR/checkpoint_${BENCHMARK}_VC_DSR_O3CPU_squashed_load_analysis/ --caches --l2cache --cpu-type=DerivO3CPU -I 1000 --mem-size=16GB --checkpoint-restore=1
