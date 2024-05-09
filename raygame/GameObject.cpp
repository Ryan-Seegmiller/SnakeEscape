#include "GameObject.h"
#include <iostream>
#include "Game.h"

/// <summary>
/// Does nothing
/// </summary>
GameObject::GameObject(){}
/// <summary>
/// Base GameObject constructor
/// </summary>
/// <param name="newName"></param>
GameObject::GameObject(string newName) {
	name = newName;//Base Name
	Start();//Base Start
}
/// <summary>
/// Destructor
/// </summary>
GameObject::~GameObject() {}
/// <summary>
/// Initiates values for the game object
/// </summary>
void GameObject::Start() {
	tilePosition = position;//Sets the tile position
	ResetTileSize();//Sets the tile size
	gameList = Game::GetInstance()->objects;//Initializes the game list and adds the object to it
	Game::GetInstance()->AddGameObject(this);//Adds the game object to the list
	glowSpeed = 0;
}
/// <summary>
/// A function that is called every frame
/// </summary>
void GameObject::Update() {
	Glowing();
	Draw();//Draws the objects to the screen
	ResetObjectList();//Makes sure that the lists are up to date
}
/// <summary>
/// A function that is called every frame to draw the object onto the screen
/// </summary>
void GameObject::Draw() {}

/// <summary>
/// Destroys te game object
/// </summary>
void GameObject::Destroy() {
	
	GameObject* thisptr = this;//Sets a pointer to the game object

	auto iter = Game::GetInstance()->objects.begin();//Sets an iterator
	while( iter != Game::GetInstance()->objects.end()) {
		//if the game object is equal to this delete it
		if ((*iter) == thisptr) {
			Game::GetInstance()->objects.erase(iter);//Removes it from the list
			cout << "Destroyed: " << name << endl;//Logs that the oject was destroyed
			break;//Breaks from the loop
		}
		iter++;
	}
	delete this;
}
/// <summary>
/// Base death
/// </summary>
void GameObject::Death() {
	if (!markedForDeletion) {
		Game::GetInstance()->garbage.push_back(this);
		markedForDeletion = true;
	}
}
/// <summary>
/// Gets the new tile size for the grid 
/// </summary>
void GameObject::ResetTileSize() {
	tileSize = Game::GetInstance()->GRID_TILE_SIZE;
}
/// <summary>
/// Checks if the object is out of bounds
/// </summary>
/// <returns></returns>
bool GameObject::OutsideBoundsCheck() {
	return (position.x + tileSize / 1.5 > Game::GetInstance()->screenWidth - (tileSize*2) || position.x < (tileSize * 2)/1.2 || position.y < (tileSize * 2)/1.2 || position.y+tileSize/1.5 > Game::GetInstance()->screenHeight - (tileSize * 2));
}
/// <summary>
/// Checks if the object is colliding with another object
/// </summary>
/// <param name="objPos"></param>
/// <returns></returns>
bool GameObject::Colliding(Vector2 objPos) {
	return (position.x + tileSize / 1.5f >= objPos.x && position.x <= objPos.x + tileSize / 1.5f) &&
		(position.y + tileSize / 1.5f >= objPos.y && position.y <= objPos.y + tileSize / 1.5f);
}
/// <summary>
/// Removes health from the object
/// </summary>
/// <param name="negativeHP"></param>
void GameObject::RemoveHealth(int negativeHP) {
	health -= negativeHP;//Removes health

	//Makes sure the object sill has health
	if (health <= 0) {
		Death();//Calls the death function
	}
}
/// <summary>
/// Resets the game object list so it accurate 
/// </summary>
void GameObject::ResetObjectList() {
	//If the list if the same return
	if (gameList == Game::GetInstance()->objects) { return; }
	gameList = Game::GetInstance()->objects;//Reset the list
}
void GameObject::Glowing() {
	if (!canGlow) { return; }

	Color rgbMaxColor = {129,0,159,100};

	Color rgbMinColor = { 149,108,159,100 };

	if (glowSpeed == 0) {
		tint = rgbMaxColor;
		 glowSpeed = 1;
	}
	Color colorToGoTo = (reverseGlow)? rgbMaxColor : rgbMinColor;
	currentLerpingTime += .9f * glowSpeed * GetFrameTime();
	int r = Lerp(tint.r,colorToGoTo.r, currentLerpingTime);
	int g = Lerp(tint.g,colorToGoTo.g, currentLerpingTime);
	int b = Lerp(tint.b,colorToGoTo.b, currentLerpingTime);

	tint = { (unsigned char)r,(unsigned char)g,(unsigned char)b,255 };

	//Vector3 newHsv = Vector3Lerp(ColorToHSV(tint), colorToGoTo, .9f * glowSpeed * GetFrameTime());

	if (currentLerpingTime >= glowSpeed) {
		reverseGlow = !reverseGlow;
		currentLerpingTime = 0;
	}

	//tint = ColorFromHSV(newHsv.x,newHsv.y,newHsv.z);
}



