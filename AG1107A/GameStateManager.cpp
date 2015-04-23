#pragma region Includes

#include "TCPSocket.h"
#include "UDPSocket.h"
#include "GameStateManager.h"	
#include <iostream>						
#include "Spaceship.h"
#include "CollisionHandler.h"
#include "Sprite.h"
#include "InputHandler.h"
#include <sys/types.h>
#include <sys/timeb.h>

#pragma endregion

#pragma region Definitions

#define LOCALPORT (1500)																					
#define REMOTEPORT (1501)
#define MAXASTEROIDCOUNT 10

#pragma endregion

#pragma region Externs

extern InputHandler m_Input;
extern SceneManager* CurrentState;									
extern MenuScene TheMenuState;	
extern GameScene TheGameState;
extern GameOverScene TheLoseState;
extern ClientGameScene TheClientState;
extern ServerGameScene TheServerState;
extern void RenderTexture(Sprite theSprite);
extern void RenderFrame();
extern RECT	screenRect;
extern HWND ghwnd;
extern bool Accept;

#pragma endregion

#pragma region Fields

Spaceship m_Spaceship;
Sprite m_GameTexture;
Sprite m_MenuTexture;
Sprite m_GameOverTexture;
Sprite m_PauseTexture;				
Sprite m_Asteroids[MAXASTEROIDCOUNT];

int m_Lives = 3;												
bool m_IsPaused = false;										

long long int get_time();
void DeadReckoningAlgorithm();
bool ReceiveFunction();

CollisionHandler m_CollisionHandler;

#pragma endregion

#pragma region Scenes

#pragma region Menu Scene

MenuScene::MenuScene()										
{	
	InitialiseBool = false;
}

MenuScene::~MenuScene()										
{

}

void MenuScene::Initialise()						
{
	m_MenuTexture.Initialise("menu.bmp");
	InitialiseBool = true;
}

void MenuScene::Unload()									
{
	InitialiseBool = false;
}

void MenuScene::Update()									
{
	if(InitialiseBool == false)	
	{
		Initialise();		
	}

	if(m_Input.Key[VK_RETURN])								
	{
		Unload();
		CurrentState = &TheGameState;
	}

	if(m_Input.Key[0x46])					
	{
		Unload();
		CurrentState = &TheClientState;						
	}

	if(m_Input.Key[0x48])					
	{
		Unload();
		CurrentState = &TheServerState;						
	}
}

void MenuScene::Render()
{	
	RenderTexture(m_MenuTexture);										
	RenderFrame();
}

#pragma endregion

#pragma region Game Scene

GameScene::GameScene()										
{
	InitialiseBool = false;
}

GameScene::~GameScene()										
{

}

void GameScene::Initialise()														
{	
	m_GameTexture.Initialise("background.bmp");									
	m_Spaceship.Initialise("sprite.bmp");
	m_PauseTexture.Initialise("pause.bmp");	

	m_Lives = 3;																			
	m_IsPaused = false;																

	for(int i = 0; i < MAXASTEROIDCOUNT; i++)														
	{
		m_Asteroids[i].Initialise("enemy.bmp");
		m_Asteroids[i].X = rand() % (screenRect.right - m_Asteroids[i].Width) + 1;				
		m_Asteroids[i].Y = -400 + (rand() % 400 + 1);												
	}

	m_Spaceship.X = ((screenRect.right) / 2);											
	m_Spaceship.Y = screenRect.bottom - m_Spaceship.Height;	

	m_GameTexture.Y = screenRect.bottom;
	m_GameTexture.Y = (m_GameTexture.Y - (m_GameTexture.Height));						

	InitialiseBool = true;															
}

void GameScene::Unload()
{
	InitialiseBool = false;
}

void GameScene::Update()									
{
	if(InitialiseBool == false)
	{
		Initialise();
	}
	
	if(m_IsPaused == true)															
	{
		if(m_Input.Key[VK_ESCAPE])												
		{
			m_IsPaused = false;		
			m_Input.Key[VK_ESCAPE] = false;	
		}
	}
	else
	{
		for(int i = 0; i < MAXASTEROIDCOUNT; i++)
		{
			m_Asteroids[i].Y += 1;

			if(m_CollisionHandler.CheckForCollision(m_Spaceship, m_Asteroids[i]) == true)						
			{																	
				m_Asteroids[i].X = rand() % (screenRect.right - m_Asteroids[i].Width ) + 1;		
				m_Asteroids[i].Y = -400 + (rand() % 400 + 1);							

				m_Lives--;														
			}
		}

		UpdatePlayer();	
	}

	if(m_Lives <= 0)																
	{
		CurrentState->Unload();
		CurrentState = &TheLoseState;											
	}

	if(m_Input.Key[VK_ESCAPE])													
	{
		m_IsPaused = true;
		m_Input.Key[VK_ESCAPE] = false;														
	}
}

void GameScene::UpdatePlayer()
{
	if(m_Spaceship.X > screenRect.right - m_Spaceship.Width)								
	{																			 
		m_Spaceship.X = (screenRect.right - m_Spaceship.Width);							
	}

	if(m_Spaceship.X < screenRect.left)
	{																			
		m_Spaceship.X = screenRect.left;
	}

	if(m_Spaceship.Y > screenRect.bottom - m_Spaceship.Height)
	{
		m_Spaceship.Y = (screenRect.bottom - m_Spaceship.Height);							
	}

	if(m_Spaceship.Y < screenRect.top)
	{
		m_Spaceship.Y = screenRect.top;												
	}

	m_Spaceship.Update();
}

void GameScene::Render()															
{
	RenderTexture(m_GameTexture);														
	RenderTexture(m_Spaceship);															
	
	if(!m_IsPaused && m_Lives != 0)
	{
		for(int i = 0; i < MAXASTEROIDCOUNT; i++)
		{	
			if(m_Asteroids[i].Y > screenRect.bottom)									
			{
				m_Asteroids[i].X = rand() % (screenRect.right - m_Asteroids[i].Width) + 1;		
				m_Asteroids[i].Y = -400 + (rand() % 400 + 1);							
			}
	
			RenderTexture(m_Asteroids[i]);											
		}
	}

	if(m_IsPaused)		
	RenderTexture(m_PauseTexture);		
	RenderFrame();
}

#pragma endregion

#pragma region Game Over Scene

GameOverScene::GameOverScene()															
{
	InitialiseBool = false;
}

GameOverScene::~GameOverScene()															
{

}

void GameOverScene::Initialise()														
{									
	m_GameOverTexture.Initialise("lose.bmp");	
	InitialiseBool = true;															
}									
									
void GameOverScene::Unload()			
{									
	InitialiseBool = false;														
}	

void GameOverScene::Update()
{	
	if(InitialiseBool == false)	
	{
		Initialise();
	}
	
	if(m_Input.Key[VK_SPACE])															
	{
		Unload();
		CurrentState = &TheMenuState;											
	}	
}

void GameOverScene::Render()
{
	RenderTexture(m_GameOverTexture);															
	RenderFrame();
}									

#pragma endregion
		
#pragma region Client Game Scene

ClientGameScene::ClientGameScene()														
{									
	InitialiseBool = false;			
}									
									
ClientGameScene::~ClientGameScene()														
{		

}	

void ClientGameScene::Shutdown()
{
	if(m_TCPSocket.ServerSocket)
	{
		closesocket(m_TCPSocket.ServerSocket);
	}

	if(m_TCPSocket.ClientSocket)
	{
		closesocket(m_TCPSocket.ClientSocket);
	}

	if(m_UDPSocket.Socket)
	{
		closesocket(m_UDPSocket.Socket);
	}

	WSACleanup();
}
									
void ClientGameScene::Update()														
{									
	if(InitialiseBool == false)
	{
		Initialise();		
	}

	if(!ReceiveFunction())														
	{
		DeadReckoningAlgorithm();												
	}
}

void ClientGameScene::Render()								
{
	TheGameState.Render();														
}

void ClientGameScene::Initialise()														
{	
	InitialiseBool = true;

	char IPADDRESS[128];															

	m_TCPSocket.InitClient((char *) "127.0.0.1");									
	
	//Make socket asynchronous and hook to WM_SOCKET message
	WSAAsyncSelect (m_TCPSocket.ClientSocket,ghwnd,WM_SOCKET,(FD_CLOSE | FD_CONNECT | FD_READ | FD_ACCEPT));		
	
	m_TCPSocket.ConnectToServer("127.0.0.1");																	 
	
	m_UDPSocket.Initialise();																			
  	m_UDPSocket.NonBlocking();																	
	m_UDPSocket.Bind(REMOTEPORT);																		
	m_UDPSocket.SetDestAddr("127.0.0.1", LOCALPORT);											

	TheGameState.Initialise();																			
}

void ClientGameScene::Unload()																			
{	
	Shutdown();	

	TheGameState.Unload();																			
	InitialiseBool = false;
}

#pragma endregion

#pragma region Server Game Scene

ServerGameScene::ServerGameScene()																
{
	InitialiseBool = false;
	Latency = 100;																
}																					

ServerGameScene::~ServerGameScene()															
{

}

void ServerGameScene::Update()							
{
	if(InitialiseBool == false)		
	{
		Initialise();
	}

	TheGameState.Update();															

	MyPacket.SpeedX = m_Spaceship.VelocityX;											
	MyPacket.SpeedY = m_Spaceship.VelocityY;

	MyPacket.SpaceshipX = m_Spaceship.X;
	MyPacket.SpaceshipY = m_Spaceship.Y;

	for(int i = 0; i < MAXASTEROIDCOUNT; i++)
	{
		MyPacket.AsteroidX[i] = m_Asteroids[i].X;
		MyPacket.AsteroidY[i] = m_Asteroids[i].Y;
	}

	MyPacket.Lives = m_Lives;
	MyPacket.IsPaused = m_IsPaused;

	if(Accept == true)																
	{
		int n;																		
		static long long int time;

		if(time != get_time()/Latency)											
		{
			time = get_time()/Latency;											

			n = UDPSocket.Send((char *)&MyPacket);									
			
			if(n<0) 
			{
				printf("Send failed\n");
			}

			// TODO: Send TCP data

			printf("Sent %d to IP:%s UDP PORT:%u\n",
				MyPacket.Lives,
			inet_ntoa(UDPSocket.GetDestAddr().sin_addr),
			ntohs(UDPSocket.GetDestAddr().sin_port));
		}
	}
}

void ServerGameScene::Render()
{
	TheGameState.Render();															
}

void ServerGameScene::Initialise()							
{	
	InitialiseBool = true;

	char IPADDRESS[128];
	/*cout << "Enter An IP Address" << endl;
	cin >> IPADDRESS;*/

	TheGameState.Initialise();

	TCPSocket.InitServer();
	TCPSocket.Bind();
	TCPSocket.Listen();

	//Make socket asynchronous and hook to WM_SOCKET message
	WSAAsyncSelect (TCPSocket.ServerSocket,ghwnd,WM_SOCKET,(FD_CLOSE | FD_CONNECT | FD_READ | FD_ACCEPT));

	UDPSocket.Initialise();
  	UDPSocket.NonBlocking();

	//The socket must be bound so that we know the port number
  	UDPSocket.Bind(LOCALPORT);
	UDPSocket.SetDestAddr("127.0.0.1", REMOTEPORT);
}

void ServerGameScene::Unload()															
{	
	Accept = false;																	
			
	Shutdown();

	TheGameState.Unload();															
	InitialiseBool = false;
}

void ServerGameScene::Shutdown()
{
	if(TCPSocket.ServerSocket)
	{
		closesocket(TCPSocket.ServerSocket);	
	}

	if(TCPSocket.ClientSocket)
	{
		closesocket(TCPSocket.ClientSocket);	
	}

	if(UDPSocket.Socket)
	{
		closesocket(UDPSocket.Socket);	
	}

	WSACleanup();
}

#pragma endregion

#pragma region Helper Methods

long long int get_time()
{															
	struct _timeb timebuffer;
	_ftime64_s(&timebuffer);
	return (timebuffer.time *1000LL) + timebuffer.millitm;
}

void DeadReckoningAlgorithm()														
{
	m_Spaceship.X += m_Spaceship.VelocityX;														 
	m_Spaceship.Y += m_Spaceship.VelocityY;														

	for(int i = 0; i < MAXASTEROIDCOUNT; i++)
	{
		if(m_CollisionHandler.CheckForCollision(m_Spaceship, m_Asteroids[i]) == true)								
		{																			
			m_Asteroids[i].X = rand() % (screenRect.right - m_Asteroids[i].Width) + 1;
			m_Asteroids[i].Y = -400 + (rand()%400 + 1);
			m_Lives--;																															
		}
	}

	TheGameState.UpdatePlayer();														
}

bool ReceiveFunction()																
{										 
	int data = TheClientState.m_UDPSocket.Receive((char*)&TheClientState.ReceivePacket);

	if(data <= 0)																																 
	{		
		return false;
	}

	printf("Received from %s:UDP%u : %d \n", 
	inet_ntoa(TheClientState.m_UDPSocket.GetDestAddr().sin_addr),
	ntohs(TheClientState.m_UDPSocket.GetDestAddr().sin_port), TheClientState.ReceivePacket.Lives);

	m_Spaceship.VelocityX = TheClientState.ReceivePacket.SpeedX;							 
	m_Spaceship.VelocityY = TheClientState.ReceivePacket.SpeedY;							 

	m_Lives = TheClientState.ReceivePacket.Lives;									
	m_IsPaused = TheClientState.ReceivePacket.IsPaused;								

	m_Spaceship.X = TheClientState.ReceivePacket.SpaceshipX;
	m_Spaceship.Y = TheClientState.ReceivePacket.SpaceshipY;

	for(int i = 0; i < MAXASTEROIDCOUNT; i++)
	{
		m_Asteroids[i].X = TheClientState.ReceivePacket.AsteroidX[i];
		m_Asteroids[i].Y = TheClientState.ReceivePacket.AsteroidY[i];
	}

	return true;
}

#pragma endregion

#pragma endregion