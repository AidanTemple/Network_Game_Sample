#pragma region Includes

#include "TCPSocket.h"
#include <stdio.h>

#pragma endregion

#pragma comment(lib, "ws2_32.lib")

TCPSocket::TCPSocket()
{
	SocketAddrSize = sizeof(sockaddr_in);
	ServerSocket = 0;
	ClientSocket = 0;

	IsConnected = false;
}

TCPSocket::~TCPSocket()
{
	Shutdown();
}

int TCPSocket::InitServer(void)
{
	if(!InitWinSock())
	{
		cout << "Error initialising WinSock\n";		
	}
	else 
	{
		cout << "WinSock initialised \n";
	}
																						
	ServerSocket = 0;																				//Set all socket descriptors to 0
	ClientSocket = 0;
																	
	ServerSocket = socket (AF_INET, SOCK_STREAM, 0);													//Create the TCP listening socket
	ServerAddr.sin_family = AF_INET;															//Internetwork, TCP, UDP
	ServerAddr.sin_port = htons (SERVERPORT);													//Sets the Server Port
	ServerAddr.sin_addr.s_addr = htonl (INADDR_ANY);											//Sets the Server Address

																	
	u_short port;																		//Notice the different byte order for network
	port = (ServerAddr.sin_port >> 8) | (ServerAddr.sin_port << 8);

	printf("PORT connected to is: ");
	printf("%d\n\n", (int)port);

	return 1;
}

int TCPSocket::InitClient(char* ip)
{
	if(!InitWinSock())
	{
		cout << "Error initialising WinSock\n";		
	}
	else 
	{
		cout << "WinSock initialised\n";
	}
													
	ServerSocket = 0;																				//Set all socket descriptors to 0
	ClientSocket = 0;																																
	ClientSocket = socket (AF_INET, SOCK_STREAM, 0);													//Create the TCP listening socket	
																						
	ClientAddr.sin_family = AF_INET;															//Internetwork, TCP, UDP
	ClientAddr.sin_port = htons (SERVERPORT);													//Sets the Server Port
	ClientAddr.sin_addr.s_addr = inet_addr(ip);											//Sets the Server Address

	printf("Server IP: ");											//Print Statements
	printf("%d.", (int)ClientAddr.sin_addr.S_un.S_un_b.s_b1);
	printf("%d.", (int)ClientAddr.sin_addr.S_un.S_un_b.s_b2);
	printf("%d.", (int)ClientAddr.sin_addr.S_un.S_un_b.s_b3);
	printf("%d\n", (int)ClientAddr.sin_addr.S_un.S_un_b.s_b4);
										
	u_short port;																		//Notice the different byte order for network
	port = (ClientAddr.sin_port >> 8) | (ClientAddr.sin_port << 8);

	printf("The PORT being connected to is: ");
	printf("%d\n\n", (int)port);

	return 1;
}

int TCPSocket::InitWinSock(void)
{
	WSADATA data;
	int error = WSAStartup (0x0202, &data);	
	
	if (error)																			//Check to see if you have winsock
	{
		printf("Error: Unable to initialise WinSock 2.2\n");

		return 0;
	}

	if (data.wVersion != 0x0202)																//Check to see if you have the right version of winsock
	{
		printf("Error: Wrong WinSock version\n");
		WSACleanup ();

		return 0;
	}

	return 1;
}

int TCPSocket::Bind()																					
{														
	if (bind(ServerSocket, (LPSOCKADDR)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)								
	{																					
		printf("Unable to bind socket, Error: %d\n", WSAGetLastError());

		WSACleanup ();

		return 0;
	}

	return 1;
}

int TCPSocket::Listen()
{
	if (listen(ServerSocket, 1) == SOCKET_ERROR)														
	{									
		printf("Unable to Listen, Error: %d\n" , WSAGetLastError());

		WSACleanup ();

		return 0;
	}

	return 1;
}

int TCPSocket::ConnectToServer(char* ip)												
{																						
	int data = connect(ClientSocket, (LPSOCKADDR)&ClientAddr, SocketAddrSize);									
	
	if(data == SOCKET_ERROR)
	{
		if(WSAGetLastError() == WSAEWOULDBLOCK)										
		{
			return 1;
		}
		else
		{
			printf("Error: %d\n" , WSAGetLastError());

			return 0;
		}
	}
	
	return 1;
}

int TCPSocket::Shutdown()
{
	if(ServerSocket)
	{
		closesocket(ServerSocket);
	}

	if(ClientSocket)
	{
		closesocket(ClientSocket);
	}

	WSACleanup();
	Sleep(1000);

	ServerSocket = 0;
	ClientSocket = 0;

	return 1;
}