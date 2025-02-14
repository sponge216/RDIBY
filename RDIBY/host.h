// host.h
#ifndef APP_NETWORK_HOST_H
#define APP_NETWORK_HOST_H

#include "network.h"
namespace host {
	/// <summary>
		///  socket for hosts.
		/// </summary>
	class HostSocket :
		public network::BaseSocket
	{

	public:
		HostSocket();
		~HostSocket();
		bool initTCP();
		bool initUDP();
		bool send(CHAR* data, SIZE_T dwTypeSize) override;
		bool recv() override;
	};

	// --------------------------------------- //

	/// <summary>
		/// 
		/// </summary>
	class HostNetworkManager {

	public:
		HostNetworkManager();
		~HostNetworkManager();

	private:
		HostSocket hostSocket;
		static const WSADATA wsaData;

	};
}
#endif //APP_NETWORK_HOST_H