#ifndef SPRITE_H
#define SPRITE_H

#pragma region Includes

#include "windows.h"

#pragma endregion

class Sprite								
{
public:
	Sprite();		
	~Sprite();

	int X;
	int Y; 
	int Height; 
	int Width;					

	HBITMAP Texture;

	void Initialise(LPSTR filename);			
	HBITMAP LoadTexture(LPSTR fileName);		
};

#endif