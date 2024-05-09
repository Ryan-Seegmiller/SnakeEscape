#pragma once
#include <iostream>
#include "raymath.h"
#include "raylib.h"
#include <functional>

using namespace std;

	//Struct with all the values the text needs
	typedef struct Text {
		string text;
		Vector2 position;
		int fontSize;
		Color color;
	};

	//Image Strcut
	typedef struct Sprite {
		Vector2 position;//Position
		Texture2D texture;//Texture
		float scale;//Scale
	};

	//Struct for the values needed in the object
	typedef struct ButtonBounds {
		string text;//Text for the text struct
		Color textColor;//Color for the text
		Vector2 position;//position of the buuton
		Vector2 size;//Size of the button
		function<void(void)> event;//Fucntion to play when the button is clicked
	};
