#pragma region Includes
	
#include "TCPSocket.h"
#include <windows.h>												
#include <stdio.h>													
#include <mmsystem.h>												
#include <math.h>													
#include <time.h>													
#include "Sprite.h"												
#include "Collisionhandler.h"
#include "Spaceship.h"
#include <conio.h>
#include "console.h"
#include "GameStateManager.h"
#include "InputHandler.h"

#pragma endregion

#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "msimg32.lib")

InputHandler m_Input;																			

MenuScene TheMenuState;																	
GameScene TheGameState;
GameOverScene TheLoseState;
ClientGameScene TheClientState;																
ServerGameScene TheServerState;																
SceneManager* CurrentState = &TheMenuState;													

bool Accept = false;	
bool ClientAccept = false;
																						
HBITMAP		theOldFrontBitMap, theOldBackBitMap;
HWND        ghwnd;
RECT		screenRect;
HDC			backHDC, frontHDC, bitmapHDC;												

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

void RegisterWindow(HINSTANCE hInstance)												
{
    WNDCLASSEX  wcx;															

    wcx.cbSize        = sizeof (wcx);				
    wcx.style         = CS_HREDRAW | CS_VREDRAW;								
    wcx.lpfnWndProc   = WndProc;												
    wcx.cbClsExtra    = 0;														
    wcx.cbWndExtra    = 0;								
    wcx.hInstance     = hInstance;						
    wcx.hIcon         = 0; 
    wcx.hCursor       = LoadCursor (NULL, IDC_ARROW);											
    wcx.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
    wcx.lpszMenuName  = NULL;							
    wcx.lpszClassName = "AG1107A";				
    wcx.hIconSm       = 0; 

	RegisterClassEx (&wcx);							
}

BOOL InitialiseMyWindow(HINSTANCE hInstance, int nCmdShow)								
{
	HWND        hwnd;
	hwnd = CreateWindow ("AG1107A",															
						 "AG1107A",											
						 (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZE),	
						 CW_USEDEFAULT,			
						 CW_USEDEFAULT,			
						 800,															
						 600,			
						 NULL,					
						 NULL,					
						 hInstance,				
						 NULL);								
	if (!hwnd)
	{
		return FALSE;
	}

    ShowWindow (hwnd, nCmdShow);						
    UpdateWindow (hwnd);	
	ghwnd = hwnd;
	return TRUE;

}

void setBuffers()
{
	GetClientRect(ghwnd, &screenRect);													
	frontHDC = GetDC(ghwnd);															
	backHDC = CreateCompatibleDC(frontHDC);												
	bitmapHDC=CreateCompatibleDC(backHDC);
	theOldFrontBitMap = CreateCompatibleBitmap(frontHDC, screenRect.right, 
		screenRect.bottom);																
    theOldBackBitMap = (HBITMAP)SelectObject(backHDC, theOldFrontBitMap);
																						
	FillRect(backHDC, &screenRect, (HBRUSH)GetStockObject(0));	
}


BOOL WaitFor(unsigned long delay)														
{
	static unsigned long clockStart = 0;
	unsigned long timePassed;
	unsigned long now = timeGetTime();

	timePassed = now - clockStart;
	if (timePassed >  delay)
	{
		clockStart = now;
		return TRUE;
	}
	else
		return FALSE;
}
	
			
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)		
{
	
    switch (message)											
    {														
		case WM_CREATE:	
			break;

		case WM_SIZE:
			setBuffers();
			break;	

		case WM_KEYDOWN:
			m_Input.Key[wParam]=true;													
			break;

		case WM_KEYUP:
			m_Input.Key[wParam]=false;													
			break;

		case WM_PAINT:		
			break;		

		case WM_DESTROY:	
			PostQuitMessage(0);						
			break;	

		case WM_SOCKET:																	 
			if (WSAGETSELECTERROR(lParam))												
			{
				cout << "Socket error\n";												
				printf("%d\n", WSAGETSELECTERROR(lParam));								
				MessageBox (hwnd,"Socket Error","Information",MB_OK);					
				CurrentState->Unload();													
				CurrentState = &TheMenuState;											
				//PostQuitMessage (0);
				return 0;
			}

			switch (WSAGETSELECTEVENT(lParam))											
			{
				case FD_CONNECT:														
					{
						cout << "Connected to server - got FD_CONNECT\n";				
						TheClientState.m_TCPSocket.IsConnected = true;						
						break;															
					}																	

				case FD_READ:															
					{
						recv (TheClientState.m_TCPSocket.ServerSocket,TheClientState.m_TCPSocket.Buffer, PACKETSIZE, 0); 
						memcpy(&TheClientState.MyPacket, TheClientState.m_TCPSocket.Buffer, sizeof(TheClientState.MyPacket));
						printf("Received Packet -> %d\n", TheClientState.MyPacket.Lives);
						break;
					}																	

				case FD_ACCEPT:															
					{
						int Sock_Num, Accepted, ClientAccepted;
						Accepted = 0;
						ClientAccepted = 0;
																						
						
						TheServerState.TCPSocket.ClientSocket = accept (TheServerState.TCPSocket.ServerSocket,(LPSOCKADDR)&TheServerState.TCPSocket.ClientAddr,
							&TheServerState.TCPSocket.SocketAddrSize);

						if (TheServerState.TCPSocket.ClientSocket==INVALID_SOCKET)
						{
							printf("%d\n", WSAGetLastError());
							printf("Error:  Unable to accept connection!\n");	
							WSACleanup ();										
							return 0;
						}
						else
						{
							printf("Accepted Client on socket \n");					
							Accepted = 1;
							Accept = true;											
							break;
						}

						if (Accepted)
						{
							printf("Client has connected!\n");
							printf ("The Following has been sent -> %c\n", TheClientState.MyPacket.Lives);
							memcpy(TheServerState.TCPSocket.Buffer, &TheClientState.MyPacket, sizeof(TheClientState.MyPacket));
							send (TheServerState.TCPSocket.ClientSocket, TheServerState.TCPSocket.Buffer, PACKETSIZE, 0);

						}
						else
						{
							printf("Can't accept any more connections\n");				
							SOCKET s_tmp = accept (TheServerState.TCPSocket.ServerSocket,(LPSOCKADDR)&TheServerState.TCPSocket.ClientAddr,&TheServerState.TCPSocket.SocketAddrSize);
							closesocket (s_tmp);
						}

						int bytes = recv (TheServerState.TCPSocket.ClientSocket,TheServerState.TCPSocket.Buffer,PACKETSIZE,0);

						if(bytes == SOCKET_ERROR)									
						{
							int SocketError = WSAGetLastError();
							printf("Socket servicing client %d has error:, closing the connection\n",  SocketError);
							closesocket (TheServerState.TCPSocket.ClientSocket);				
							TheServerState.TCPSocket.ClientSocket = 0;							
						}
				
							
						if(bytes > 0)												
						{
							memcpy(&TheClientState.MyPacket, TheServerState.TCPSocket.Buffer, sizeof(TheClientState.MyPacket));
							printf("Received from client  -> %s\n", TheClientState.MyPacket.Lives);
							printf("Packet Lives = %d\n", TheClientState.MyPacket.Lives);
							send (TheServerState.TCPSocket.ClientSocket,TheServerState.TCPSocket.Buffer,PACKETSIZE,0);
						}
				
							
						if(bytes == 0)												
						{
							printf("The connection to client has been closed\n");
							closesocket (TheServerState.TCPSocket.ClientSocket);				
							TheServerState.TCPSocket.ClientSocket = 0;							
						}

						// Accept connection from client to server
						TheClientState.m_TCPSocket.ServerSocket = accept(TheClientState.m_TCPSocket.ClientSocket, (LPSOCKADDR)&TheClientState.m_TCPSocket.ServerAddr,
							&TheClientState.m_TCPSocket.SocketAddrSize);

						if(TheClientState.m_TCPSocket.ServerSocket == INVALID_SOCKET)
						{
							WSACleanup();

							return 0;
						}
						else
						{
							ClientAccepted = 1;
							ClientAccept = true;

							break;
						}

						if(ClientAccept)
						{
							memcpy(TheClientState.m_TCPSocket.Buffer, &TheServerState.MyPacket, sizeof(TheServerState.MyPacket));
							send (TheClientState.m_TCPSocket.ServerSocket, TheClientState.m_TCPSocket.Buffer, PACKETSIZE, 0);
						}
						else
						{
							SOCKET temp = accept(TheClientState.m_TCPSocket.ClientSocket, (LPSOCKADDR)&TheClientState.m_TCPSocket.ServerAddr,
								&TheClientState.m_TCPSocket.SocketAddrSize);

							closesocket(temp);
						}

					break;
					}	
				case FD_CLOSE:															
					{
						MessageBox (hwnd,"The server has Quit!","Information",MB_OK);	
						CurrentState->Unload();											
						PostQuitMessage (0);
						break;
					}																	
			}																			
	}													

	return DefWindowProc (hwnd, message, wParam, lParam);		
															
}

void RenderTexture(Sprite theSprite)														
{																						
	HBITMAP originalBitMap;
	originalBitMap = (HBITMAP)SelectObject(bitmapHDC,theSprite.Texture);
	TransparentBlt(backHDC,theSprite.X,theSprite.Y,theSprite.Width,theSprite.Height,bitmapHDC,0,0,theSprite.Width,theSprite.Height,0xFFFFFF);
	SelectObject(bitmapHDC,originalBitMap);
}

void RenderFrame()																		
{
		BitBlt(frontHDC, screenRect.left,screenRect.top, screenRect.right, 
		screenRect.bottom, backHDC, 0, 0, SRCCOPY);
		FillRect(backHDC, &screenRect, (HBRUSH)GetStockObject(0));	
}

void Shutdown()
{
	SelectObject(backHDC,theOldBackBitMap);
	DeleteDC(backHDC);
	DeleteDC(bitmapHDC);
	ReleaseDC(ghwnd,frontHDC);
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	PSTR szCmdLine, int nCmdShow)			
{	
    MSG         msg;	
	HDC	hdcWindow;
	srand ( unsigned long(time(0)));

	RegisterWindow(hInstance);

	
   	if (!InitialiseMyWindow(hInstance, nCmdShow))
	return FALSE;
	
	setBuffers();																		
	
	InitWin32Console();																	
	
	CurrentState = &TheMenuState;														

	while (TRUE)																		
    {		
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))										
		{
		    if (msg.message == WM_QUIT)
				break;

			TranslateMessage (&msg);							
			DispatchMessage (&msg);
		}

		else
		{	
			if (WaitFor(10))
			{	
				CurrentState->Update();													
				CurrentState->Render();													
			}
		}
	}

    Shutdown();
	
	return msg.wParam ;										
}



