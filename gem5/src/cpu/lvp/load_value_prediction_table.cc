/****************************************************************************/
// Author 	: Prajyot Gupta
// Department   : Grad Student @ Dept. of Electrical & Computer Engineering
// Contact      : pgupta54@wisc.edu
// Project      : ECE 752
/****************************************************************************/

#include "cpu/lvp/load_value_prediction_table.hh"

#include "base/intmath.hh"
#include "base/trace.hh"
#include "debug/LVPT.hh"

LoadValuePredictionTable::LoadValuePredictionTable(const LoadValuePredictionTableParams *params)
    : SimObject(params),
      numEntries(params->entries),
      historyDepth(params->historyDepth),
      idxMask(numEntries - 1),
      instShiftAmt(0) 

{
    DPRINTF(LVPT, "LVPT: Creating LVPT object.\n");

    if (!isPowerOf2(numEntries)) {
        fatal("LVPT entries is not a power of 2!");
    }

    LVPT.resize(numEntries);

    DPRINTF(LVPT, "LVPT: Doing an initial reset \n");
    for (unsigned i = 0; i < numEntries; ++i) {
        LVPT[i].valid = false;
    }

    //prajyotg :: TODO :: To discuss
    idxMask = numEntries - 1;
    tagMask = (1 << tagBits) - 1;
    tagShiftAmt = instShiftAmt + floorLog2(numEntries);
}

/* Reset API */
void
LoadValuePredictionTable::reset()
{
    DPRINTF(LVPT, "LVPT : Calling the Reset API \n");
    for (unsigned i = 0; i < numEntries; ++i) {
        LVPT[i].valid = false;
    }
}

/* APIs to get index and tag*/
inline
unsigned
LoadValuePredictionTable::getIndex(Addr instPC, ThreadID tid)
{
    // Need to shift PC over by the word offset.
    // Math: ((instPC >> instShiftAmt)^(tid<<(tagShiftAmt-instShiftAmt-log2NumThreads)))&idxMask;
    DPRINTF(LVPT, "LVPT : Getting Index \n");
    return ((instPC >> instShiftAmt)
            ^ (tid << (tagShiftAmt - instShiftAmt - log2NumThreads)))
            & idxMask;
}

inline
Addr
LoadValuePredictionTable::getTag(Addr instPC)
{
    DPRINTF(LVPT, "LVPT : Getting Tag \n");
    return (instPC >> tagShiftAmt) & tagMask;
}

/** Checks if the load entry is in the LVPT.i **/
bool
LoadValuePredictionTable::valid(Addr instPC, ThreadID tid)
{
    DPRINTF(LVPT, "LVPT : Checking if LVPT entry is valid \n");
    unsigned LVPT_idx = getIndex(instPC, tid);

    Addr inst_tag = getTag(instPC);

    // Making sure index doesn't go out of bounds
    assert(LVPT_idx < numEntries);

    // Check if: (a) LVPT entry is valid
    // (b) index matches
    // (c) tag matches
    if (LVPT[LVPT_idx].valid
        && inst_tag == LVPT[LVPT_idx].tag
        && LVPT[LVPT_idx].tid == tid) {
        return true;
    } else {
        return false;
    }
}

// data = 0 represent invalid entry.
// prajyotg :: TODO :: better representation of incorrect data
// prajyotg ::TheISA::PCState
unsigned 
LoadValuePredictionTable::lookup(Addr instPC, ThreadID tid)
{
    DPRINTF(LVPT, "LVPT : Looking up the entry in PC :: prajyotg  \n");
    unsigned LVPT_idx = getIndex(instPC, tid);

    Addr inst_tag = getTag(instPC);

    assert(LVPT_idx < numEntries);

    if (LVPT[LVPT_idx].valid
        && inst_tag == LVPT[LVPT_idx].tag
        && LVPT[LVPT_idx].tid == tid) {
        return LVPT[LVPT_idx].target;
    } else {
        return 0;
    }
}

void
//prajyotg :: updated :: LoadValuePredictionTable::update(Addr instPC, const TheISA::PCState &target, ThreadID tid)
LoadValuePredictionTable::update(Addr instPC, const unsigned target, ThreadID tid)
{
    DPRINTF(LVPT, "LVPT : Updating the value in the LVPT \n");
    unsigned LVPT_idx = getIndex(instPC, tid);

    assert(LVPT_idx < numEntries);

    LVPT[LVPT_idx].tid = tid;
    LVPT[LVPT_idx].valid = true;
    LVPT[LVPT_idx].target = target;
    LVPT[LVPT_idx].tag = getTag(instPC);
}

LoadValuePredictionTable*
LoadValuePredictionTableParams::create()
{
    return new LoadValuePredictionTable(this);
}
