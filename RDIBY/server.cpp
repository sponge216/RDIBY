#include "server.h"

namespace server {

	inline server::ServerSocket::ServerSocket() {
		this->init();
	}
	bool server::ServerSocket::init(PCSTR port, INT backlog, INT ai_family, INT ai_flags, INT ai_protocol, INT ai_socktype) {
		if (this->pAddrInfo != NULL) {
			this->hints = { 0 };
			this->pAddrInfo = NULL;
		}
		SOCKET serverSocket;
		struct addrinfo* pAddrInfo = NULL; // the address info struct, holds all info about the address.
		struct addrinfo hints = { 0 }; // used to set the socket's behavior and address
		ZeroMemory(&hints, sizeof(hints));

		hints.ai_family = ai_family; // set internet family.
		hints.ai_socktype = ai_socktype; //set socket type. We use TCP so we set it to sock_stream.
		hints.ai_protocol = ai_protocol;
		hints.ai_flags = ai_flags;

		// Resolve the local address and port to be used by the server
		int res = getaddrinfo(NULL, port, &hints, &pAddrInfo);
		if (res != 0) {
			printf("getaddrinfo failed: %d\n", res);
			WSACleanup();
			return false;
		}

		if ((serverSocket = socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol)) <= 1) {
			printf("Error at socket: %ld\n", WSAGetLastError());
			freeaddrinfo(pAddrInfo);
			WSACleanup();
			return false;
		}

		if (serverSocket == INVALID_SOCKET) {
			printf("Error at socket: %ld\n", WSAGetLastError());
			freeaddrinfo(pAddrInfo);
			WSACleanup();
			return false;
		}

		if (bind(serverSocket, pAddrInfo->ai_addr, (INT)pAddrInfo->ai_addrlen) == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(pAddrInfo);
			closesocket(serverSocket);
			WSACleanup();
			return false;
		}

		printf("Server Socket initiated!\n");
		if (listen(serverSocket, backlog) < 0) {
			fprintf(stdout, "Listen failed on Server Socket with error: %ld\n", WSAGetLastError());
			return false;
		}

		this->hints = hints;
		this->pAddrInfo = pAddrInfo;
		return true;
	}

	inline server::ServerSocket::~ServerSocket() {
		fprintf(stdout, "Closing Server Socket. FD - %llu", this->sock);
		freeaddrinfo(this->pAddrInfo);
		closesocket(this->sock);
	}

	inline DWORD server::ServerSocket::sendData(SOCKET sock, CHAR* pData, DWORD dwTypeSize, DWORD flags) {
		DWORD res = send(sock, pData, dwTypeSize, flags);
		return res;
	}

	inline DWORD server::ServerSocket::recvData(SOCKET sock, CHAR* pBuffer, DWORD dwBufferLen, DWORD flags) {
		DWORD res = recv(sock, pBuffer, dwBufferLen, flags);
		return res;
	}

	server::SocketAddrPair server::ServerSocket::acceptNewConnection(SOCKET serverSocket, sockaddr* addr, int* addrLen) {
		SOCKET clientSocket = accept(serverSocket, addr, addrLen);
		if (clientSocket <= 1) {
			printf("Error at establishing new client connection: %ld\n", WSAGetLastError());
			WSACleanup();
			return server::SAD_NULL;
		}
		SocketAddrPair sadRes = SocketAddrPair(clientSocket, addr);
		return sadRes;
	}
	// --------------------------------------- //

	server::ServerNetworkManager::ServerNetworkManager() {

	}

	server::ServerNetworkManager::~ServerNetworkManager() {
	}

	DWORD WINAPI server::ServerNetworkManager::clientThreadEntrypoint(LPVOID params) {
		// Convert LPVOID back to the instance pointer
		ServerNetworkManager* instance = static_cast<ServerNetworkManager*>(params);
		//TODO: FIX PARAM PASSING
		return instance->clientHandlerThreadFunc(NULL);
	}

	DWORD WINAPI server::ServerNetworkManager::acceptThreadFunc(LPVOID params) {
		while (!this->killSNM) {
			SocketAddrPair sadRes = this->serverSocket.acceptNewConnection(this->serverSocket.sock);
			if (server::compareSocketAddrPair(sadRes, server::SAD_NULL))
				continue;

			HANDLE hStopEvent = CreateEventA(NULL, true, false, NULL);
			if (hStopEvent == NULL) {
				std::cout << "failed to create stop event. last error: " << GetLastError() << "\n";
			}
			LPVOID lpParameter = NULL;

			concurrency::pConThread pctClient = new concurrency::ConThread(hStopEvent, clientThreadEntrypoint, lpParameter);
			this->threadManager.createNewThread(this->serverSocket.sock, pctClient);
		}
		return 1;
	}

	DWORD WINAPI server::ServerNetworkManager::clientHandlerThreadFunc(LPVOID params) {
		while (!this->killSNM) {

		}
		return 1;
	}
}