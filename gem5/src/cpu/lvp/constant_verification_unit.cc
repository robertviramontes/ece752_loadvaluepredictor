#include "cpu/lvp/constant_verification_unit.hh"

ConstantVerificationUnit::ConstantVerificationUnit(ConstantVerificationUnitParams *params) :
    SimObject(params) 
{}

ConstantVerificationUnit::~ConstantVerificationUnit() {}

void ConstantVerificationUnit::processStoreAddress(ThreadID tid, 
													 Addr address) {
	// Only the load address needs to be compared with the store address
	uint64_t currentStoreHits = _numStoreHits;
	auto itr = _cvuCAM.begin();
	while (itr != _cvuCAM.end()) {
		std::pair<ThreadID, std::pair<Addr, Addr>> temp = *itr;
		if (temp.first == tid && temp.second.first == address) {
			itr = _cvuCAM.erase(itr);
			++_numStoreHits;
			if (_cvuCAM.empty()) break;
		}
	}
	if (currentStoreHits != _numStoreHits) {
		++_numStoreMiss;
	}
}

bool ConstantVerificationUnit::processLoadAddress(Addr address, 
													Addr lvptIndex,
													ThreadID tid) {
	// Both load address and LVPT index have to be searched
	auto itr = _cvuCAM.begin();
	while (itr != _cvuCAM.end()) {
		std::pair<ThreadID, std::pair<Addr, Addr>> temp = *itr;
		if (temp.first == tid && temp.second.first == address 
			&& temp.second.second == lvptIndex) {
			++_numConstantHits;
			return true;
		}
	}
	++_numConstantMiss;
	return false;
}

ConstantVerificationUnit*
ConstantVerificationUnitParams::create()
{
    return new ConstantVerificationUnit(this);
}
