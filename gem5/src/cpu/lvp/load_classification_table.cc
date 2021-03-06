/*
 * Copyright (c) 2004-2006 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "cpu/lvp/load_classification_table.hh"

#include "base/intmath.hh"
#include "base/logging.hh"
#include "base/trace.hh"
#include "debug/LCT.hh"

LoadClassificationTable::LoadClassificationTable(const LoadClassificationTableParams *params)
    : SimObject(params),
      localPredictorSize(params->localPredictorSize),
      localCtrBits(params->localCtrBits),
      localPredictorSets(localPredictorSize / localCtrBits),
      localCtrs(localPredictorSets, SatCounter(localCtrBits, 0)),
      localCtrThreads(localPredictorSets, 0),
      indexMask(localPredictorSets - 1),
      invalidateConstToZero(params->invalidateConstToZero),
      instShiftAmt(0) // TODO what is correct???
{
    if (!isPowerOf2(localPredictorSize)) {
        fatal("Invalid LCT local predictor size!\n");
    }

    if (!isPowerOf2(localPredictorSets)) {
        fatal("Invalid number of LCT local predictor sets! Check localCtrBits.\n");
    }

    DPRINTF(LCT, "LCT index mask: %#x\n", indexMask);

    DPRINTF(LCT, "LCT size: %i\n",
            localPredictorSize);

    DPRINTF(LCT, "LCT counter bits: %i\n", localCtrBits);

    DPRINTF(LCT, "instruction shift amount: %i\n",
            instShiftAmt);
}

LVPType
LoadClassificationTable::lookup(ThreadID tid, Addr inst_addr)
{
    unsigned local_predictor_idx = getLocalIndex(inst_addr);

    if (tid != localCtrThreads[local_predictor_idx])
    {
        return LVP_STRONG_UNPREDICTABLE;
    }

    DPRINTF(LCT, "Thread ID: %d", tid);

    uint8_t counter_val = localCtrs[local_predictor_idx];

    DPRINTF(LCT, "During lookup index %#x had value %i\n",
            local_predictor_idx, (int)counter_val);

    return getPrediction(counter_val);
}

LVPType
LoadClassificationTable::update(ThreadID tid, Addr inst_addr, LVPType prediction, bool prediction_correct)
{
    unsigned local_predictor_idx;

    // No state to restore, and we do not update on the wrong
    // path.
    // if (squashed) {
    //     return;
    // }

    // Update the local predictor.
    local_predictor_idx = getLocalIndex(inst_addr);

    if (tid == localCtrThreads[local_predictor_idx])
    {
        if (prediction_correct) {
            DPRINTF(LCT, "Load classification updated as correct.\n");
            localCtrs[local_predictor_idx]++;
        } else {
            DPRINTF(LCT, "Load classification updated as incorrect.\n");
            if (prediction == LVP_CONSTANT && invalidateConstToZero) {
                resetCtr(local_predictor_idx);
            } else {
                localCtrs[local_predictor_idx]--;
            }
        }
    }
    else
    {
        // Destructive interference with a different thread, reset this index and update the thread
        localCtrThreads[local_predictor_idx] = tid;
        resetCtr(local_predictor_idx);
    }

    uint8_t counter_val = localCtrs[local_predictor_idx];
    return getPrediction(counter_val);
}

void
LoadClassificationTable::reset()
{
    auto zeroCounter = new SatCounter(localCtrBits, 0);
    for (int c = 0; c < localPredictorSets; c++)
    {
        localCtrs[c] = *zeroCounter;
    }

    delete zeroCounter;
}

void
LoadClassificationTable::resetCtr(unsigned local_predictor_idx)
{
    while (localCtrs[local_predictor_idx] != LVP_STRONG_UNPREDICTABLE) {
        localCtrs[local_predictor_idx]--;
    }
}

inline
LVPType
LoadClassificationTable::getPrediction(uint8_t &count)
{
    // If MSB is 0, value is unpredictable
    // If counter is saturated, value is constant
    // Otherwise, the value is predictable
    return (count == 0) ? LVP_STRONG_UNPREDICTABLE 
            : (count >> (localCtrBits - 1)) == 0 ? LVP_WEAK_UNPREDICTABLE 
            : count == (power(2, localCtrBits) -1) ? LVP_CONSTANT 
            : LVP_PREDICTABLE;
}

inline
unsigned
LoadClassificationTable::getLocalIndex(Addr &inst_addr)
{
    return (inst_addr >> instShiftAmt) & indexMask;
}

LoadClassificationTable*
LoadClassificationTableParams::create()
{
    return new LoadClassificationTable(this);
}
