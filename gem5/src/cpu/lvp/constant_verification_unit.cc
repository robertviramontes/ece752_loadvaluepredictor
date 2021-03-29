#include "cpu/lvp/constant_verification_unit.hh"

constant_verification_unit::constant_verification_unit() {}

constant_verification_unit::~constant_verification_unit() {}

void constant_verification_unit::processStoreAddress(uint64_t address) {
	// Only the load address needs to be compared with the store address
	auto itr = _cvuCAM.begin();
	while (itr != _cvuCAM.end()) {
		std::pair<uint64_t, uint64_t> temp = *itr;
		if (tmp.first == address) {
			itr = _cvuCAM.erase(itr);
			if (_cvuCAM.empty()) break;
		}
	}
}

bool constant_verification_unit::processLoadAddress(uint64_t address, uint64_t lvptIndex) {
	// Both load address and LVPT index have to be searched
	auto itr = _cvuCAM.begin();
	while (itr != _cvuCAM.end()) {
		std::pair<uint64_t, uint64_t> temp = *itr;
		if (tmp.first == address && tmp.second == lvptIndex) {
			return true;
		}
	}
	return false;
}

