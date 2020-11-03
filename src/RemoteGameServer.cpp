#include "RemoteGameServer.h"

RemoteGameServer::RemoteGameServer() {
	conn.receiveBoard(data);
}

void RemoteGameServer::move(std::vector<unsigned long>& move) {
	conn.sendMove(move);
	conn.receiveBoard(data);
}

RemoteGameServer::~RemoteGameServer() {
	conn.~Conn();
}