#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#pragma region Includes

#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <winsock2.h>										
#include <windows.h>

#pragma endregion

#pragma region Using Statements

using namespace std;

#pragma endregion

#pragma region Definitions

#define SERVERIP "127.0.0.1"
#define SERVERPORT 1500
#define CLIENTPORT 1501
#define PACKETSIZE (1024)
#define MSG_CONNECTED (0x41)
#define WM_SOCKET (WM_USER+1)

#pragma endregion

#pragma comment(lib, "ws2_32.lib")

class TCPSocket
{
public:
	TCPSocket();
	~TCPSocket();

	int InitWinSock(void);
	int InitServer(void);
	int InitClient(char* ip);
	int Shutdown();
	
	int Bind();
	int Listen();
	int ConnectToServer(char* ip);

	void SetDestAddr(char* ip, const int port);
	sockaddr_in GetDestAddr(void);

	bool IsConnected;

	SOCKET ServerSocket;
	SOCKET ClientSocket;

	char Buffer[PACKETSIZE];
	int SocketAddrSize;

	sockaddr_in ServerAddr;
	sockaddr_in ClientAddr;

	WSADATA WSAData;
};

#endif