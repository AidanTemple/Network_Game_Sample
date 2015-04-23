#pragma region Includes

#include "CollisionHandler.h"

#pragma endregion

CollisionHandler::CollisionHandler()
{

}

CollisionHandler::~CollisionHandler()
{

}

int CollisionHandler::CheckForCollision(Sprite &spriteA, Sprite &spriteB)
{	
	if((spriteA.X + (spriteA.Width / 2)) < (spriteB.X -(spriteB.Width / 2))) 
		return 0;

	if((spriteA.X - (spriteA.Width / 2)) > (spriteB.X +(spriteB.Width / 2))) 
		return 0;
	
	if((spriteA.Y + (spriteA.Height / 2)) < (spriteB.Y -(spriteB.Height / 2))) 
		return 0;
	
	if((spriteA.Y - (spriteA.Height / 2)) > (spriteB.Y +(spriteB.Height / 2))) 
		return 0;
		
	return 1;
}