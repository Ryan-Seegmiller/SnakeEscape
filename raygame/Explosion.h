#pragma once
#include "GameObject.h"
class Explosion : GameObject
{
public:
	//Constructor
	Explosion(string, Vector2, int, int);

	//Base Functions
	void Start();
	void Update();
	void Draw();

private:

	void CollisonCheck();//Checks if there are any objects in the area
	void Animation();
	int radius;//Radius of check
	int damage;//Damage it will do
	int animationFrame;
	int frame;
	int textureNum;

	Texture2D currentTexture;

	Color tempColor;

protected:
};

