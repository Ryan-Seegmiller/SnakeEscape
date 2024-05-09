#pragma once
#include "GameObject.h"
#include "Title.h"
#include <functional>
#include <iostream>
#include "Title.h"
#include "ImageUI.h"
#include "UI.h"

class Button : GameObject
{
public:

	//Constructor
	Button(string, string, Vector2, Vector2, function<void(void)>);
	Button(string, ButtonBounds);
	Button(string, ButtonBounds, Sprite);

	//Base methods
	void Start();
	void Update();
	void Draw();

	bool selected;

private:
	string displayText;// Button text
	Text text;// Button text values
	Rectangle buttonBounds;// Button values
	Vector2 buttonSize;//Size if the button
	Color buttonColor;//Button color
	Vector2 buttonOffset;//Offset for the button hover check
	Vector2 mousePosition;
	Sprite spriteValues;//Sprite struct that holds the values
	ImageUI* buttonSprite;//Sprite for the button
	bool drawTexture;//If the button is a draw texture
	bool isHovering;//if hovering
	
	Color normalColor;//Normal color
	Color hoverColor;//Hover color
	Color selectColor;

	Title* buttonText;//The text object for the button
	function<void(void)> event;//Function to play if the button is clicked

	//Button mouse values
	bool HoverCheck();//Checks if the mouse is over the button
	void ClickCheck();//Checks if the button is clicked

	void DrawButtonRec();//Draws button without a sprite
	void DrawButtonTexture();//Draws button with a sprite

protected:

};

