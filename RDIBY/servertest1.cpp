#include "server.h"

int test1(int testNum);
int test2(int testNum);
int test3(int testNum);

int main(int argc, char** argv) {
	//test1(1);
	//test2(2);
	test3(3);
}

// test to check if accept thread function works
int test1(int testNum) {
	std::cout << "IN TEST  " << testNum << "\n";

	server::ServerNetworkManager* pSnm = new server::ServerNetworkManager();
	HANDLE hStopEvent = CreateEventA(NULL, true, false, NULL);
	if (hStopEvent == NULL) {
		std::cout << "failed to create stop event. last error: " << GetLastError() << "\n";
	}
	LPVOID lpParameter = (LPVOID)pSnm;

	concurrency::pConThread pctClient = new concurrency::ConThread(hStopEvent, pSnm->acceptThreadFunc, lpParameter);
	pSnm->threadManager.createNewThread(pSnm->serverSocket.sock, pctClient);

	return 1;
}

// test to check that accept function works
int test2(int testNum) {
	std::cout << "IN TEST  " << testNum << "\n";

	server::ServerNetworkManager* pSnm = new server::ServerNetworkManager();
	pSnm->acceptFunc(pSnm);

	return 1;
}

int test3(int testNum) {
	std::cout << "IN TEST  " << testNum << "\n";

	auto roomsSize = 10;

	server::ServerNetworkManager* pSnm = new server::ServerNetworkManager();
	server::RoomManager* pRm = new server::RoomManager;

	for (int i = 0; i < roomsSize; i++) {
		server::pRoom r = new server::Room();
		pRm->createNewRoom(i, r);

		for (int j = 0; j < roomsSize; j++) {
			auto p = new server::RoomClient;
			p->sock = j;
			pRm->addClientToRoom(i, p);
		}
	}

	for (int i = 0; i < roomsSize; i++) {

		for (int j = 0; j < roomsSize; j++) {
			pRm->removeClientFromRoom(i, j);
		}
		pRm->deleteRoom(i);
	}
	pRm->~RoomManager();
	return 1;
}