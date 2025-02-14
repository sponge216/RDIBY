// network.h

#ifndef APP_NETWORK_H
#define APP_NETWORK_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <memory>
	
#pragma comment(lib, "Ws2_32.lib")

namespace network {

	class BaseSocket {

	public:
		virtual bool send(CHAR* pData,SIZE_T dwTypeSize) = 0;
		virtual bool recv() = 0;

	protected:
		SOCKET sock;
		struct addrinfo* pAddrInfo = NULL; // the address info struct, holds all info about the address.
		struct addrinfo hints = { 0 }; // used to set the socket's behavior and address

	};
}
#endif //APP_NETWORK_H