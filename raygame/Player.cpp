#include "Player.h"
#include <iostream>
#include "Game.h"
#include "Laser.h"
#include "Title.h"
#include <cassert>
#include "UIManager.h"


Player::Player(string newName, Vector2 pos) {

	playerInput = Vector2Zero();//Starts the player moving is a certain direction
	position = pos;//Starts the player position
	speed = Game::GetInstance()->difficulty.speed;//Sets the speed
	baseSpeed = speed;
	slowMotionSpeed = speed /2;
	speedBoostSpeed = speed * 2;
	name = newName;//Sets the name
	classification = PLAYER;//Sets the classification
	baseScoreToAdd = 50;//Sets the base score to add
	lengthScoreMultiplier = 10;//Sets the multiplier for length
	speedBoostScoreMultiplier = 1;
	baseDamage = 50;
	turning[0] = NONE;
	turning[1] = NONE;
	Game::GetInstance()->playerStats.scoreMultipler = 1;

	

	//Sets the directions the player can move in
	directions = {
		pair<Directions, bool>(RIGHT, false), 
		pair<Directions, bool>(LEFT, false),
		pair<Directions, bool>(UP, false),
		pair<Directions, bool>(DOWN, false),
	};
	
	//Sets the base snake piece
	snakePiece = { 0, 0, (float)Game::GetInstance()->GRID_TILE_SIZE, (float)Game::GetInstance()->GRID_TILE_SIZE };
	snakeLength = 2;	//Sets the base length of the tail

	//Makes sure the tail has a length
	for (int i = 0; i < snakeLength; i++) {
		tilePath.push_back(Vector2Zero());//Adds nothing into the grid 
	}
	GetSnakeTextures();
	newTail = { {0,0}, {0,0}, {0,0}, snakePiece, snakeTextures[0]};//Sets an empty tail
	snake.push_back(newTail);//Adds the first tail
	Start();//Starts the object
	tilePosition = Vector2{ tilePosition.x + tileSize / 2,tilePosition.y + tileSize / 2 };
	canGlow = true;
}
void Player::GetSnakeTextures() {
	snakeTextures.push_back(Game::GetInstance()->gameTextures["SnakeTexture0"]);
	snakeTextures.push_back(Game::GetInstance()->gameTextures["SnakeTexture1"]);
	snakeTextures.push_back(Game::GetInstance()->gameTextures["SnakeTexture2"]);
	snakeTextures.push_back(Game::GetInstance()->gameTextures["SnakeDoubleFire"]);
	snakeTextures.push_back(Game::GetInstance()->gameTextures["SnakeDoubleFireBatteringRam"]);
	snakeTextures.push_back(Game::GetInstance()->gameTextures["SnakeBatteringRam"]);
}
void Player::Draw() {
	int tailNum = 0;
	for (Tail snakePiece : snake) {
		if (tailNum++ >= tilePath.size()-1) { return; }

		Rectangle source = { 0,0,snakePiece.snakeTexture.width, snakePiece.snakeTexture.height };
		Vector2 origin = { tileSize/2,tileSize/2 };

		
		//Sets the tail piece position
		snakePiece.tailPiece.x = snakePiece.position.x;
		snakePiece.tailPiece.y = snakePiece.position.y;
		
		//Draws the tail piece
		//DrawRectanglePro(snakePiece.tailPiece, { 0,0 }, 0, RED);
		
		if (isGlowing) {
			DrawRectangle(snakePiece.tailPiece.x - origin.x, snakePiece.tailPiece.y - origin.y, tileSize, tileSize, Color{ tint.r,tint.g,tint.b,100 });
		}
		if (wallBreaker && tailNum == 1) {
			Texture2D BatteringRam = Game::GetInstance()->gameTextures["BatteringRam"];
			Rectangle batteringRamSource = {0,0,BatteringRam.width,BatteringRam.height };
			Rectangle batteringRamDest = { snakePiece.tailPiece.x + (tileSize/2 * playerInput.x) ,snakePiece.tailPiece.y + (tileSize/2 * playerInput.y),tileSize,tileSize };

			DrawTexturePro(BatteringRam, source, batteringRamDest, origin, CalcRotation(snakePiece.direction), WHITE);
		}
		DrawTexturePro(snakePiece.snakeTexture, source, snakePiece.tailPiece, origin, CalcRotation(snakePiece.direction), WHITE);
	}
	/*for (Vector2 vec : tilePath) {
		DrawRectangle(vec.x*tileSize, vec.y*tileSize, tileSize, tileSize, BLACK);
	}*/
}
void Player::Update() {
	GameObject::Update();//Base update
	UpdatePowerUpBar();
	SetPowerUp();
	RemovePowerUp();
	SetTilePosition();//Sets the position on the grid that the player is currently on
	PlayerControls();//Handles the controls
	Move();//Moves the player and the tail
	CollisonCheck();//Detects collision
	
	SetStats();//Sets the stats for the ui
	
}
#pragma region Movement
/// <summary>
/// Tracks the tiles the player has moved through depending on the length of the snake
/// </summary>
/// <param name="gridSpot"></param>
void Player::RecordTileSpot(Vector2 gridSpot) {
	//if (turning[0] != NONE && !gridSpotOvveride) { return; }
	//Makes sure the list is only as long as the snake
	if (tilePath.size() > snakeLength) {
		vector<Vector2> tempVec;
		//If not it will create a new list without the first element as that element is the last in the line
		for (int i = 1; i < tilePath.size(); i++) {
			tempVec.push_back(tilePath[i]);
		}
		tilePath = tempVec;//Sets the new list to be the new tile path
	}
	//Iterates through the list
	for (int i = 1; i < tilePath.size(); i++) {
		//Checks if the gridSpot is already in the list
		if (gridSpot.x == tilePath[i].x && gridSpot.y == tilePath[i].y) {
			return;//Return if true
		}
	}
	//Adds the current grid spot to the list
	tilePath.push_back(gridSpot); 
}
/// <summary>
/// Determines the movement the player needs
/// </summary>
void Player::Move() {
	if (markedForDeletion) { return; }

	assert(Vector2DotProduct(playerInput, nextDirection) != -1.0f && "Player is somehow attempting to move in the opposite direction!!");

	
	//Sets the speed based on the frame time
	int frameSpeed = GetFrameTime() * speed;
	//int frameSpeed = 1;
	

	for (int i = 0; i < tilePath.size() - 1; i++) {
		Tail updatedTailPiece;
		if (!(snake.size() - 1 >= i)) { break; }

		if (i == 0) {
			//Sets the position of the head of the tail relative to the frame speed
			position.x += playerInput.x * frameSpeed;
			position.y += playerInput.y * frameSpeed;



			//Sets the position of the head of the tail
			updatedTailPiece.position = Vector2Add(position, Vector2{(float)tileSize/2, (float)tileSize/2});
			updatedTailPiece.tileNum = gridSpot;//Sets the tile Num of the head of the tail
			updatedTailPiece.direction = playerInput;//Sets the direction of the head of the tail
			updatedTailPiece.tailPiece = snakePiece;//Sets the rectangle of the head of the tail
			if (doubleFire && wallBreaker) {
				updatedTailPiece.snakeTexture = snakeTextures[5];
			}
			else if (wallBreaker) {
				updatedTailPiece.snakeTexture = snakeTextures[4];
			}
			else if (doubleFire) {
				updatedTailPiece.snakeTexture = snakeTextures[3];
			}
			else{
				updatedTailPiece.snakeTexture = snakeTextures[0];
			}
		}
		else {
			updatedTailPiece = TailMovement(i);
		}
		//Sets the tail to the updated tail
		snake[i] = updatedTailPiece;
	}	
}
Tail Player::TailMovement(int i) {

	Tail updatedTailPiece;
	updatedTailPiece.snakeTexture = (i == snakeLength - 2) ? snakeTextures[2] : snakeTextures[1];
	
	

	i = tilePath.size() - i - 1;//Flips elements

	Vector2 direction = { tilePath[i + 1].x - tilePath[i].x, tilePath[i + 1].y - tilePath[i].y };

	
	Vector2 nextTilePos = Vector2Scale(Vector2Add(gridSpot, playerInput), tileSize);
	Vector2 actualPos = Vector2Add(position, Vector2Scale(Vector2One(), tileSize/2));

	float distanceToNextTile = Vector2Distance(actualPos, nextTilePos);


	Vector2 currentPosition = Vector2Scale(tilePath[i], tileSize);



	float tileDiffernce = (direction.x == -1 || direction.y == -1)? -tileSize/2: tileSize/2;

	currentPosition = Vector2Add(currentPosition, Vector2Scale(direction, tileSize));

	Vector2 offsetPosition = Vector2Subtract(currentPosition, Vector2Scale(direction, distanceToNextTile));
	
	updatedTailPiece.direction = direction;
	updatedTailPiece.position = offsetPosition;
	updatedTailPiece.tileNum = tilePath[i];//Sets the tile Num of the head of the tail
	updatedTailPiece.tailPiece = snakePiece;//Sets the rectangle of the head of the tail
	

	return updatedTailPiece;
}

float Player::CalcRotation(Vector2 direction) {
	//Sets the rotation to 90 if its moving on the x axis
	if (direction.x == -1) {
		return -90;
	}
	if (direction.x == 1) {
		return 90;
	}
	if (direction.y == 1) {
		return 180;
	}
	if (direction.y == -1) {
		return 0;
	}
	return 0;
}
/// <summary>
/// Reacts to player input
/// </summary>
void Player::PlayerControls() {
	//Input Check for the right Direction
	if ((IsKeyPressed(KEY_RIGHT)|| IsKeyPressed(KEY_D)) && ((turning[0] != NONE && nextDirection.x == 0) || turning[0] == NONE && playerInput.x == 0) && turning[0] != RIGHT) {
		directions[RIGHT] = true;
		
		AddTurning(RIGHT, Vector2{ 1,0 });
		if (Vector2Equals(Vector2Zero(), playerInput)) {
			playerInput = nextDirection;
			UIManager::GetInstance()->EndDrawTutorial();
		}
	}
	//Input Check for the left Direction
	if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && ((turning[0] != NONE && nextDirection.x == 0) || turning[0] == NONE && playerInput.x == 0) && turning[0] != LEFT) {
		directions[LEFT] = true;
		
		AddTurning(LEFT, Vector2{ -1,0 });
		if (Vector2Equals(Vector2Zero(), playerInput)) {
			playerInput = nextDirection;
			UIManager::GetInstance()->EndDrawTutorial();
		}
	}
	//Input Check for the down Direction
	if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && ((turning[0] != NONE && nextDirection.y == 0) || turning[0] == NONE && playerInput.y == 0) && turning[0] != DOWN) {
		cout << playerInput.y << endl;
		directions[DOWN] = true;
		
		AddTurning(DOWN, Vector2{ 0,1 });
		if (Vector2Equals(Vector2Zero(), playerInput)) {
			playerInput = nextDirection;
			UIManager::GetInstance()->EndDrawTutorial();
		}

	}
	//Input Check for the up Direction
	if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && ((turning[0] != NONE && nextDirection.y == 0) || turning[0] == NONE && playerInput.y == 0) && turning[0] != UP) {
		directions[UP] = true;
		
		AddTurning(UP, Vector2{ 0,-1 });

		if (Vector2Equals(Vector2Zero(), playerInput)) {
			playerInput = nextDirection;
			UIManager::GetInstance()->EndDrawTutorial();
		}

	}
	//Makes sure the snake has a tail
	if (IsKeyPressed(KEY_SPACE) && snakeLength > 2 && !markedForDeletion) {
		if (doubleFire) {
			DoubleFire();
		}
		else {
			Shoot();//Shoots a laser
		}
		
	}
	if (IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT)) {
		vector<PowerUp> powerUps = Game::GetInstance()->playerStats.powerUps;
		for (int i = 0; i < powerUps.size(); i++) {
			if (powerUps[i].type == SPEEDBOOST && powerUps[i].hasntActivated) {
				SpeedBooster();
				Game::GetInstance()->playerStats.powerUps[i].hasntActivated = false;
				Game::GetInstance()->playerStats.powerUps[i].isActive = true;
				Game::GetInstance()->playerStats.scoreMultipler = speedBoostScoreMultiplier;
				Bar* speedBoostBar = new Bar("WallBreakerBar", position, powerUps[i].durationInt, YELLOW, powerUps[i].type);

				powerUpBars.push_back(speedBoostBar);
				for (int j = 1; j < powerUps.size(); j++) {
					if (powerUps[j].type == SLOWMOTION && powerUps[j].isActive) {
						Game::GetInstance()->playerStats.powerUps[j].hasntActivated = true;
						Game::GetInstance()->playerStats.powerUps[j].isActive = false;
					}

				}
				break;
			}
			else if (powerUps[i].type == SPEEDBOOST && powerUps[i].isActive) {
				Game::GetInstance()->playerStats.powerUps[i].hasntActivated = true;
				Game::GetInstance()->playerStats.powerUps[i].isActive = false;
				Game::GetInstance()->playerStats.scoreMultipler = 1;


				speed = baseSpeed;
				break;
			}
			else if (powerUps[i].type == SPEEDBOOST && powerUps[i].isActive && powerUps[i].duration) {
				Game::GetInstance()->playerStats.powerUps[i].hasntActivated = true;
				Game::GetInstance()->playerStats.powerUps[i].isActive = false;
				Game::GetInstance()->playerStats.scoreMultipler = 1;
				speed = baseSpeed;
				speedBoost = false;
				break;
			}
			else if(powerUps[i].type == SPEEDBOOST && powerUps[i].isActive)
				for (int j = 0; j < powerUps.size(); j++) {
					if (powerUps[j].type == SLOWMOTION && powerUps[j].isActive && powerUps[j].duration >= 0) {
						Game::GetInstance()->playerStats.powerUps[j].hasntActivated = true;
						Game::GetInstance()->playerStats.powerUps[j].isActive = false;
						Game::GetInstance()->playerStats.scoreMultipler = 1;
						slowMotion = false;
					}
				}
		}
		
	}
	if (IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyPressed(KEY_RIGHT_CONTROL)) {
		vector<PowerUp> powerUps = Game::GetInstance()->playerStats.powerUps;
		for (int i = 0; i < powerUps.size(); i++) {
			if (powerUps[i].type == SLOWMOTION && powerUps[i].hasntActivated) {
				SlowMotion();
				Game::GetInstance()->playerStats.powerUps[i].hasntActivated = false;
				Game::GetInstance()->playerStats.powerUps[i].isActive = true;
				Bar* speedBoostBar = new Bar("WallBreakerBar", position, powerUps[i].durationInt, BLUE, powerUps[i].type);

				powerUpBars.push_back(speedBoostBar);
				break;
				for (int j = 1; j < powerUps.size(); j++) {
					if (powerUps[j].type == SPEEDBOOST && powerUps[j].isActive) {
						Game::GetInstance()->playerStats.powerUps[j].hasntActivated = true;
						Game::GetInstance()->playerStats.powerUps[j].isActive = false;
						Game::GetInstance()->playerStats.scoreMultipler = 1;
					}

				}
			}
			else if (powerUps[i].type == SLOWMOTION && powerUps[i].isActive) {
				Game::GetInstance()->playerStats.powerUps[i].hasntActivated = true;
				Game::GetInstance()->playerStats.powerUps[i].isActive = false;


				speed = baseSpeed;
				break;
			}
			else if (powerUps[i].type == SLOWMOTION && powerUps[i].isActive && powerUps[i].duration) {
				Game::GetInstance()->playerStats.powerUps[i].hasntActivated = true;
				Game::GetInstance()->playerStats.powerUps[i].isActive = false;
				speed = baseSpeed;
				slowMotion = false;
				break;
			}
			else if (powerUps[i].type == SLOWMOTION && powerUps[i].isActive)
				for (int j = 0; j < powerUps.size(); j++) {
					if (powerUps[j].type == SPEEDBOOST && powerUps[j].isActive && powerUps[j].duration >= 0) {
						Game::GetInstance()->playerStats.powerUps[j].hasntActivated = true;
						Game::GetInstance()->playerStats.powerUps[j].isActive = false;
						Game::GetInstance()->playerStats.scoreMultipler = 1;
						slowMotion = false;
					}
				}
			
		}
	}
	
}
void Player::AddTurning(Directions direction, Vector2 newDiretion) {
	if (turning[0] == NONE) {
		turning[0] = direction;
		nextDirection = newDiretion;
		directionForTurn[0] = newDiretion;
		
	}
	else if (turning[1] == NONE) {
		turning[1] = direction;
		directionForTurn[1] = newDiretion;
		
	}
}
/// <summary>
/// Checks if the player is ready to change directions and if so does accordingly
/// </summary>
void Player::ChangeDirections() {
	
	if (turning[0] == RIGHT) {
		position.y = tilePosition.y - tileSize/2;
		//Sets the direction the snake will move
		playerInput = nextDirection;
	 

		//Resets bool
		directions[RIGHT] = false;
		PlaySound(Game::GetInstance()->sounds["Turn"]);
		turning[0] = turning[1];
		directionForTurn[0] = directionForTurn[1];
		nextDirection = directionForTurn[1];
		directionForTurn[1] = Vector2Zero();
		
		turning[1] = NONE;

	}
	else if (turning[0] == LEFT) {
		
		position.y = tilePosition.y - tileSize / 2;
		//Sets the direction the snake will move
		playerInput = nextDirection;

		//Resets bool
		directions[LEFT] = false;
		PlaySound(Game::GetInstance()->sounds["Turn"]);
		turning[0] = turning[1];
		directionForTurn[0] = directionForTurn[1];
		nextDirection = directionForTurn[1];
		directionForTurn[1] = Vector2Zero();
		
		turning[1] = NONE;

	}
	else if (turning[0] == UP) {
		position.x = tilePosition.x - tileSize / 2;
		//Sets the direction the snake will move
		playerInput = nextDirection;

		//Resets bool
		directions[UP] = false;		
		PlaySound(Game::GetInstance()->sounds["Turn"]);
		turning[0] = turning[1];
		directionForTurn[0] = directionForTurn[1];
		nextDirection = directionForTurn[1];
		directionForTurn[1] = Vector2Zero();
		
		turning[1] = NONE;


	}
	else if (turning[0] == DOWN) {
		playerInput = nextDirection;
		position.x = tilePosition.x - tileSize / 2;
		//Resets bool
		directions[DOWN] = false;
		PlaySound(Game::GetInstance()->sounds["Turn"]);
		turning[0] = turning[1];
		directionForTurn[0] = directionForTurn[1];
		nextDirection = directionForTurn[1];
		directionForTurn[1] = Vector2Zero();
		
		turning[1] = NONE;

	}
}

#pragma endregion
/// <summary>
/// Adds to the snake length
/// </summary>
void Player::AddLength() {
	snakeLength++;//Adds new snake length
	snake.push_back(newTail);//Adds a new tail to the snake list
	if (snakeLength - 1 >= maxLength) { maxLength = snakeLength - 1; };//Sets the max length to the length if its greater
}
void Player::RemoveLength() {
	snakeLength--;//Removes 1 from the snake length
	RecordTileSpot(gridSpot);
	snake.pop_back();//Removes the last tail piece
}
void Player::CollisonCheck() {
	if (markedForDeletion) { return; }
	//Iterates through the list of game objects
	for (int i = 0; i < gameList.size(); i++) {

		//Continues if the player cant collide with it
		if (gameList[i]->classification == UI || gameList[i]->classification == PLAYER) { continue; }

		//AABB collision with some leeway
		bool collisionWithObjectCheck = Colliding(gameList[i]->tilePosition);
		
		//Makes sure the player collides with a wall
		if (collisionWithObjectCheck && gameList[i]->classification == WALL) {
			WallCollison(i);
			return;//Returns the function to break the loop
		}


		//Detects if the player has hit a collectible
		else if (collisionWithObjectCheck && gameList[i]->classification == COLLECTIBLE) {
			gameList[i]->Death();//Destroys the collectible
			AddLength();//Adds length
			cout << name << "Collected an " << gameList[i]->name << endl;//Debug log
			Game::GetInstance()->playerStats.tilePath = tilePath;

			CollectibleType type = ((Collectible*)gameList[i])->powerType;
			if (type == BASE) {
				Game::GetInstance()->CreateCollectible();//creates a new one
			}
			gameList = Game::GetInstance()->objects;//Makes sure the player has the updated list
			
			return;
		}
	}
	//Makes sure the player isnt out of bounds
	if (OutsideBoundsCheck()) {
		
		Death();
		cout << name << " went out of bounds" << endl;
	}
	//If the snake is just one
	if (!(snakeLength > 2)) { return; }

	//Determines if the snake has run into itself and not an empty grid spot
	for (int i = 2; i < tilePath.size() - 2; i++) {
		//Makes sure that the snake is consitently collidng with itself
		
		//if the players grid spot is already on the map
		if (gridSpot.x == tilePath[i].x && gridSpot.y == tilePath[i].y) {
			
			PlaySound(Game::GetInstance()->sounds["Death"]);
			cout << "You have collided with Yourself" << endl;//Logs that the player hit itself
			Death();
			return;
		}
	}

}
/// <summary>
/// Sets the tile position to the position of the object in correlation to the grid
/// </summary>
void Player::SetTilePosition() {

	int halfTile = tileSize / 2;

	//Checks if the player has crossed thye threshold for a new tile
	bool leftCheck = (position.x >= (tilePosition.x - halfTile) + (tileSize * playerInput.x));
	bool rightCheck = (position.x + tileSize <= (tilePosition.x + halfTile) + (tileSize * playerInput.x));
	bool upCheck = (position.y >= (tilePosition.y - halfTile) + (tileSize * playerInput.y));
	bool downCheck = (position.y + tileSize <= (tilePosition.y + halfTile) + (tileSize * playerInput.y));

	bool inTile = (leftCheck && (playerInput.x == 1)) ||
			(rightCheck && (playerInput.x == -1)) ||
			(upCheck && (playerInput.y == 1)) ||
			(downCheck && (playerInput.y == -1));

	if (!inTile || speed == 0) { return; }

	Vector2 nextTile = {
	tilePosition.x + ((tileSize / 2) * (playerInput.x)) + (playerInput.x * tileSize / 2),
	tilePosition.y + ((tileSize / 2) * (playerInput.y) + (playerInput.y * tileSize / 2))
	};
	tilePosition = nextTile;

	//Changes directions
	ChangeDirections();

	//Sets the gridSpot by dividing the tile position by the tile size
	gridSpot = { (float)(tilePosition.x / tileSize),(float)(tilePosition.y / tileSize) };
	RecordTileSpot(gridSpot);
	
}
/// <summary>
/// Shoots a laser in the direction the player is moving
/// </summary>
void Player::Shoot() {
	Vector2 laserOffset = (playerInput.x != 0) ? Vector2{ tilePosition.x, tilePosition.y - tileSize / 2 }: Vector2{tilePosition.x - tileSize/2, tilePosition.y};
	Laser* laser = new Laser("Laser", laserOffset, playerInput, baseDamage + expolsionDamage);//Creates a laser
	PlaySound(Game::GetInstance()->sounds["Shoot"]);
	RemoveLength();//Removes length
}
/// <summary>
/// Sets the stats of the player in the game
/// </summary>
void Player::SetStats() {
	Game::GetInstance()->playerStats.maxLength = maxLength;//Sets the length
	Game::GetInstance()->playerStats.length = snakeLength - 1;//Sets the length
}
void Player::SetPowerUp() {
	vector<PowerUp> powerUpList = Game::GetInstance()->playerStats.powerUps;
	for (PowerUp powerUp : powerUpList) {
		if (powerUp.isActive && !powerUp.hasntActivated) {
			switch (powerUp.type) {
			case WALLBREAKER:
				if (!wallBreaker) {
					WallBreaker();
					Bar* wallBreakerBar = new Bar("WallBreakerBar", position, powerUp.uses, WHITE, powerUp.type);

					powerUpBars.push_back(wallBreakerBar);
				}
				break;
			case DOUBLEFIRE:
				if (!doubleFire) {
					StartDoubleFire();
					Bar* doubleFireBar = new Bar("WallBreakerBar", position, powerUp.durationInt, PURPLE, powerUp.type);

					powerUpBars.push_back(doubleFireBar);
				}
				break;
			case EXPLOSIVEROUNDS:
				if (!explosiveRounds) {
					ExplosiveRounds();
					Bar* exlopsiveRoundsBar = new Bar("WallBreakerBar", position, powerUp.durationInt, ORANGE, powerUp.type);

					powerUpBars.push_back(exlopsiveRoundsBar);
				}
				break;
			}
			}
		}
	}

void Player::DoubleFire() {
	Vector2 laserOffset = (playerInput.x != 0) ? Vector2{ tilePosition.x, tilePosition.y - tileSize / 2 } : Vector2{ tilePosition.x - tileSize / 2, tilePosition.y };

	Vector2 laserLeft = (playerInput.y == 0) ? Vector2Add(laserOffset, Vector2{ 0,-(float)tileSize / 10 }): Vector2Add(laserOffset, Vector2{ -(float)tileSize / 10, 0});
	Vector2 laserRight = (playerInput.y == 0) ? Vector2Add(laserOffset, Vector2{ 0,(float)tileSize / 10 }): Vector2Add(laserOffset, Vector2{ (float)tileSize / 10, 0});

	Laser* laser = new Laser("Laser", laserLeft, playerInput, baseDamage + expolsionDamage);//Creates a laser
	Laser* laser1 = new Laser("Laser", laserRight, playerInput,baseDamage + expolsionDamage);//Creates a laser
	PlaySound(Game::GetInstance()->sounds["Shoot"]);
	PlaySound(Game::GetInstance()->sounds["Shoot"]);
	RemoveLength();//Removes length
}
void Player::WallCollison(int i) {
	string tempName = gameList[i]->name;//Sets a temp name for the log to display after the game has ended
	cout << name << " collided with " << tempName << endl;//Displays the name of the collisions that took place in the log
	if (!wallBreaker && !markedForDeletion) {
		Death();
	}
	else{
		gameList[i]->Death();
		for (int i = 0; i < Game::GetInstance()->playerStats.powerUps.size() - 1; i++) {
			if (Game::GetInstance()->playerStats.powerUps[i].type == WALLBREAKER) {
				Game::GetInstance()->playerStats.powerUps[i].uses--;
			}
		}
	}
}

void Player::RemovePowerUp(){
	bool isPowered = false;
	bool resetSpeedBoost = false;
	bool resetSpeedSlow = false;
	for (PowerUp powerUp : Game::GetInstance()->playerStats.powerUps) {
		if (!powerUp.isActive) {
			switch (powerUp.type) {
				case WALLBREAKER:
					wallBreaker = false;
					continue;

				case DOUBLEFIRE:
					doubleFire = false;
					continue;

				case EXPLOSIVEROUNDS:
					explosiveRounds = false;
					expolsionDamage = 0;
					continue;

				case SPEEDBOOST:
					speedBoost = false;
					resetSpeedBoost = true;
					Game::GetInstance()->playerStats.scoreMultipler = 1;
					continue;

				case SLOWMOTION: 
					slowMotion = false;
					resetSpeedSlow = true;
					continue;
				default:
					continue;
			}
		}
		else if (powerUp.isActive) {
			isPowered = true;
		}
	}
	
	if (resetSpeedSlow && resetSpeedBoost) {
		speed = baseSpeed;
	}
	if (!isPowered) {
		intervalblink = 0;
		boosted = false;
		isGlowing = false;
	}
}
void Player::UpdatePowerUpBar() {
	vector<PowerUp> powerUpList = Game::GetInstance()->playerStats.powerUps;
	int yPos = 0;

	for (PowerUp powerUp : powerUpList) {
		for (int i = 0; i < powerUpBars.size(); i++) {
			if (powerUp.type != powerUpBars[i]->type) { continue; }
			yPos += tileSize / 6;
			((GameObject*)powerUpBars[i])->position.x = position.x;
			((GameObject*)powerUpBars[i])->position.y = position.y + yPos;
			switch (powerUpBars[i]->type) {
				case WALLBREAKER:
					if (powerUp.isActive) {
						powerUpBars[i]->currentLength = powerUp.uses;
					}
					else {
						((GameObject*)powerUpBars[i])->Death();
						powerUpBars.erase(powerUpBars.begin() + i);
					}
					break;
				case EXPLOSIVEROUNDS:
					if (powerUp.isActive) {
						powerUpBars[i]->currentLength = powerUp.duration;
					}
					else {
						((GameObject*)powerUpBars[i])->Death();
						powerUpBars.erase(powerUpBars.begin() + i);
					}
					break;
				case DOUBLEFIRE:
					if (powerUp.isActive) {
						powerUpBars[i]->currentLength = powerUp.duration;
					}
					else {
						((GameObject*)powerUpBars[i])->Death();
						powerUpBars.erase(powerUpBars.begin() + i);
					}
					break;
				case SPEEDBOOST:
					if (powerUp.isActive) {
						powerUpBars[i]->currentLength = powerUp.duration;
					}
					else {
						((GameObject*)powerUpBars[i])->Death();
						powerUpBars.erase(powerUpBars.begin() + i);
					}
					break;
				case SLOWMOTION:
					if (powerUp.isActive) {
						powerUpBars[i]->currentLength = powerUp.duration;
					}
					else {
						((GameObject*)powerUpBars[i])->Death();
						powerUpBars.erase(powerUpBars.begin() + i);
					}
					break;


			}
		}
	}
}

void Player::WallBreaker() {

	wallBreaker = true;
	isGlowing = true;
	boosted = true;
}
void Player::ExplosiveRounds() {

	expolsionDamage = 150;

	isGlowing = true;
	explosiveRounds = true;
	boosted = true;
}
void Player::StartDoubleFire() {
	
	doubleFire = true;
	isGlowing = true;
	boosted = true;
}
void Player::SlowMotion() {
	slowMotion = true;
	speed = slowMotionSpeed;
	
}
void Player::SpeedBooster() {
	speedBoost = true;
	speed = speedBoostSpeed;
	Game::GetInstance()->playerStats.scoreMultipler = 50;
	
}
void Player::Death() {
	SetStats();//Sets the final stats
	PlaySound(Game::GetInstance()->sounds["Death"]);
	UIManager::GetInstance()->CreateDeathUI();
	Game::GetInstance()->currentState  = END;

	markedForDeletion = true;

	speed = 0;
}

