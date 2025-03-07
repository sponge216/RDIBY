// server.h

#ifndef APP_NETWORK_SERVER_H
#define APP_NETWORK_SERVER_H

#include "network.h"
#include "concurrency.h"
#include <iostream>

namespace server {

	constexpr auto DEFAULT_BACKLOG = 20;
	constexpr auto DEFAULT_PORT = "8080";
	constexpr auto ROOM_SIZE = 20;

	typedef std::pair<SOCKET, sockaddr*> SocketAddrPair;
	constexpr SocketAddrPair SAD_NULL = SocketAddrPair(NULL, NULL);	// CREATE NULL CASE

	inline bool compareSocketAddrPair(SocketAddrPair a, SocketAddrPair b) {
		return a.first == b.first && a.second == b.second;
	}

	class ServerSocket : public network::BaseSocket {
	public:
		ServerSocket();
		~ServerSocket();

		bool init(PCSTR port = DEFAULT_PORT, INT backlog = DEFAULT_BACKLOG, INT ai_family = AF_INET, INT ai_flags = AI_PASSIVE, INT ai_protocol = IPPROTO_TCP, INT ai_socktype = SOCK_STREAM);
		inline DWORD sendData(SOCKET sock, CHAR* pData, DWORD dwTypeSize, DWORD flags) override;
		inline DWORD recvData(SOCKET sock, CHAR* pBuffer, DWORD dwBufferLen, DWORD flags) override;
		SocketAddrPair acceptNewConnection(SOCKET serverSocket, sockaddr* addr = NULL, int* addrLen = NULL);


	private:
		std::vector<SocketAddrPair> sadVector;
	};


	// --------------------------------------- //

	class ServerNetworkManager {
	public:
		ServerNetworkManager();
		~ServerNetworkManager();

		ServerSocket serverSocket;
		concurrency::ThreadManager threadManager;

		static DWORD WINAPI  acceptThreadFunc(LPVOID params);
		static DWORD WINAPI clientHandlerThreadFunc(LPVOID params);
		static DWORD WINAPI clientThreadEntrypoint(LPVOID params);
		bool acceptFunc(ServerNetworkManager* pSnm);


		//TODO: FIX ACCESS LEVELS!@!!!
	private:
		bool killSNM = false;
		/*LPTHREAD_START_ROUTINE(*pAcceptThreadFunc)(LPVOID params);
		LPTHREAD_START_ROUTINE(*pClientHandlerThreadFunc)(LPVOID params);*/




	};

	// --------------------------------------- //
	typedef struct roomClient_t {
		SOCKET sock = -1;
		char* name = NULL;
	}RoomClient, * pRoomClient, RoomHost, * pRoomHost;

	typedef struct room_t {
		std::vector<pRoomClient>* pRoomVector;
		pRoomHost host = NULL;
		DWORD roomID = 0;
		DWORD roomPassword = 0;
		DWORD dwCurrRoomSize = 0;

		room_t() :pRoomVector(new std::vector<pRoomClient>), host(NULL), roomID(0), roomPassword(0), dwCurrRoomSize(0) {

		}
	} Room, * pRoom;

	class RoomManager {
	public:
		RoomManager();
		~RoomManager();

		bool createNewRoom(DWORD roomID, pRoom room);
		bool deleteRoom(DWORD roomID);

		bool addClientToRoom(DWORD roomID, pRoomClient pClient);
		bool removeClientFromRoom(DWORD roomID, SOCKET clientSock);

	private:
		std::unordered_map<DWORD, pRoom>* pRoomMap; // ID -> room map
	};

}
#endif //APP_NETWORK_SERVER_H