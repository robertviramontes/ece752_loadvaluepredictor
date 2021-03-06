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

#include "cpu/lvp/load_value_prediction_table.hh"

#include "base/intmath.hh"
#include "base/logging.hh"
#include "base/trace.hh"
#include "debug/LVPT.hh"

LoadValuePredictionTable::LoadValuePredictionTable(const LoadValuePredictionTableParams *params)
    : SimObject(params),
      numEntries(params->entries),
      historyDepth(params->historyDepth),
      indexMask(numEntries - 1),
      instShiftAmt(0) // TODO what is correct???
{
    if (!isPowerOf2(numEntries)) {
        fatal("Invalid LVPT number of entries!\n");
    }

    DPRINTF(LVPT, "LVPT index mask: %#x\n", indexMask);

    DPRINTF(LVPT, "LVPT entries: %i\n",
            numEntries);

    DPRINTF(LVPT, "LVPT history depth: %i\n", historyDepth);

    DPRINTF(LVPT, "instruction shift amount: %i\n",
            instShiftAmt);
}

uint64_t
LoadValuePredictionTable::lookup(ThreadID tid, Addr inst_addr)
{
    unsigned local_predictor_idx = getLocalIndex(inst_addr);

    DPRINTF(LVPT, "Looking up index %#x\n",
            local_predictor_idx);

    // TODO lookup value in table
    return 0;
}

void
LoadValuePredictionTable::update(ThreadID tid, Addr inst_addr, bool prediction_correct,
                bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
    unsigned local_predictor_idx;

    // No state to restore, and we do not update on the wrong
    // path.
    if (squashed) {
        return;
    }

    // Update the local predictor.
    local_predictor_idx = getLocalIndex(inst_addr);

    DPRINTF(LVPT, "Looking up index %#x\n", local_predictor_idx);
}

inline
unsigned
LoadValuePredictionTable::getLocalIndex(Addr &inst_addr)
{
    return (inst_addr >> instShiftAmt) & indexMask;
}

LoadValuePredictionTable*
LoadValuePredictionTableParams::create()
{
    return new LoadValuePredictionTable(this);
}
