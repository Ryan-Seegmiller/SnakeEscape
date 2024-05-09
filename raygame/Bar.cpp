#include "Bar.h"

Bar::Bar(string newName, Vector2 pos, int length, Color color, CollectibleType newType)
{
	name = newName;
	position = pos;
	maxLength = length;
	lineColor = color;
	type = newType;

	Start();
}

void Bar::Update()
{
	GameObject::Update();
}

void Bar::Draw()
{
	DrawBarLine();
}

void Bar::Start()
{
	GameObject::Start();
	
}


void Bar::DrawBarLine()
{
	float difference = maxLength - currentLength;
	float average = (maxLength + currentLength) / 2;
	float perecntage = (float)currentLength / (float)maxLength;

	DrawLineEx(position, Vector2{ position.x + tileSize - (tileSize - (tileSize * perecntage)) , position.y}, tileSize / 7, lineColor);
}
