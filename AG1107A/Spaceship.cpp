#pragma region Includes

#include "Spaceship.h"
#include "InputHandler.h"

#pragma endregion

extern InputHandler m_Input;							

Spaceship::Spaceship()
{
	m_Speed = 5;
}

Spaceship::~Spaceship()
{

}

void Spaceship::Update()								
{
	UpdateInput();
}

void Spaceship::UpdateInput()
{
	if(m_Input.Key[VK_LEFT])
	{
		VelocityX = -m_Speed;
		X += VelocityX;								
	}
	else if(m_Input.Key[VK_RIGHT])
	{
		VelocityX = m_Speed;
		X += VelocityX;
	}
	else if(m_Input.Key[VK_UP])
	{
		VelocityY = -m_Speed;
		Y+=VelocityY;
	}
	else if(m_Input.Key[VK_DOWN])
	{
		VelocityY = m_Speed;
		Y += VelocityY;
	}
	else
	{
		VelocityX = 0;								
		VelocityY = 0;
	}
}