/**
 * @defgroup   CONSTANT_VERIFICATION_UNIT constant verification unit
 *
 * @brief      This file the implements class of the constant verification unit
 * 			   for a load value predictor.
 * 			   The constant verification unit has the following functions:                     
 * 			   - Keep track of load addresses which have been deemed "constant"
 * 			   by the LCT
 * 			   - Monitor all committed stores and invalidate a constant load 
 * 			   address if a store that is being committed has the same address
 * 			   as the constant load
 * 			   - Compare a load value for address that had been deemed 
 * 			   "predictable" by the LCT with the actual load value retrieved 
 * 			   from memory
 * 			   - Provide feedback to the LCT for all predictable loads
 * 			   - Possible implementation of this class could also include a 
 * 			   mechanism to pass load values from the commit stage back to the
 * 			   LVPT (Since this will be needed if an instruction is to be 
 * 			   promoted from unpredictable to predictable and so on.)
 *
 * @author     Satvik
 * @date       2021
 */

#ifndef __CPU_LVP_CONSTANT_VERIFICATION_UNIT__
#define __CPU_LVP_CONSTANT_VERIFICATION_UNIT__

#include "sim/sim_object.hh"
#include "base/types.hh"
#include "params/ConstantVerificationUnit.hh"

#include <vector>
#include <map>
#include <iterator>
#include <algorithm>

/**
 * @brief      The CVU stores the load address and the index of the LVPT in a 
 *             concatenated form in its CAM. This is searched by loads that are
 *             predicted as constant and by store addresses. Thus, the search of
 *             the CAM will be different for each case. The search for a load 
 *             predicted as "constant" will be done on the entire entry in the 
 *             CAM while a store address will only need to be compared with the 
 *             load address stored in the CAM.
 */

class ConstantVerificationUnit : public SimObject {
public: 
	ConstantVerificationUnit(ConstantVerificationUnitParams *p);

	~ConstantVerificationUnit();

	/**
	 * @brief      A store address will be provided to the CVU which will then 
	 * 			   search the CAM and invalidate every entry which matches the 
	 * 			   input address. This will also generate the control signals
	 * 			   required for communicating with the LCT.
	 *
	 * @param[in]  address  The store address
	 */	
	void processStoreAddress(ThreadID tid, Addr address);

	/**
	 * @brief      Check if a load address classified as constant is present in
	 *  		   the CVU CAM.
	 *
	 * @param[in]  address    The load address
	 * @param[in]  lvptIndex  The lvpt index corresponding to that load address
	 *
	 * @return     True if the load address, LVPT index pair exist in the CAM
	 * 			   False otherwise
	 */	
	bool processLoadAddress(Addr address, Addr lvptIndex, ThreadID tid);

	void updateLCT();

	bool comparePredictedLoad();

	bool updateConstLoad(Addr address, Addr lvptIndex);

private:
	/**
	 * The CVU Content Addressable Memory; 
	 * If a store address is found in this memory, the corresponding entry is
	 * invalidated and the invalidation also triggers an update routine which
	 * tells the LCT that this load address is no longer constant.
	 * The vector stores an ordered pair (load address, LVPT index)
	 */
	std::vector<std::pair<ThreadID, std::pair<Addr, Addr>>> _cvuCAM;

	/**
	 * Number of entries in the CVU CAM
	 */
	uint32_t _numEntries;

	/**
	 * Number of loads marked "predictable" which had a correct predicted value
	 */
	uint64_t _numPredictHits;

	/**
	 * Number of loads marked "predictable" that had an incorrect predicted
	 * value.
	 */
	uint64_t _numPredictMiss;

	/**
	 * Number of loads marked "constant" which were incorrectly predicted.
	 */
	uint64_t _numConstantHits;

	/**
	 * Number of loads marked "constant" that were correctly predicted.
	 */
	uint64_t _numConstantMiss;

	/**
	 * Number of store addresses which hit in the CVU CAM
	 */
	uint64_t _numStoreHits;

	/**
	 * Number of store addresses which missed in the CVU CAM.
	 */
	uint64_t _numStoreMiss;
};


#endif
