#include <iostream>

#include "LocalGameServer.h"
#include "RemoteGameServer.h"


int main() {
	while (true) {
		LocalGameServer server;
		//server.board.print();
		unsigned long score = 0;
		while (true) {
			auto move = server.board.findBestMove(100);
			//for (auto c : move)
			//	std::cout << c << " ";
			if (move.size() == 0)
				break;
			score += move.size() * server.board.get(move[0]);
			//std::cout << std::endl << score << std::endl;
			server.move(move);
			//server.board.print();
		}
		std::cout << std::endl;
		server.board.print();
		std::cout << "FINAL SCORE: " << score << std::endl;
	}
}