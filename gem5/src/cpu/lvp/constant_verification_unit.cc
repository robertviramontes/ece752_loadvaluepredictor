#include "cpu/lvp/constant_verification_unit.hh"

Constant_verification_unit::Constant_verification_unit() {}

Constant_verification_unit::~Constant_verification_unit() {}

void Constant_verification_unit::processStoreAddress(Addr address, 
													 ThreadID tid) {
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

bool Constant_verification_unit::processLoadAddress(Addr address, 
													Addr lvptIndex,
													ThreadID tid) {
	// Both load address and LVPT index have to be searched
	auto itr = _cvuCAM.begin();
	while (itr != _cvuCAM.end()) {
		std::pair<>ThreadID, std::pair<Addr, Addr> temp = *itr;
		if (tmp.first == tid && tmp.second.first == address 
			&& tmp.second.second == lvptIndex) {
			++_numConstantHits;
			return true;
		}
	}
	++_numConstantMisses;
	return false;
}

