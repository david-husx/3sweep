// server.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <WinSock2.h>
#include <iostream>

using namespace std;

#define PORT 4000
#define IP_ADDRESS "127.0.0.1"
#define BUFF_SIZE 1000

DWORD WINAPI ClientThread(LPVOID IpParameter){
	SOCKET clientSocket = (SOCKET)IpParameter;
	int ret = 0;
	char buff[BUFF_SIZE];

	//Control control;
	while (true){
		memset(buff, 0x00, BUFF_SIZE);
		ret = recv(clientSocket, buff, BUFF_SIZE, 0);
		if ((ret == 0) || (ret == SOCKET_ERROR)){
			cout << "client quit" << endl;
			break;
		}
		//control.control(buff);
		cout << "Buff: " << buff << endl;
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA ws;
	SOCKET serverSocket, clientSocket;
	struct sockaddr_in localAddr, clientAddr;
	int ret = 0;
	int addrLen = 0;
	HANDLE hThread = NULL;

	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0){
		cout << "init Windows socket failed" << endl;
		return -1;
	}

	serverSocket= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET){
		cout << "create socket failed" << endl;
		return -1;
	}

	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	localAddr.sin_port = htons(PORT);
	memset(localAddr.sin_zero, 0, 8);

	ret = bind(serverSocket, (struct sockaddr*)&localAddr, sizeof(localAddr));
	if (ret != 0){
		cout << "bind failed" << endl;
	}

	ret = listen(serverSocket, 10);
	if (ret != 0){
		cout << "listen failed" <<endl;
	}
	cout << "server is on work.." << endl;

	while (true){
		addrLen = sizeof(clientAddr);
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET) {
			cout << "accept failed" << endl;
			break;
		}
		cout << "client is connected: " << inet_ntoa(clientAddr.sin_addr) << ": " << clientAddr.sin_port << endl;
		hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)clientSocket, 0, NULL);
		if (hThread == NULL){
			cout << "create thread failed" << endl;
			break;
		}

		CloseHandle(hThread);
	}

	closesocket(serverSocket);
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}

