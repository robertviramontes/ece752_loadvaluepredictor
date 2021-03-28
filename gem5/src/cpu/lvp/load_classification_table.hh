/*
 * Robert Viramontes
 * Created March 25, 2021
 * Based on the 2bit_local branch predictor provided with the gem5 source. 
 * 
 * 
 */

#ifndef __CPU_LVP_LOADCLASSIFICATIONTABLE_HH__
#define __CPU_LVP_LOADCLASSIFICATIONTABLE_HH__

#include <vector>

#include "base/sat_counter.hh"
#include "base/types.hh"
#include "sim/sim_object.hh"
#include "static_inst.hh"

#include "enums.hh"
#include "params/LoadClassificationTable.hh"

/**
 * Implements a local predictor that uses the PC to index into a table of
 * counters.  Note that any time a pointer to the bp_history is given, it
 * should be NULL using this predictor because it does not have any branch
 * predictor state that needs to be recorded or updated; the update can be
 * determined solely by the branch being taken or not taken.
 */
class LoadClassificationTable : public SimObject
{
  public:
    /**
     * Build load classfication table.
     */
    LoadClassificationTable(const LoadClassificationTableParams *params);

    /**
     * Looks up the given instruction address in the LCT and returns
     * a true/false value as to whether it is taken.
     * @param branch_addr The address of the branch to look up.
     * @param bp_history Pointer to any bp history state.
     * @return Whether or not the branch is taken.
     */
    LctResult lookup(ThreadID tid, Addr branch_addr, void * &bp_history);

    /**
     * Updates the branch predictor with the actual result of a branch.
     * @param branch_addr The address of the branch to update.
     * @param taken Whether or not the branch was taken.
     */
    void update(ThreadID tid, Addr branch_addr, bool prediction_correct,
                bool squashed, const StaticInstPtr & inst, Addr corrTarget);

    // void squash(ThreadID tid, void *bp_history)
    // { assert(bp_history == NULL); }

  private:
    /**
     *  Returns the unpredictable/predictable/constant prediction given 
     *  the value of the counter.
     *  @param count The value of the counter.
     *  @return The prediction based on the counter value.
     */
    inline LctResult getPrediction(uint8_t &count);

    /** Calculates the local index based on the PC. */
    inline unsigned getLocalIndex(Addr &PC);

    /** Size of the local predictor. */
    const unsigned localPredictorSize;

    /** Number of bits of the local predictor's counters. */
    const unsigned localCtrBits;

    /** Number of sets. */
    const unsigned localPredictorSets;

    /** Array of counters that make up the local predictor. */
    std::vector<SatCounter> localCtrs;

    /** Mask to get index bits. */
    const unsigned indexMask;

    /** Number of bits to shift instructions by for predictor addresses. */
    const unsigned instShiftAmt;
};

#endif // __CPU_LVP_LOADCLASSIFICATIONTABLE_HH__
