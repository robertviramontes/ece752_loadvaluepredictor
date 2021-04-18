/*
 * Robert Viramontes
 * Created March 26, 2021
 * Based on the learning/part2/simpleobject 
 * 
 * 
 */

#ifndef __CPU_LVP_LOADVALUEPREDICTIONUNIT_HH__
#define __CPU_LVP_LOADVALUEPREDICTIONUNIT_HH__

#include <string>

#include "cpu/lvp/load_classification_table.hh"
#include "cpu/lvp/load_value_prediction_table.hh"
#include "cpu/lvp/constant_verification_unit.hh"
#include "params/LoadValuePredictionUnit.hh"
#include "sim/sim_object.hh"
#include "base/types.hh"
#include "base/statistics.hh"

#include "cpu/base_dyn_inst.hh"

struct LvptResult {
    LVPType taken;
    RegVal value;
};

class LoadValuePredictionUnit : public SimObject
{
  private:
    /// Pointer to the corresponding load value prediction units. Set via Python
    LoadClassificationTable* loadClassificationTable;
    LoadValuePredictionTable* loadValuePredictionTable;
    ConstantVerificationUnit* constantVerificationUnit;

    Stats::Scalar numPredictableLoads;
    Stats::Scalar numPredictableCorrect;
    Stats::Scalar numPredictableIncorrect;
    Stats::Scalar numConstLoads;
    Stats::Scalar numConstLoadsMispredicted;
    Stats::Scalar numConstLoadsCorrect;
    Stats::Scalar totalLoads;

  public:
    LoadValuePredictionUnit(LoadValuePredictionUnitParams *p);

    /**
     * Looks up the given instruction address and returns
     * a LvptResult with the LctResult and predicted value.
     * @param inst_addr The address of the instruction to look up.
     * @param bp_history Pointer to any bp history state.
     * @return Whether or not the branch is taken.
     */
    LvptResult lookup(ThreadID tid, Addr inst_addr);

    /**
     * Part of a SimObject's initilaization. Startup is called after all
     * SimObjects have been constructed. It is called after the user calls
     * simulate() for the first time.
     */
    void startup();

    void regStats() override;

    std::pair<LVPType, RegVal> predictLoad(ThreadID tid, Addr pc);

    Addr lookupLVPTIndex(ThreadID tid, Addr pc);

    bool processLoadAddress(ThreadID tid, Addr pc, Addr lvpt_index);

    /** 
     * @brief Used to process a store to the store_address and update the CVU
     * so it can verify constants in the pipeline
     * 
     * @param tid The thread id
     * @param store_address data address of the value to be stored
     */
    bool processStoreAddress(ThreadID tid, Addr store_address);

    bool verifyPrediction(ThreadID tid, Addr pc, Addr load_address,
                          RegVal correct_val, RegVal predicted_val,
                          LVPType classification);

};

#endif // __CPU_LVP_LOADVALUEPREDICTIONUNIT_HH__
