#pragma once
#include "GameObject.h"
enum TextType {
	Statinoary,
	Background,
	Shrinkable
};

class Title : GameObject
{
public:
	
	//Constructors
	Title(string, string, Vector2, int, Color);
	Title(string, Text);
	Title(string, Text, float, float);
	Title(string, Text, float, float, int);
	Title(string, Text, Color, int);

	//Base methods
	void Start();
	void Update();
	void Draw();
	void Shrink();

	int fontSize;//Font size
	string displayText;//Text to be on display
	TextType type;

private:
	int fontSpacing;//Space between the letters
	Color fontColor;//Color of the text
	Vector2 textSize;
	Vector2 centerPosition;
	Font font;
	void CalcTextValues();
	

	//Outline
	Rectangle outlineRec;
	Vector2 outlineOffset;
	Color outlineColor;
	bool canDrawOutline;
	void DrawOutline();

	//Shrinking
	float shrinkSpeed;
	float timeBeforeShrink;
	float shrinkTimer;
	int endOpacity;
protected:

};

