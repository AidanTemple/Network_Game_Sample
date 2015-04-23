#ifndef UDPSOCKET_H
#define UDPSOCKET_H

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

typedef  int socklen_t;

class UDPSocket
{
public:
	UDPSocket();
	~UDPSocket();

	int Initialise(void);
	int NonBlocking(void);
	int Bind(const int port);
	int Receive(char* buffer);
	int Send(char* buffer);
	int Shutdown();

	sockaddr_in GetDestAddr(void);
	void SetDestAddr(char* ip, const int port);

	SOCKET Socket;

protected:
	socklen_t m_SocketAddrSize;

	struct sockaddr_in m_LocalAddr;
	struct sockaddr_in m_RemoteAddr;

	WSADATA m_WSAData;
};

#endif