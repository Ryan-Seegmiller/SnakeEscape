#pragma once
#include "GameObject.h"
#include "Collectible.h"
class Bar : GameObject
{
public:
	Bar(string, Vector2, int, Color, CollectibleType);

	void Update();
	void Draw();
	void Start();

	int currentLength;
	CollectibleType type;
private:
	int maxLength;
	
	Rectangle lineBorder;
	Color lineColor;

	void DrawBarLine();


};

