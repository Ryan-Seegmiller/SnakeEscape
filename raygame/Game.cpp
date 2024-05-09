#include "Game.h"
#include "Wall.h"
#include <iostream>
#include "raylib.h"
#include <functional>
#include <map>
#include "UIManager.h"



#pragma region Singleton
Game* Game::instance = NULL;

/// <summary>
/// Gets the single instance of game
/// </summary>
/// <returns>Returns the single instance of the game class</returns>
Game* Game::GetInstance() {
	if (instance == NULL) {
		instance = new Game();
	}
	return instance;
}
#pragma endregion

#pragma region Game Setup

/// <summary>
/// Setup for the game
/// </summary>
void Game::Start() {
	//Sets the grid pixel size
	GRID_TILE_SIZE = 32;

	GetDirectoryTextures("WallTexture", "Resources//Wall//Wall0",10);//Gets the wall textures
	GetTexture("Logo", "Resources//Logo.png");//Gets the logo texture
	GetDirectoryTextures("ButtonTexture", "Resources//Buttons//Buttons", 3);//Gets the button textures
	GetDirectoryTextures("SnakeTexture", "Resources//Snake//Snake", 3);//Gets the Snake textures
	GetDirectoryTextures("CollectiblesTexture", "Resources//Collectibles//Collectible", 1);//Gets the collectible textures
	GetDirectoryTextures("ExplosionTexture", "Resources//Explosion//Explosion", 7);
	GetTexture("ExplosionIconTexture", "Resources//Explosion//ExplosionIcon.png");
	GetTexture("SnakeDoubleFire", "Resources//Snake//SnakeDoubleFire.png");
	GetTexture("SnakeDoubleFireBatteringRam", "Resources//Snake//BatteringRamDoubleFire.png");
	GetTexture("SnakeBatteringRam", "Resources//Snake//BatteringRamPlayer.png");
	GetTexture("BatteringRam", "Resources//Collectibles//BatteringRam.png");
	GetTexture("DoubleFireIcon", "Resources//Collectibles//DoubleFire.png");
	GetTexture("FreezeIcon", "Resources//Collectibles//Freeze.png");
	GetTexture("SpeedIcon", "Resources//Collectibles//Speed.png");
	GetTexture("wallMystery", "Resources//Wall//wallMysterey.png");
	GetTexture("GameOver", "Resources//GameOver.png");//Gets the game over texture
	GetFont("Main", "Resources//Fonts//GrinchedRegular.otf");//Gets the font
	GetSound("Shoot", "Resources//Sounds//LaserShoot.wav");
	GetSound("WallHit", "Resources//Sounds//HitWall.wav");
	GetSound("Death", "Resources//Sounds//Death.wav");
	GetSound("Collectible", "Resources//Sounds//Pickup.wav");
	GetSound("MouseHover", "Resources//Sounds//MouseHover.wav");
	GetSound("WallDeath", "Resources//Sounds//Crumble.wav");
	GetSound("Turn", "Resources//Sounds//Turn.wav");
	GetSound("Freeze", "Resources//Sounds//freeze.wav");
	GetSound("SpeedBoost", "Resources//Sounds//SpeedBoost.wav");
	GetSound("WallRam", "Resources//Sounds//WallRam.wav");
	GetSound("ExplosiveRounds", "Resources//Sounds//ExplosiveRounds.wav");
	GetSound("Explosion", "Resources//Sounds//Explosion.wav");
	//TODO: Make sounds for the double fire shot/pickup, slowmotion activate, speedboost activate

	//Window
	screenHeight = GetScreenHeight();//Screen height
	screenWidth = GetScreenWidth();//Screen width
	gridTileLength = (screenWidth / GRID_TILE_SIZE) - 5;
	gridTileHeigth = (screenHeight / GRID_TILE_SIZE) - 5;

	//Walls
	wallBorderSize = 3;
	wallBorderSizeX = 3;
	wallBorderSizeY = 2;
	playSpaceBorder = 2;
	
	MediumMode();

	GridSetup();//Sets up the grid
	
	currentState = MENU;//Sets the current state of the game to menu
	

}
/// <summary>
/// Gets a texture and adds it to a list
/// </summary>
/// <param name="name"></param>
/// <param name="path"></param>
void Game::GetTexture(string name, string path) {
	//Loads the image
	Image newImage = LoadImage(path.c_str());//Converts the the string to const char
	Texture2D newTexture = LoadTextureFromImage(newImage);//Loads texture into a texture for the wall texture
	UnloadImage(newImage);//Unloads the image

	//Adds the texture to the "map"(Dictionary)
	gameTextures.insert(pair<string, Texture2D>(name, newTexture));
}
/// <summary>
/// Gets the textures form a directory based on how many (int)there are
/// </summary>
/// <param name="name"></param>
/// <param name="path"></param>
/// <param name="length"></param>
void Game::GetDirectoryTextures(string name, string path, int length) {
	for (int i = 0; i <= length; i++) {
		//Gets the file path based on the loop iteration
		string newPath = path + to_string(i) + ".png";

		GetTexture(name + to_string(i), newPath);
	};
}
void Game::GetFont(string name, string path) {
	Font newFont = LoadFontEx(path.c_str(), 84, 0,250);//Loads the new font

	fonts.insert(pair<string, Font>(name, newFont));//Adds the font
}
void Game::GetSound(string name, string path) {
	Sound newSound = LoadSound(path.c_str());

	sounds.insert(pair<string, Sound>(name, newSound));
}
#pragma endregion


void Game::UpdateGameObjects() {
	MenuControls();
	DrawGrid();//Draws the grid
	CalcBorder();
	if (IsKeyReleased(KEY_F11)) {
		ToggleFullscreen();
	}
	
	for (GameObject* obj : objects) {
		GameStates state = currentState;
		obj->Update();//Calls update for all the game objects
		
		
		if (state != currentState) {
			break;
		}
	}
	if (currentState == PLAY) {
		UIManager::GetInstance()->UpdatePlayUI();
		PowerUpTimer();
	}
	GarbbageCollection();
	AddObjects();
	UIManager::GetInstance()->clicked = false;

}
void Game::GarbbageCollection() {
	for (GameObject* obj : garbage) {
		obj->Destroy();
	}
	garbage.clear();
}
/// <summary>
/// Hot controls for the menu
/// </summary>
void Game::MenuControls() {
	
	if (IsKeyPressed(KEY_ENTER) && currentState == MENU) {
		StartPlay();
		PlaySound(sounds["MouseHover"]);
	}
	if (IsKeyPressed(KEY_ENTER) && currentState == END) {
		UIManager::GetInstance()->CreateStartMenu();
		PlaySound(sounds["MouseHover"]);
	}
	if (IsKeyPressed(KEY_R) && currentState == END) {
		StartPlay();
		PlaySound(sounds["MouseHover"]);
	}
	if (IsKeyPressed(KEY_ENTER) && currentState == SETTINGS) {
		UIManager::GetInstance()->CreateStartMenu();
		PlaySound(sounds["MouseHover"]);
	}
}

#pragma region Grid
/// <summary>
/// Sets up the tile Grid
/// </summary>
void Game::GridSetup() {
	//Iterates through the rows determined by the screen heigth
	for (int i = GRID_TILE_SIZE * playSpaceBorder; i <= screenHeight - GRID_TILE_SIZE * 3; i += GRID_TILE_SIZE) {
		//Sets the row number
		int row = (i / GRID_TILE_SIZE)-2;
		//Iterates through the columns determined by the screen width
		for (int j = GRID_TILE_SIZE * playSpaceBorder; j <= screenWidth - GRID_TILE_SIZE * 3; j += GRID_TILE_SIZE) {
			int column = (j / GRID_TILE_SIZE) - 2;
			//Sets the value for the grid square
			pair<int, int> newSquare = { row, column };

			//Sets the position of the grid square
			Vector2 squarePos = { j, i };

			//Adds the value and position to the map
			gridTiles.insert(pair<pair<int, int>, Vector2>(newSquare, squarePos));
		}
	}
	return;
}
/// <summary>
/// Draws the grid
/// </summary>
void Game::DrawGrid() {
	//Caches color to be used
	Color groundColor;

	//Sets the iterator to the grid
	map<pair<int, int>, Vector2>::iterator it = gridTiles.begin();

	//Iterates through the grid and determins the color of the rectangles for the floor
	while (it != gridTiles.end()) {

		//Caches the grid position for each square
		int yGridPos = it->first.second;
		int xGridPos = it->first.first;

		//Alternates the colors based on where the postion of the square is
		Color otherGreen = Color{ 0, 200, 40, 255 };
		groundColor = ((yGridPos % 2 == 0 && xGridPos % 2 == 0) || (yGridPos % 1 == 0 && !(yGridPos % 2 == 0)) && xGridPos % 1 == 0 && !(xGridPos % 2 == 0)) ? otherGreen : GREEN;
		
		DrawRectangle(it->second.x, it->second.y, GRID_TILE_SIZE, GRID_TILE_SIZE, groundColor);
		it++;
	}
}
#pragma endregion

#pragma region Randomly Generated Objects

pair<Vector2, Vector2> Game::RandomWallSection(Vector2 xClamp, Vector2 yClamp) {
	//Gets the random wall section
	Vector2 sections[] = { xClampFirstColumn, xClampSecondColumn, yClampFirstRow, yClampSecondRow };
	int randomSection = GetRandomValue(0, 3);
	if (randomSection > 1) {
		return pair<Vector2, Vector2>(xClamp, sections[randomSection]);
	}
	else {
		return pair<Vector2, Vector2>(sections[randomSection], yClamp);
	}
}
/// <summary>
/// Places a certain number of walls based off around a threshold
/// </summary>
void Game::RandomWallPlacements() {
	//Determines the wall amount
	
	Vector2 xClamp = { (float)wallBorderSizeX,(float)gridTileLength - wallBorderSizeX };
	Vector2 yClamp = { (float)wallBorderSizeY,(float)gridTileHeigth - wallBorderSizeY };

	int 	wallArea = ((xClamp.y - xClamp.x) * wallBorderSizeX) + ((yClamp.y - yClamp.x) * wallBorderSizeY);

	int wallAmount = GetRandomValue(wallArea - wallArea/10 ,wallArea);

	WallSectionGenerator(xClamp, yClamp, wallAmount);

	xClamp = { (float)wallBorderSizeX * 2,(float)gridTileLength - wallBorderSizeX *2};
	yClamp = { (float)wallBorderSizeY * 2,(float)gridTileHeigth - wallBorderSizeY *2 };

	wallArea = ((xClamp.y - xClamp.x) * wallBorderSizeX) + ((yClamp.y - yClamp.x) * wallBorderSizeY);
	
	wallAmount = GetRandomValue(wallArea/3, wallArea);

	WallSectionGenerator(xClamp, yClamp, wallAmount);

	border[0] = { xClamp.x + wallBorderSizeX + 1, yClamp.x + wallBorderSizeX  + 1};
	border[1] = { xClamp.y - wallBorderSizeY - 1, yClamp.x + wallBorderSize + 1};
	border[2] = { xClamp.x + wallBorderSizeX + 1, yClamp.y - wallBorderSizeY - 1};
	border[3] = { xClamp.y - wallBorderSizeY - 1, yClamp.y - wallBorderSizeY - 1};
}
void Game::WallSectionGenerator(Vector2 xClamp, Vector2 yClamp, int wallAmount) {
	 xClampFirstColumn = { xClamp.x, xClamp.x + wallBorderSizeX - 1 };
	 xClampSecondColumn = { xClamp.y, xClamp.y - wallBorderSizeY + 1 };
	 yClampFirstRow = { yClamp.x, yClamp.x + wallBorderSizeX - 1 };
	 yClampSecondRow = { yClamp.y, yClamp.y - wallBorderSizeY + 1 };

	 //Iterates through the amount of wall needed
	for (int i = 0; i < wallAmount; i++) {
		//Creates a new wall in the section selected
		pair<Vector2, Vector2> section = RandomWallSection(xClamp, yClamp);
		Vector2 randomWallPostionGridValue = RandomNumberOverlapAdjuster(section.first, section.second, xClamp, yClamp);

		CreateWall(randomWallPostionGridValue);
	}
}
/// <summary>
/// Sets the border walls
/// </summary>
void Game::FixedWallPlacement() {
	for (int i = 0; i < wallBorderSize; i++) {
		for (int j = 0; j < gridTileLength + 1; j++) {
			Vector2 wallGridPos = {j,i};
			CreateWall(wallGridPos);
		}
	}
	for (int i = 0; i < wallBorderSize; i++) {
		for (int j = 0; j < gridTileLength + 1; j++) {
			Vector2 wallGridPos = { j,gridTileHeigth-i };
			CreateWall(wallGridPos);
		}
	}
	for (int i = 0; i < gridTileHeigth - wallBorderSize + 1; i++) {
		for (int j = 0; j < wallBorderSize; j++) {
			Vector2 wallGridPos = { j,i };
			CreateWall(wallGridPos);
		}
	}
	for (int i = 0; i < gridTileHeigth - wallBorderSize + 1; i++) {
		for (int j = 0; j < wallBorderSize; j++) {
			Vector2 wallGridPos = { gridTileLength-j,i };
			CreateWall(wallGridPos);
		}
	}
}
void Game::CreateWall(Vector2 wallGridValue) {
	//Gets the position based on the grid value
	Vector2 wallPosition = gridTiles[pair<int, int>(wallGridValue.y, wallGridValue.x)];

	//Creates the wall
	bool powerUpWall = (GetRandomValue(0, 1000) % 500 <= difficulty.powerUpAmount);
	Wall* newWall = new Wall("Wall", wallPosition, powerUpWall);
}
/// <summary>
/// Makes sure there is no oject already in that tile spot
/// </summary>
/// <param name="maxX"></param>
/// <param name="maxY"></param>
/// <returns></returns>
Vector2 Game::RandomNumberOverlapAdjuster(Vector2 xClamp, Vector2 yClamp, Vector2 wallXClamp, Vector2 wallYClamp) {
	//Caches vectors for use
	vector<int> xValues;
	vector<int> yValues;

	//Sets all the possible x and y values to be checked
	for (GameObject* obj : toBeAdded) {
		xValues.push_back((obj->tilePosition.x/ GRID_TILE_SIZE)-2);
		yValues.push_back((obj->tilePosition.y/ GRID_TILE_SIZE)-2);
	}

	//Selects a random number
	int randX = GetRandomValue(xClamp.x, xClamp.y);
	int randY = GetRandomValue(yClamp.x, yClamp.y);

	//Makes sure that there is not an object already there
	for (int i = 0; i < xValues.size(); i++) {
		if (xValues[i] == randX && yValues[i] == randY) {
			pair<Vector2, Vector2> section = RandomWallSection(wallXClamp, wallYClamp);
			xClamp = section.first;
			yClamp = section.second;
			randX = GetRandomValue(xClamp.x, xClamp.y);
			randY = GetRandomValue(yClamp.x, yClamp.y);
			i = 0;
		}
	}
	//Sets the walls new tile position
	Vector2 WallTilePosition = { randX, randY };

	return WallTilePosition;

}

//Methods that handle border placement
#pragma region Border
/// <summary>
/// Clculates the border of the collectible spawns
/// </summary>
void Game::CalcBorder() {
	//Bool to determine wether the border should be moved or not
	bool moveXBorder = false;
	bool moveYBorder = false;

	for (GameObject* obj : objects){ 
		if (obj->classification != WALL) { continue; }
		if (!moveXBorder) {
			moveXBorder = CalcBorderX(obj);
		}
		if (!moveYBorder) {
			moveYBorder = CalcBorderY(obj);
		}
	}
	if (!moveXBorder) {
		if (border[0].x < 0) { return; }//Makes sure if dosent go past the end
		//Changes the border of the collectibles to spawn in
		border[0].x = border[0].x - 1;
		border[1].x = border[1].x + 1;
		border[2].x = border[2].x - 1;
		border[3].x = border[3].x + 1;
	}
	if (!moveYBorder) {
		if (border[0].y < 0) { return; }//Makes sure if dosent go past the end
		//Changes the border of the collectibles to spawn in
		border[0].y = border[0].y - 1;
		border[1].y = border[1].y - 1;
		border[2].y = border[2].y + 1;
		border[3].y = border[3].y + 1;
	}
}
/// <summary>
/// Detects if the border has changed on the x value
/// </summary>
/// <param name="obj"></param>
bool Game::CalcBorderX(GameObject* obj) {
	int objX = (obj->tilePosition.x / GRID_TILE_SIZE) - 2;
	int objY = (obj->tilePosition.y / GRID_TILE_SIZE) - 2;

	if (objX >= border[0].x && objX <= border[1].x && (objY == border[0].y || objY == border[2].y)) {
		return true;
	}

	return false;
}

/// <summary>
/// Detects of the border has changed on the y value
/// </summary>
/// <param name="obj"></param>
bool Game::CalcBorderY(GameObject* obj) {
	int objX = (obj->tilePosition.x / GRID_TILE_SIZE) - 2;
	int objY = (obj->tilePosition.y / GRID_TILE_SIZE) - 2;

	if (objY >= border[1].y && objY <= border[2].y && (objY == border[0].x || objY == border[1].x)) {
		return true;
	}

	return false;
}
#pragma endregion

#pragma endregion

#pragma region Collectibles

void Game::CreateCollectible() {
	//Makes sure the collectible doesnt spawn on top of any walls
	Vector2 CollectiblePositionGridValue = CollectiblePLacement();

	int yValue = CollectiblePositionGridValue.y;
	int xValue = CollectiblePositionGridValue.x;

	//Gets the position based on the grid value
	Vector2 collectiblePosition = gridTiles[pair<int, int>(yValue, xValue)];

	//Creates the Collectible
	Collectible* newCollectible = new Collectible("Apple", collectiblePosition);

	//cout << "Apple created" << endl;//Debug to make sure its been created
}
void Game::CreatePowerUp(Vector2 pos) {
	

	//Gets the position based on the grid value
	Vector2 collectiblePosition = pos;
	int randomPowerUp = GetRandomValue(1, COLLECTIBLE_END-1);

	//Creates the Collectible
	Collectible* newCollectible = new Collectible("Apple", collectiblePosition, (CollectibleType)randomPowerUp);

}
Vector2 Game::CollectiblePLacement() {
	
	float xValue = GetRandomValue(border[0].x + 1, border[1].x - 1);
	float yValue = GetRandomValue(border[1].y + 1, border[2].y - 1);
	
	auto vec = playerStats.tilePath.begin();
	while (vec != playerStats.tilePath.end()) {
		if (xValue == vec->x-2 && yValue == vec->y-2) {
			xValue = GetRandomValue(border[0].x + 1, border[1].x - 1);
			yValue = GetRandomValue(border[1].y + 1, border[2].y - 1);
			vec = playerStats.tilePath.begin();
		}
		vec++;
	}

	return Vector2{xValue, yValue};
}
#pragma endregion



#pragma region Play States

/// <summary>
/// Starts the game
/// </summary>
void Game::StartPlay() {
	currentState = PLAY;//Sets is playing to true
	EraseGameObjects();//Erases all the game objects before creating new ones
	ResetPlayerValues();//Resets the player values for the new game
	
	FixedWallPlacement();
	RandomWallPlacements();//Creates the walls
	CreateCollectible();//Creates a collectible

	//Gets the center's tile 
	Vector2 CenterOfScreen = { ((screenWidth / GRID_TILE_SIZE)/2) * GRID_TILE_SIZE, ((screenHeight / GRID_TILE_SIZE)/2)*GRID_TILE_SIZE };
	Player* snake = new Player("Snake", CenterOfScreen);//Creates the player in the center of the screen
	UIManager::GetInstance()->CreatePlayUI();//creates the UI
	
}
/// <summary>
/// Ends the game and shows the end screen
/// </summary>
void Game::EndPlay() {
	EraseGameObjects();//Clears the game objects of the screen
	currentState = END;//Sets play mode to false
	UIManager::GetInstance()->CreateEndMenu();
}
#pragma endregion

#pragma region GameObjects
/// <summary>
/// Clears the game object list 
/// </summary>
void Game::EraseGameObjects() {
	//Deconstructs all the game objects
	auto iter = objects.begin();
	while (iter != objects.end()) {
		cout << "Destroyed " << (*iter)->name << endl;
		delete (*iter);
		iter++;
	}
	//Clears the list
	objects.clear();
}
/// <summary>
/// Adds the game object to the list
/// </summary>
/// <param name="newObject"></param>
void Game::AddGameObject(GameObject* newObject) {
	toBeAdded.push_back(newObject);//Adds to the list
}
void Game::AddObjects() {
	for (GameObject* obj : toBeAdded) {
		objects.push_back(obj);
		cout << "Added(\"" << obj->name << "\") to the Objects list" << endl;//Prints a log signifying that it was done
	}
	toBeAdded.clear();
	
}
#pragma endregion

#pragma region UI

void Game::EasyMode()
{
	level = EASY;
	difficulty = {
		.25,
		150,
		200,
		25
	};
}
void Game::MediumMode()
{
	level = MEDIUM;
	difficulty = {
		1,
		300,
		100,
		100
	};
}
void Game::HardMode()
{
	level = HARD;
	difficulty = {
		5,
		500,
		50,
		300
	};
}
#pragma endregion


#pragma endregion

#pragma region Player stat methods
/// <summary>
/// Adds score
/// </summary>
/// <param name="scoreToAdd"></param>
void Game::AddScore(int scoreToAdd) {
	playerStats.score += scoreToAdd * difficulty.scoreMulitplier;
}
/// <summary>
/// Resets the values back to 0
/// </summary>
void Game::ResetPlayerValues() {
	playerStats.score = 0;
	playerStats.maxLength = 0;
	playerStats.length = 0;
	playerStats.scoreMultipler = 1;
	playerStats.tilePath.push_back(Vector2Zero());
	ResetPlayerPowerUps();
}
void Game::ResetPlayerPowerUps() {
	playerStats.powerUps.clear();
	for (int i = 1; i < COLLECTIBLE_END; i++) {
		PowerUp newPowerUp = {
			(CollectibleType)i,
			false,
			false,
			0,
			0
		};

		playerStats.powerUps.push_back(newPowerUp);
	}
}

void Game::AddPowerUp(PowerUp powerUpToAdd) {
	for (int i = 0; i < playerStats.powerUps.size(); i++) {
		if (playerStats.powerUps[i].type == powerUpToAdd.type) {
			playerStats.powerUps[i].isActive = powerUpToAdd.isActive;
			playerStats.powerUps[i].duration += powerUpToAdd.duration;
			playerStats.powerUps[i].durationInt += powerUpToAdd.durationInt;
			playerStats.powerUps[i].uses += powerUpToAdd.uses;
			playerStats.powerUps[i].hasntActivated += powerUpToAdd.hasntActivated;
			return;
		}
	}
}
void Game::PowerUpTimer() {
	for (int i = 0; i < playerStats.powerUps.size(); i++) {
		if (playerStats.powerUps[i].durationInt <= 0 && playerStats.powerUps[i].isActive && !(playerStats.powerUps[i].uses > 0)) {
			DeactivatePowerUp(&playerStats.powerUps[i]);
		}
		else if(playerStats.powerUps[i].uses < 0){
			DeactivatePowerUp(&playerStats.powerUps[i]);
		}
		else if (playerStats.powerUps[i].hasntActivated) {
			continue;
		}
		else if(playerStats.powerUps[i].duration >= 0 && playerStats.powerUps[i].isActive){
			playerStats.powerUps[i].duration -= GetFrameTime();
			playerStats.powerUps[i].durationInt = (int)playerStats.powerUps[i].duration;

		}
	}
}
void Game::DeactivatePowerUp(PowerUp* powerUpToDeactivate) {
	powerUpToDeactivate->duration = 0;
	powerUpToDeactivate->isActive = false;
	powerUpToDeactivate->uses = 0;
	
}

#pragma endregion

void Game::Unload() {
	//inloads textures
	auto txtIter = gameTextures.begin();
	while (txtIter != gameTextures.end()) {
		UnloadTexture(txtIter->second);
		txtIter++;
	}
	//Unloads sounds
	auto soundIter = sounds.begin();
	while (soundIter != sounds.end()) {
		UnloadSound(soundIter->second);
		soundIter++;
	}
	//Unloads fonts
	auto fontIter = fonts.begin();
	while (fontIter != fonts.end()) {
		UnloadFont(fontIter->second);
		fontIter++;
	}
}