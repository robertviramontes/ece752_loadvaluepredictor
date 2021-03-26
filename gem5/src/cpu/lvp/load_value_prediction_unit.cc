/*
 * Robert Viramontes
 * Created March 26, 2021
 * Based on the 2bit_local branch predictor provided with the gem5 source. 
 * 
 * 
 */

#include "cpu/lvp/load_value_predictor.hh"

#include "debug/LVP.hh"

LoadValuePredictionUnit::LoadValuePredictionUnit(LoadValuePredictionUnitParams *params) :
    SimObject(params),
    loadClassificationTable(params->load_classification_table),
{
    DPRINTF(LVP, "Created the LVP\n");
    panic_if(!loadClassificationTable, "LVP must have a non-null LCT");
}

void
LoadValuePredictionUnit::startup()
{
    // Before simulation starts, we need to schedule the event
    schedule(event, latency);
}

LoadValuePredictionUnit*
LoadValuePredictionUnit::create()
{
    return new LoadValuePredictionUnit(this);
}
