/*
 * Robert Viramontes
 * Created March 26, 2021
 * Based on the 2bit_local branch predictor provided with the gem5 source. 
 * 
 * 
 */

#include "cpu/lvp/load_value_predictor.hh"

#include "debug/HelloExample.hh"

LoadValuePredictionUnit::LoadValuePredictionUnit(LoadClassificationTableParams *params) :
    SimObject(params),
    loadClassificationTable(params->goodbye_object),
{
    DPRINTF(HelloExample, "Created the hello object\n");
    panic_if(!goodbye, "HelloObject must have a non-null GoodbyeObject");
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
