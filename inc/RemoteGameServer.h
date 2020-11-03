#pragma once

#include "IGameServer.h"
#include "Conn.h"

class RemoteGameServer : public IGameServer {
public:
	RemoteGameServer();
	void move(std::vector<unsigned long>& move);
	~RemoteGameServer();

protected:
	Conn conn;
};