#include <iostream>

#include "LocalGameServer.h"
#include "RemoteGameServer.h"


int main() {
	auto server = LocalGameServer();
	server.board.print();
	std::cout << server.board.eval();
}