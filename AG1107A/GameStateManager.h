#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#pragma region Includes

#include "SceneManager.h"
#include "TCPSocket.h"
#include "UDPSocket.h"

#pragma endregion

#pragma region Definitions

#define MAXASTEROIDCOUNT 10

#pragma endregion

#pragma region Menu Scene

class MenuScene : public SceneManager									
{
public:
	MenuScene();													
	~MenuScene();
	
	void Initialise();
	void Unload();
	void Update();													
	void Render();
}; 

#pragma endregion 

#pragma region Game Scene

class GameScene : public SceneManager										
{
public:
	GameScene();													
	~GameScene();
	
	void Initialise();
	void Unload();
	void Update();													
	void UpdatePlayer();																						
	void Render();
};

#pragma endregion

#pragma region Game Over Scene

class GameOverScene : public SceneManager										
{
public:
	GameOverScene();													
	~GameOverScene();
	
	void Initialise();
	void Unload();
	void Update();													
	void Render();
};

#pragma endregion

#pragma region Client Game Scene

class ClientGameScene : public SceneManager																
{
public:
	ClientGameScene();														
	~ClientGameScene();
	
	void Initialise();
	void Unload();
	void Shutdown();
	void Update();													
	void Render();

	UDPSocket m_UDPSocket;												
	TCPSocket m_TCPSocket;	

	char Buffer[PACKETSIZE];										

	struct MyPacket_t												
	{
		int SpeedX;												
		int SpeedY;

		int Lives;
		bool IsPaused;

		int SpaceshipX;
		int SpaceshipY; 										
		int AsteroidX[MAXASTEROIDCOUNT];
		int AsteroidY[MAXASTEROIDCOUNT];									

	}MyPacket, ReceivePacket;										
}; 

#pragma endregion

#pragma region Server Game Scene

class ServerGameScene : public SceneManager									
{
public:
	ServerGameScene();															
	~ServerGameScene();
	
	void Initialise();
	void Unload();
	void Shutdown();
	void Update();													
	void Render();

	TCPSocket TCPSocket;												
	UDPSocket UDPSocket;

	char Buffer[PACKETSIZE];										
	int Latency;													 
																	
	struct MyPacket_t												
	{
		int SpeedX;												
		int SpeedY;

		int Lives;
		bool IsPaused;

		int SpaceshipX;
		int SpaceshipY; 										
		int AsteroidX[MAXASTEROIDCOUNT];
		int AsteroidY[MAXASTEROIDCOUNT];									

	}MyPacket, ReceivePacket;										
}; 

#pragma endregion

#endif