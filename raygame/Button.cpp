#include "Button.h"
#include "Game.h"
#include "UIManager.h"


Button::Button(string newName, string textToDisplay, Vector2 pos, Vector2 size, function<void(void)> task) {
	name = newName;//Names the game object
	text.text = textToDisplay; // Sets the display text
	position = pos;//Sets the position of the title
	buttonSize = size;//Sets the size of the rectangle
	event = task;//Function to play when the button is clicked
	text.color = RED;//Color of text

	normalColor = Color{ 150, 150, 150, 100 };
	hoverColor = Color{ 130, 130, 130, 100 };

	Start();//Starts the game object
}
Button::Button(string newName, ButtonBounds button) {
	name = newName;//Names the game object
	text.text = button.text; // Sets the display text
	position = button.position;//Sets the position of the title
	buttonSize = button.size;//Sets the size of the rectangle
	event = button.event;//Function to play when the button is clicked
	text.color = button.textColor;//Color of text

	normalColor = Color{ 150, 150, 150, 100 };
	hoverColor = Color{ 130, 130, 130, 100 };

	Start();//Starts the game object
}
Button::Button(string newName, ButtonBounds button, Sprite spriteValues) {
	name = newName;//Names the game object
	text.text = button.text; // Sets the display text
	position = button.position;//Sets the position of the title
	buttonSize = button.size;//Sets the size of the rectangle
	event = button.event;//Function to play when the button is clicked
	text.color = button.textColor;//Color of text
	spriteValues = spriteValues;

	normalColor = WHITE;
	hoverColor = Color{ 200, 200, 200, 255 };

	drawTexture = true;
	buttonSprite = new ImageUI(name + "ButtonBG", spriteValues);//Creates new button
	//Sets the size of the button
	buttonSprite->imageSize.x = buttonSize.x;
	buttonSprite->imageSize.y = buttonSize.y;

	Start();//Starts the game object
}
void Button::Start() {
	GameObject::Start();//Base start
	classification = UI;//Sets the object type
	//Sets a rectangle with all the values
	buttonBounds = { position.x,position.y,buttonSize.x,buttonSize.y };	

	//Sets the font size to fit inside the button
	text.position = position;
	text.fontSize = (buttonSize.x / text.text.length()) + (buttonSize.y/ text.text.length());
	buttonOffset = { buttonBounds.width / 2, buttonBounds.height / 2 };//Sets an offset for the button to be centered

	selectColor = GRAY;

	buttonText = new Title(name + "Button Text", text);//Creates new text to add on to the button
	

}
void Button::Update() {
	GameObject::Update();//Base Update
	mousePosition = GetMousePosition();

	ClickCheck();

}
void Button::Draw() {
	//Checks if the mouse if hovering and playsa sound
	if (isHovering != HoverCheck() && HoverCheck()) { 
		PlaySound(Game::GetInstance()->sounds["MouseHover"]); 
	};
	isHovering = HoverCheck();
	
	string stupidHead = name;
	stupidHead;

	//Changes the color if hovered over
	buttonColor = (isHovering) ? hoverColor : (!selected) ? normalColor : selectColor;//Deteremines the color of the button

	DrawButtonRec();
	DrawButtonTexture();
}
/// <summary>
/// Checks if the mouse if over the button
/// </summary>
/// <returns>If the mouse is over the button</returns>
bool Button::HoverCheck() {

	//Checks if the mouse is within the bounds of the button
	bool xPosCol = mousePosition.x >= buttonBounds.x - buttonOffset.x && mousePosition.x <= buttonBounds.x + buttonBounds.width/2;
	bool yPosCol = mousePosition.y >= buttonBounds.y - buttonOffset.y && mousePosition.y <= buttonBounds.y + buttonBounds.height/2;

	return xPosCol && yPosCol;
}
/// <summary>
/// Checks if the button was clicked on
/// </summary>
void Button::ClickCheck() {
	if (IsMouseButtonPressed(0) && HoverCheck()) {
		UIManager::GetInstance()->clicked = true;
		selected = true;
		for (GameObject* obj : Game::GetInstance()->objects) {
			if (typeid(*obj) == typeid(Button) && ((Button*)obj)->selected && !(obj == this)) {
				((Button*)obj)->selected = false;
			}
		}
		event();//Plays the event
		
	}
}
/// <summary>
/// Draws the button as a rectangle
/// </summary>
void Button::DrawButtonRec() {
	if (drawTexture) { return; }	

	//Draws the button with a center
	DrawRectanglePro(buttonBounds, { buttonOffset.x,buttonOffset.y }, 0, buttonColor);
}
/// <summary>
/// Draws the button as a texture
/// </summary>
void Button::DrawButtonTexture() {
	if (!drawTexture) { return; }
	buttonSprite->tint = buttonColor;//Changes the button color

}