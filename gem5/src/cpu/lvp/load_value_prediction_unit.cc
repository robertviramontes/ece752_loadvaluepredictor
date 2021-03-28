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
    loadClassificationTable(params->load_classification_table)
{
    DPRINTF(LVP, "Created the LVP\n");
    panic_if(!loadClassificationTable, "LVP must have a non-null LCT");
}

LvptResult
LoadValuePredictionUnit::lookup(ThreadID tid, Addr inst_addr, void * &bp_history)
{
    auto lctResult = loadClassificationTable->lookup(tid, inst_addr, &bp_history);
    // TODO Lookup value in the LVPT
    // auto lvptResult = loadValuePredictionTable->lookup();

    LvptResult result;
    result.taken = lctResult;
    // result.value = lvptResult;
    return result;
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
