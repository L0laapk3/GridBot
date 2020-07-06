

#include "Board.h"

const void printRaw(const Data& data) {
	unsigned long i = 0;
	for (int j = 0; j < 5; j++) {
		for (int k = 0; k < 5; k++) {
			for (int m = 0; m < 5; m++) {
				std::cout << ((data >> i)& Data(0b1)).to_ulong();
				i++;
			}
			if (k < 4)
				std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


const Data expand(const unsigned long& move, const unsigned int& valueInt) {
	return Data(std::array<uint64_t, 2>{
		_pdep_u64(move, 0b0001000010000100001000010000100001000010000100001000010000100001) * valueInt,
		_pdep_u64(move >> 13, 0b0000000100001000010000100001000010000100001000010000100001000010) * valueInt + ((move >> 12) & 1) * (valueInt >> 4)
	});
}

const uint64_t shrink(const Data& data) {
	return _pext_u64((data & Data(~(uint64_t)0)).to_ullong(), 0b0001000010000100001000010000100001000010000100001000010000100001)
		| (_pext_u64((data >> 64).to_ullong(), 0b0000000100001000010000100001000010000100001000010000100001000010) << 13);
}
