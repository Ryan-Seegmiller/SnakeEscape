#pragma once
#include <vector>
#include "GameObject.h"
#include <map>
#include "Player.h"
#include "Collectible.h"
#include "Button.h"
#include "Title.h"
#include "ImageUI.h"



//GameStates
enum GameStates {
	PLAY,
	PAUSE,
	END,
	MENU,
	SETTINGS
};
enum Level {
	EASY,
	MEDIUM,
	HARD
};
class Game
{
protected:
	
	static Game* instance;

public:
	
	//Player stats struct
	struct PlayerValues {
		int maxLength;
		int score;
		int length;
		vector<Vector2> tilePath;
		vector<PowerUp> powerUps;
		int scoreMultipler;
	}playerStats;

	struct DifficultySettings {
		float scoreMulitplier;
		int speed;
		int powerUpAmount;
		int wallHealth;
	}difficulty;

	Level level;
	
	void AddScore(int);//Adds Score
	void ResetPlayerValues();


	//Setup
	static Game* GetInstance();//Gets the singleton of the game
	void AddGameObject(GameObject*);//Adds a game object to the list
	void UpdateGameObjects();//Updates the gameObjects
	void Start();//Sets up the scene
	void StartPlay();//Resets the game
	void EndPlay();//Ends the game
	void MenuControls();//Controls in menu
	void EraseGameObjects();//Erases the game object from the list
	void GetTexture(string, string);//Loads a new texture
	void GetFont(string, string);//Loads a new Font
	void GetDirectoryTextures(string, string, int);
	void GetSound(string, string);
	void GarbbageCollection();
	void AddObjects();

	//Take Down
	void Unload();

	//Grid
	void GridSetup();//Sets up the grid
	void DrawGrid();//Draws the grid every frame
	int GRID_TILE_SIZE;//Determines the grid size

	//Grid variables
	int screenHeight;//Screen Height
	int screenWidth;//Screen Width
	int gridTileLength;
	int gridTileHeigth;
	map<pair<int, int>, Vector2> gridTiles;//Sets the key and value pair for each grid value


	//Collectible
	void CreateCollectible();
	void CreatePowerUp(Vector2);
	void ResetPlayerPowerUps();
	void AddPowerUp(PowerUp);
	void DeactivatePowerUp(PowerUp*);
	void PowerUpTimer();

	#pragma region UI

	bool windowShouldClose;


	//Settings UI 
	
	void EasyMode();

	
	void MediumMode();

	
	void HardMode();


	

	#pragma endregion

	//Essentials
	map<string, Texture2D> gameTextures;//All textures
	vector<GameObject*> objects;//List of game objects to update
	vector<GameObject*> garbage;//Things to delete after update
	vector<GameObject*> toBeAdded;//Things to be added
	map<string, Font> fonts;//Map of fonts
	map<string, Sound> sounds;//Map of sounds


	
	GameStates currentState;//Curent state the game is in

	private:
	//Border
	void CalcBorder();//Calculates the border for the 
	bool CalcBorderX(GameObject*);
	bool CalcBorderY(GameObject*);
	Vector2 border[4];

	//Wall
	void RandomWallPlacements();//Places the walls in random locations
	pair<Vector2, Vector2> RandomWallSection(Vector2, Vector2);
	Vector2 RandomNumberOverlapAdjuster(Vector2, Vector2, Vector2, Vector2);//Makes sure no walls overlap with each other
	Vector2 CollectiblePLacement();//Places the collectible
	void FixedWallPlacement();//The wall border
	void WallSectionGenerator(Vector2, Vector2, int);//Genertates waql sections
	void CreateWall(Vector2);//Creates a wall

	int wallBorderSize;
	int wallBorderSizeX;
	int wallBorderSizeY;
	int playSpaceBorder;

	Vector2 xClampFirstColumn;
	Vector2 xClampSecondColumn;
	Vector2 yClampFirstRow;
	Vector2 yClampSecondRow;

	Image wallImage;//Wall image
};
