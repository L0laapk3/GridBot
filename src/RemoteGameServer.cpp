#include "RemoteGameServer.h"

RemoteGameServer::RemoteGameServer() {
	conn.receive(board);
}

void RemoteGameServer::move(std::vector<unsigned long>& move) {
	conn.sendMove(move);
	conn.receive(board);
}

RemoteGameServer::~RemoteGameServer() {
	conn.~Conn();
}