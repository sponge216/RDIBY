// client.h

#ifndef APP_NETWORK_CLIENT_H
#define APP_NETWORK_CLIENT_H

#include "network.h"

namespace client {
	/// <summary>
	/// socket for clients.
	/// </summary>
	class ClientSocket :
		public network::BaseSocket,
		public network::ITCPSocket,
		public network::IUDPSocket {

	public:
		ClientSocket();
		~ClientSocket();
		bool initTCP() override;
		bool initUDP() override;
		bool send(CHAR* data) override;
		bool recv() override;
	};


	/// <summary>
	/// 
	/// </summary>
	class ClientNetworkManager {

	public:
		ClientNetworkManager();
		~ClientNetworkManager();

	private:
		ClientSocket clientSocket;
		static const WSADATA wsaData;

	};
}

#endif // APP_NETWORK_CLIENT_H