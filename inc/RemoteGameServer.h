#pragma once

#include "GameServer.h"
#include "Conn.h"

class RemoteGameServer : public GameServer {
public:
	RemoteGameServer();
	void move(std::vector<unsigned long>& move);
	~RemoteGameServer();

protected:
	Conn conn;
};