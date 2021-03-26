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
#include "cpu/pred/bpred_unit.hh"
#include "params/LocalBP.hh"

/**
 * Implements a local predictor that uses the PC to index into a table of
 * counters.  Note that any time a pointer to the bp_history is given, it
 * should be NULL using this predictor because it does not have any branch
 * predictor state that needs to be recorded or updated; the update can be
 * determined solely by the branch being taken or not taken.
 */
class LoadClassificationTable : public BPredUnit
{
  public:
    /**
     * Default branch predictor constructor.
     */
    LocalBP(const LocalBPParams *params);

    virtual void uncondBranch(ThreadID tid, Addr pc, void * &bp_history);

    /**
     * Looks up the given address in the branch predictor and returns
     * a true/false value as to whether it is taken.
     * @param branch_addr The address of the branch to look up.
     * @param bp_history Pointer to any bp history state.
     * @return Whether or not the branch is taken.
     */
    bool lookup(ThreadID tid, Addr branch_addr, void * &bp_history);

    /**
     * Updates the branch predictor to Not Taken if a BTB entry is
     * invalid or not found.
     * @param branch_addr The address of the branch to look up.
     * @param bp_history Pointer to any bp history state.
     * @return Whether or not the branch is taken.
     */
    void btbUpdate(ThreadID tid, Addr branch_addr, void * &bp_history);

    /**
     * Updates the branch predictor with the actual result of a branch.
     * @param branch_addr The address of the branch to update.
     * @param taken Whether or not the branch was taken.
     */
    void update(ThreadID tid, Addr branch_addr, bool taken, void *bp_history,
                bool squashed, const StaticInstPtr & inst, Addr corrTarget);

    void squash(ThreadID tid, void *bp_history)
    { assert(bp_history == NULL); }

  private:
    /**
     *  Returns the taken/not taken prediction given the value of the
     *  counter.
     *  @param count The value of the counter.
     *  @return The prediction based on the counter value.
     */
    inline bool getPrediction(uint8_t &count);

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
};

#endif // __CPU_LVP_LOADCLASSIFICATIONTABLE_HH__
