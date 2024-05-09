#include "ImageUI.h"
#include "Game.h"

ImageUI::ImageUI(string newName, Vector2 pos, Texture2D texture, float imageScale) {
	name = newName;//Sets the name
	values.position = pos;//Sets the position
	values.texture = texture;//Sets the texture
	values.scale = imageScale;//Sets the scale

	Start();//Starts the object
}
ImageUI::ImageUI(string newName, Sprite imageValues) {
	name = newName;//Sets the name of the object
	values = imageValues;//Sets the sprite values
	

	Start();//Starts the object
}

void ImageUI::Start() {
	GameObject::Start();//Base start

	classification = UI;//Sets the type of object

	imageSize = { (float)values.texture.width, (float)values.texture.height };

	CalcDimensions();

	tint = WHITE;
}
void ImageUI::Update() {
	GameObject::Update();//base update
}
void ImageUI::Draw() {
	CalcDimensions();
	//Draws the sprite
	DrawTexturePro(values.texture, source, dest, origin, 0, tint);
}
void ImageUI::CalcDimensions() {

	Vector2 size = { imageSize.x * values.scale, imageSize.y * values.scale };

	Vector2 center = { values.position.x - size.x / 2, values.position.y - size.y / 2 };//Centers the sprite

	origin = { 0,0 };//Origin
	source = { 0, 0, (float)values.texture.width, (float)values.texture.height };//Source rec
	dest = { center.x, center.y, size.x, size.y };//Destination rec
}