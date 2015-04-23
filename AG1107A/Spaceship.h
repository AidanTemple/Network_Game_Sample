#ifndef SPACESHIP_H
#define SPACESHIP_H

#pragma region Includes

#include "Sprite.h"

#pragma endregion

class Spaceship : public Sprite	
{
public:
	Spaceship();					
	~Spaceship();
	
	int VelocityX;				
	int VelocityY;				

	void Update();		
	void UpdateInput();

private:
	int m_Speed;
};

#endif
