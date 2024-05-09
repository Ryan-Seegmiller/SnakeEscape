#pragma once
#include "GameObject.h"
#include "UI.h"
class ImageUI:GameObject
{
private:
	Rectangle source;//Source rectangle
	Rectangle dest;//Destination rectangle
	Vector2 origin;//Origin

	
public:
	Sprite values;

	//Constructors
	ImageUI(string, Vector2, Texture2D, float);
	ImageUI(string, Sprite);
	

	//Base
	void Start();
	void Update();
	void Draw();

	void CalcDimensions();

	Color tint;
	Vector2 imageSize;

protected:

};

