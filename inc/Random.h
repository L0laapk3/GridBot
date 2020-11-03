#include <cstdint>

class Random
{
private:
	uint64_t m_seed;

public:
	static const uint64_t a = 16807;
	static const uint64_t m = 1924421567;

	Random(uint64_t seed) {
		m_seed = seed;
	}

	uint64_t peek() {
		return a * m_seed % m;
	}

	uint64_t next() {
		m_seed = a * m_seed % m;
		if (m_seed == 0) { m_seed = 3229763266; }
		return m_seed;
	}
};