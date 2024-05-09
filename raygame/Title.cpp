#include "Title.h"
#include "Game.h"

Title::Title(string newName, string textToDisplay, Vector2 pos, int textSize, Color color) {
	name = newName;//Names the game object
	displayText = textToDisplay; // Sets the display text
	position = pos;//Sets the position of the title
	fontSize = textSize;//Size of the font
	fontColor = color;//Sets the color of the text
	type = Statinoary;

	Start();//Start the object
}
Title::Title(string newName, Text text) {
	name = newName;//Names the game object
	displayText = text.text; // Sets the display text
	position = text.position;//Sets the position of the title
	fontSize = text.fontSize;//Size of the font
	fontColor = text.color;//Sets the text color
	type = Statinoary;

	Start();//Starts the object
}
Title::Title(string newName, Text text, float speed, float time) {
	name = newName;//Names the game object
	displayText = text.text; // Sets the display text
	position = text.position;//Sets the position of the title
	fontSize = text.fontSize;//Size of the font
	fontColor = text.color;//Sets the text color
	shrinkSpeed = speed;
	timeBeforeShrink = time;
	endOpacity = 255;
	type = Shrinkable;

	Start();//Starts the object
}
Title::Title(string newName, Text text, float speed, float time, int opacity) {
	name = newName;//Names the game object
	displayText = text.text; // Sets the display text
	position = text.position;//Sets the position of the title
	fontSize = text.fontSize;//Size of the font
	fontColor = text.color;//Sets the text color
	shrinkSpeed = speed;
	timeBeforeShrink = time;
	endOpacity = opacity;
	type = Shrinkable;

	Start();//Starts the object
}
Title::Title(string newName, Text text, Color rawOutlineColor, int opacity) {
	name = newName;//Names the game object
	displayText = text.text; // Sets the display text
	position = text.position;//Sets the position of the title
	fontSize = text.fontSize;//Size of the font
	fontColor = text.color;//Sets the text color
	outlineColor = Color{ rawOutlineColor.r,rawOutlineColor.g, rawOutlineColor.b, (unsigned char)opacity };
	canDrawOutline = true;
	type = Background;

	Start();//Starts the object
}
void Title::Start() {
	GameObject::Start();//Base update
	classification = UI;//Sets the object type
	CalcTextValues();

}
void Title::Update() {
	GameObject::Update();//Base update
	Shrink();
}
void Title::Draw() {
	//Draws the outline
	DrawOutline();

	//Draws the text
	DrawTextEx(font, displayText.c_str(),centerPosition, fontSize, fontSpacing ,fontColor);
}
void Title::CalcTextValues() {
	fontSpacing = fontSize / 10;//Sets the spacing to be the size divided by 10
	font = Game::GetInstance()->fonts["Main"];//Gets the default font
	textSize = MeasureTextEx(font, displayText.c_str(), fontSize, fontSpacing);//Gets the size of the text

	int posX = position.x - textSize.x / 2;//Centers the x position
	int posY = position.y - textSize.y / 2;//Centers the x position

	centerPosition = { (float)posX,(float)posY };
	int outlineSizer = 10;
	Vector2 outlineSize = { textSize.x + textSize.x / outlineSizer,textSize.y + textSize.y / outlineSizer };
	outlineRec = { centerPosition.x - (textSize.x / outlineSizer) / 2,centerPosition.y - (textSize.y / outlineSizer) / 2,outlineSize.x,outlineSize.y };

}
void Title::DrawOutline() {
	if (!canDrawOutline) { return; }
	CalcTextValues();
	DrawRectanglePro(outlineRec, {0,0}, 0, outlineColor);
}
void Title::Shrink() {
	if (type != Shrinkable) { return; }

	
	if (shrinkTimer <= timeBeforeShrink) { shrinkTimer += GetFrameTime() * 10; return; }

	fontSize = Lerp(fontSize, 0, .1f * shrinkSpeed * GetFrameTime());
	position.y = Lerp(position.y, position.y - textSize.y, shrinkSpeed * GetFrameTime());
	fontColor.a = Lerp(fontColor.a, endOpacity, .1f * shrinkSpeed * GetFrameTime());

	CalcTextValues();

	if (fontSize <= 0.5) {
		Death();
	}
}