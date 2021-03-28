/*
 * Robert Viramontes
 * Created March 25, 2021
 * Based on the 2bit_local branch predictor provided with the gem5 source. 
 * 
 */

#ifndef __CPU_LVP_ENUMS_HH__
#define __CPU_LVP_ENUMS_HH__

enum LctResult {Unpredictable = 0, Predictable, Constant};

#endif // __CPU_LVP_ENUMS_HH__