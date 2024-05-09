#pragma once
#include "GameObject.h"
#include <map>;
#include "Collectible.h"
#include "Title.h"
#include "Bar.h"

using namespace std;
//Struct for the tail pieces
struct Tail {
	Vector2 position;//Determines the position for each tail piece
	Vector2 direction;//Determines the direction for ecah tail piece
	Vector2 tileNum;//Determines the tile num they are all on
	Rectangle tailPiece;//Sets the rectangle for each one
	Texture2D snakeTexture;
};
enum Directions {
	RIGHT,
	LEFT,
	UP,
	DOWN,
	NONE
};

class Player : public GameObject
{
public:
	//Constructors
	Player(string, Vector2);

	//Movement variables
	Vector2 playerInput;//Determines the direction the player will be moving in
	map<Directions, bool> directions;//Map of directions the player can go and bool is the player has pressed on
	Directions turning[2];
	Vector2 directionForTurn[2];
	void AddTurning(Directions, Vector2);
	
	int baseSpeed;
	int speed;//Speed for the player
	Vector2 previousDirection;
	Vector2 previousTilePosition;
	Vector2 movePointTilePosition;
	bool canTurn;
	bool gridSpotOvveride;
	bool ChangedDuringTurn;


	//Base attributes
	void Draw();//Draws the player to the scene
	void Update();//Is called every frame
	void Death();
	void CollisonCheck();//Checks if the player collides
	void WallCollison(int);
	void GetSnakeTextures();
	Rectangle snakePiece;//Sets the width and heigh for the snake


	//Player Movement
	void Move();//Moves the player
	Tail TailMovement(int);//Moves the tail based on the trail the player leaves behind
	void PlayerControls();//Takes input from the player
	void ChangeDirections();//Changes directions the player is going
	float CalcRotation(Vector2);
	

	//Player movement helpers
	void RecordTileSpot(Vector2);//Records the tile spots the player leaves behind
	void SetTilePosition();//Sets the gridded tile position
	Vector2 nextDirection;

	//Player length
	void AddLength();//Adds length to the snake
	void RemoveLength();//Removes a part of the tail
	int snakeLength; // length of the snake
	vector<Tail> snake;//Sets the list for the tail pieces
	Tail newTail;// Base tail with nothing in it
	vector<Vector2> tilePath;//Tile path that the player leaves behind
	vector<Texture2D> snakeTextures;
	vector<Bar*> powerUpBars;
	void UpdatePowerUpBar();

	//Laser
	void Shoot();//Shoots a laser forward from the player

	//Player Stats
	void SetStats();
	int maxLength;
	int lengthScoreMultiplier;
	int baseScoreToAdd;
	int baseDamage;


	//Powerups
	bool isGlowing;

	bool doubleFire;
	void DoubleFire();
	void StartDoubleFire();
	

	bool wallBreaker;
	void WallBreaker();

	bool explosiveRounds;
	int expolsionDamage;
	void ExplosiveRounds();

	bool slowMotion;
	int slowMotionSpeed;
	void SlowMotion();

	bool speedBoost;
	int speedBoostSpeed;
	int speedBoostScoreMultiplier;
	void SpeedBooster();

	void SetPowerUp();
	void RemovePowerUp();



	bool boosted;
	int intervalblink;
	

};

