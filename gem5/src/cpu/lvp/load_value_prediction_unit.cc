/*
 * Robert Viramontes
 * Created March 26, 2021
 * Based on the learning/part2/simpleobject 
 * 
 * 
 */

#include "cpu/lvp/load_value_prediction_unit.hh"

#include "base/logging.hh"
#include "base/trace.hh"
#include "debug/LVP.hh"

LoadValuePredictionUnit::LoadValuePredictionUnit(LoadValuePredictionUnitParams *params) :
    SimObject(params),
    loadClassificationTable(params->load_classification_table),
    loadValuePredictionTable(params->load_value_prediction_table),
    constantVerificationUnit(params->constant_verification_unit)
{
    DPRINTF(LVP, "Created the LVP\n");
    panic_if(!loadClassificationTable, "LVP must have a non-null LCT");
    panic_if(!loadValuePredictionTable, "LVP must have a non-null LVPT");
    panic_if(!constantVerificationUnit, "LVP must have a non-null LVPT");
}

LvptResult
LoadValuePredictionUnit::lookup(ThreadID tid, Addr inst_addr)
{
    auto lctResult = loadClassificationTable->lookup(tid, inst_addr);
    auto lvptResult = loadValuePredictionTable->lookup(tid, inst_addr);

    LvptResult result;
    result.taken = lctResult;
    result.value = lvptResult;
    return result;
}

bool
LoadValuePredictionUnit::processStoreAddress(ThreadID tid, Addr store_address)
{
    constantVerificationUnit->processStoreAddress(tid, store_address);
    return true;
}

bool 
LoadValuePredictionUnit::verifyPrediction(ThreadID tid, Addr pc, Addr load_address,
                RegVal correct_val, RegVal predicted_val) {
    if (correct_val == predicted_val)
    {
        loadClassificationTable->update(tid, pc, true, false, nullptr, pc);
        return true;
    } else{
        return false;
    }
}

void
LoadValuePredictionUnit::startup()
{
    // Before simulation starts, we need to schedule the event
    DPRINTF(LVP, "Starting the LVP");
}

LoadValuePredictionUnit*
LoadValuePredictionUnitParams::create()
{
    return new LoadValuePredictionUnit(this);
}
