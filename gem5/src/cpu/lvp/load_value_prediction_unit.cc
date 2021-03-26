/*
 * Robert Viramontes
 * Created March 26, 2021
 * Based on the 2bit_local branch predictor provided with the gem5 source. 
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
