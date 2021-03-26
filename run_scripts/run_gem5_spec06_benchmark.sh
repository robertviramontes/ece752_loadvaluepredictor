#!/bin/bash

###################################################################################################
## Author 	: Prajyot Gupta
## Contact      : pgupta54@wisc.edu
## Coursework   : ECE 752
## Department   : Grad Student @ Dept. of Electrical & Computer Engineering
##
################################# DIRECTORY VARIABLES: MODIFY ACCORDINGLY ########################
##
## prajyotg :: to add exacxt paths per each user
GEM5_DIR=
SPEC_DIR=                                    # Install location of your SPEC2006 benchmarks
##
###################################################################################################

ARGC=$# # Get number of arguments excluding arg0 (the script itself). Check for help message condition.
if [[ "$ARGC" != 3 ]]; then # Bad number of arguments.
        echo "run_gem5_alpha_spec06_benchmark.sh  Copyright (C) 2014 Mark Gottscho"
   echo "This program comes with ABSOLUTELY NO WARRANTY; for details see <http://www.gnu.org/licenses/>."
   echo "Referred from https://markgottscho.wordpress.com/2014/09/20/tutorial-easily-running-spec-cpu2006-benchmarks-in-the-gem5-simulator/"
   echo ""
        echo "Author: Prajyot Gupta"
        echo "pgupta54@wisc.edu"
        echo ""
        echo "This script runs a single gem5 simulation of a single SPEC CPU2006 benchmark."
        echo ""
        echo "USAGE: run_gem5_spec06_benchmark.sh <BENCHMARK> <OUTPUT_DIR> <CHECKPOINT DIRECTORY>"
        echo "EXAMPLE: ./run_gem5_alpha_spec06_benchmark.sh bzip2 /FULL/PATH/TO/output_dir /FULL/PATH/TO/CHECKPOINT"
        echo ""
        echo "A single --help help or -h argument will bring this message back."
        exit
fi

# Get command line input. We will need to check these.
BENCHMARK=$1                                    # Benchmark name, e.g. bzip2
OUTPUT_DIR=$2                                   # Directory to place run output. Make sure this exists!
CHECKPOINT_DIR=$3				# Gets details of the checkpoint for SPEC Benchmarks

######################### BENCHMARK CODENAMES ####################
##prajyotg ::  Discuss with Vanshika/Adarsh and reduce this subset

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
##
## Check BENCHMARK input
##
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
if [[ "$BENCHMARK" == "sphinx3" ]]; then
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

########## Below is the main Gem5 Command 

$GEM5_DIR/build/X86/gem5.opt --outdir=$OUTPUT_DIR $GEM5_DIR/configs/example/se.py --restore-with-cpu=AtomicSimpleCPU -c../spec2006_apps/${BENCHMARK}_base.i386-m32-gcc42-nn  --checkpoint-dir=../checkpoints/checkpoint_$CHECKPOINT_DIR --caches --l2cache --cpu-type=DerivO3CPU  -I 10000000  --mem-size=16GB --checkpoint-restore=1

######### The End! 
