#pragma region Includes

#include "UDPSocket.h"
#include <stdio.h>

#pragma endregion

#pragma comment(lib, "ws2_32.lib")

UDPSocket::UDPSocket()																	
{
	m_SocketAddrSize = sizeof(sockaddr_in);											
	Socket = 0;																		
}

UDPSocket::~UDPSocket()																	
{
	Shutdown();																	
}

int UDPSocket::NonBlocking(void)													
{
	u_long isNonBlocking = true;	
	
	if ((ioctlsocket(Socket, FIONBIO, (u_long*)&isNonBlocking)) == SOCKET_ERROR)		
	{
		printf("Error:  Can't make Socket nonblocking\n");
		printf("%d\n" , WSAGetLastError());

		return 0;
	}
		
	return 1;
}
	
int UDPSocket::Initialise(void)															
{
	int error = WSAStartup(0x0202,&m_WSAData);
	
	if (error)																			
	{
		printf("You need WinSock 2.2\n");	

		return 0;
	}

	if (m_WSAData.wVersion!=0x0202)														
	{
		printf("Error:  Wrong WinSock version!\n");		

		WSACleanup ();

		return 0;
	}
																				
	Socket = socket(AF_INET, SOCK_DGRAM, 0);
	 
	if(Socket < 0)																		
	{
		printf("%d\n" , WSAGetLastError());
		printf("Cannot open socket \n");

		return 0;
	}
	
	return 1;
}

int UDPSocket::Bind(const int Port)														
{																			
	m_LocalAddr.sin_family = AF_INET;											
	m_LocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);								
	m_LocalAddr.sin_port = htons(Port);										
	
	int result = bind (Socket, (struct sockaddr *)&m_LocalAddr, m_SocketAddrSize);	

	if(result < 0)																			
	{
		printf("%d\n" , WSAGetLastError());
		printf("cannot bind port number %d \n", Port);

		return 0;
	}

	return 1;	
}

int UDPSocket::Receive(char * Buffer)													
{																						
	int data = recvfrom(Socket, Buffer, PACKETSIZE, 0, (struct sockaddr*)&m_RemoteAddr, 
		&m_SocketAddrSize);
	
	return data;	
}

int UDPSocket::Send(char * Buffer)														
{
	int data = sendto(Socket, Buffer, PACKETSIZE, 0, (struct sockaddr*)&m_RemoteAddr, 
		m_SocketAddrSize);
	
	return data;	
}

sockaddr_in UDPSocket::GetDestAddr(void)										
{
	return m_RemoteAddr;
}

void UDPSocket::SetDestAddr(char * IP, const int Port)						
{																						
	m_RemoteAddr.sin_family = AF_INET;	
	m_RemoteAddr.sin_port = htons (Port);
	m_RemoteAddr.sin_addr.s_addr = inet_addr (IP);									
}

int UDPSocket::Shutdown()
{
	if(Socket)
	{
		closesocket(Socket);
	}
	
	WSACleanup ();																		
	
	Socket = 0;	

	return 1;
}