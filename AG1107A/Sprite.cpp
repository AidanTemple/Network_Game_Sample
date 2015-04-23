#pragma region Includes

#include "Sprite.h"

#pragma endregion

Sprite::Sprite()
{

}

Sprite::~Sprite()
{

}

void Sprite::Initialise(LPSTR filename)
{
	BITMAP texture;
	Texture = LoadTexture(filename);

	GetObject(Texture, sizeof(BITMAP), &texture);
	Height = texture.bmHeight;	 
	Width = texture.bmWidth;
}

HBITMAP Sprite::LoadTexture(LPSTR filename)
{
	return (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0 , 0, LR_LOADFROMFILE);
}