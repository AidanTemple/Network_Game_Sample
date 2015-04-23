#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#pragma region Includes

#include "Sprite.h"

#pragma endregion

class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();

	int CheckForCollision(Sprite &spriteA, Sprite &spriteB);
};

#endif