#include <iostream>

#include "LocalGameServer.h"
#include "RemoteGameServer.h"

int main() {
	auto server = LocalGameServer();
	for (int i = 0; i < 25; i++) {
		std::cout << server.data[i] << " ";
		if (i % 5 == 4)
			std::cout << std::endl;
	}
}