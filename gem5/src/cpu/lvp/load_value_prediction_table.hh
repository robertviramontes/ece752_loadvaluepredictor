/****************************************************************************/
// Author 	: Prajyot Gupta
// Department   : Grad Student @ Dept. of Electrical & Computer Engineering
// Contact      : pgupta54@wisc.edu
// Project      : ECE 752
/****************************************************************************/

#ifndef __CPU_LVP_LOADVALUEPREDICTIONTABLE_HH__
#define __CPU_LVP_LOADVALUEPREDICTIONTABLE_HH__

#include "base/types.hh"
#include "cpu/static_inst.hh"
#include "sim/sim_object.hh"

#include "enums.hh"
#include "params/LoadValuePredictionTable.hh"

#include "arch/types.hh"
#include "base/logging.hh"
#include "config/the_isa.hh"

/** Creating a default Load Value Prediction Table entry
 *  which will have below attributes
 *  tag   : Specifies the Opcode of the Load instruction
 *  taget : Specifies the Load value associated with the tag
 *  valid : Specifies if the value loaded is valid
 */

class LoadValuePredictionTable : public SimObject
{
  private:
    struct LVPTEntry
    {
        LVPTEntry()
            : tag(0), target(0), valid(false)
        {}

        /** The entry's tag. */
        Addr tag;

        /** The entry's target. */ 
        RegVal target;

        /** The entry's thread id. */
        ThreadID tid;

        /** Whether or not the entry is valid. */
        bool valid;
    };

  public:
    /** Creates a LVPT with the given number of entries, number of bits per
     *  tag, and instruction offset amount.
     *  @param numEntries Number of entries for the LVPT.
     *  @param tagBits Number of bits for each tag in the LVPT.
     *  @param instShiftAmt Offset amount for instructions to ignore alignment.
     */
    LoadValuePredictionTable(const LoadValuePredictionTableParams *params);
    
    void reset();

    /** Looks up an address in the LVPT. Must call valid() first on the address.
     *  @param inst_PC The address of the branch to look up.
     *  @param tid The thread id.
     *  @return Returns the predicated load value.
     */
    RegVal lookup(Addr instPC, ThreadID tid);

    /** Checks if the load entry is in the LVPT.
     *  @param inst_PC The address of the branch to look up.
     *  @param tid The thread id.
     *  @return Whether or not the branch exists in the LVPT.
     */
    bool valid(Addr instPC, ThreadID tid);

    /** Updates the LVPT with the latest predicted Load Value.
     *  @param inst_PC The address of the branch being updated.
     *  @param target_PC The predicted target data.
     *  @param tid The thread id.
     */
    
    void update(Addr instPC, const RegVal target, ThreadID tid);

    /** Returns the index into the LVPT, based on the branch's PC.
     *  @param inst_PC The branch to look up.
     *  @return Returns the index into the LVPT.
     */
    
    inline unsigned getIndex(Addr instPC, ThreadID tid);

    /** Returns the tag bits of a given address.
     *  @param inst_PC The branch's address.
     *  @return Returns the tag bits.
     */
    inline Addr getTag(Addr instPC);

  private:

    /** The actual LVPT declaration */
    std::vector<LVPTEntry> LVPT;

    /** The number of entries in the LVPT. */
    const unsigned numEntries;

    /** Depth of data history kept in the LVPT*/
    const unsigned historyDepth;

    //prajyotg :: No sure if below will be used
    /** The index mask. */
    unsigned idxMask;

    /** The number of tag bits per entry. */
    unsigned tagBits;

    /** The tag mask. */
    unsigned tagMask;

    /** Number of bits to shift PC when calculating index. */
    unsigned instShiftAmt;

    /** Number of bits to shift PC when calculating tag. */
    unsigned tagShiftAmt;

    /** Log2 NumThreads used for hashing threadid */
    unsigned log2NumThreads;
};

#endif // __CPU_LVP_LOADVALUEPREDICTIONTABLE_HH__
