#include "LocalGameServer.h"

#include <cassert>

LocalGameServer::LocalGameServer() {
	for (int i = 0; i < 25; i++)
		data[i] = m_random.next() % 3 + 1;
}

void LocalGameServer::move(std::vector<unsigned long>& move) {
	assert(move.size() >= 2);
	for (const auto& c : move)
		assert(c >= 0 && c < 25);
	const auto value = data[move[0]];
	for (int i = 1; i < move.size(); i++) {
		const auto step = move[i - 1] - move[i];
		assert(step == -5 || step == -1 || step == 1 || step == 5);
		assert(data[move[i]] == value);
		for (int j = 0; j < i; j++)
			assert(move[j] != move[i]);
	}

	for (int i = 0; i < move.size() - 1; i++)
		data[move[i]] = m_random.next() % 3 + 1;
	data[move[move.size() - 1]] = value * move.size();
}