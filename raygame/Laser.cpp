#include "Laser.h"
#include <iostream>
#include "Game.h"
#include "Explosion.h"

Laser::Laser(string newName, Vector2 pos, Vector2 dir)
{
	name = newName;//Sets the name of the object
	position = pos;//sets the starting position of the object
	direction = dir;//Sets the direction the object will move in
	damage = 50;//Sets the amount of damage the laser does

	Start();//Starts the object
}
Laser::Laser(string newName, Vector2 pos, Vector2 dir, int damageToDeal)
{
	name = newName;//Sets the name of the object
	position = pos;//sets the starting position of the object
	direction = dir;//Sets the direction the object will move in
	damage = damageToDeal;

	Start();//Starts the object
}

void Laser::Draw()
{
	//Sets the size the laser should be
	Vector2 laserSize = { (float)tileSize / 10, (float)tileSize / 2 };

	//Sets the offset of the y direction  to account for the size and rotation
	int originOffsetY = (direction.y == 1 ) ? -tileSize * 2:(direction.x == 1)? tileSize * 2 : 0;

	//Sets the origin to account for the size changes
	Vector2 origin = { (- tileSize / 2), (tileSize / 2) + originOffsetY};

	//Draws the laser
	DrawRectanglePro({position.x, position.y, laserSize.x, laserSize.y}, origin, rotation, YELLOW);
}

void Laser::Start()
{
	GameObject::Start();
	CalcRotation();//Gets the rotation of the laser
	speed = 1000;//Sets the speed of the laser
	classification = PROJECTILE;//Sets the classification
	baseScoreToAdd = damage / 10;
	tailLengthAtSpawn = Game::GetInstance()->playerStats.maxLength;
	tailLengthMultiplier = tailLengthAtSpawn;
	for (PowerUp powerUp : Game::GetInstance()->playerStats.powerUps) {
		if (powerUp.type == EXPLOSIVEROUNDS) {
 			isExplosive = powerUp.isActive;
			break;
		}
	}
}

void Laser::Update()
{
	GameObject::Update();//Base update
	Move();//Moves the laser
	CollisonCheck();//Checks for collisions
}
/// <summary>
///Moves the lasers position
/// </summary>
void Laser::Move() {
	int frameSpeed = GetFrameTime() * speed;

	position.x += direction.x * frameSpeed;
	position.y += direction.y * frameSpeed;
}
/// <summary>
/// Checks for collisions
/// </summary>
void Laser::CollisonCheck() {
	//Iterates through the objects list
	for (GameObject* obj : gameList) {
		//Makes sure the laser is only checking against walls
		if (obj->classification != WALL) { return; }
		//checks if the object has collided
		if (Colliding(obj->tilePosition)) {
			CollisonWithWall(obj);
			return;
		}
	}
	//Makes sure the laser is not out of bounds
	if (OutsideBoundsCheck()) {
		cout << name << " went out of bounds and is destroyed" << endl;//Debig for the laser going out of  bounds
		Death();
		return;
	}
}
void Laser::CalcRotation() {
	//Sets the rotation to 90 if its moving on the x axis
	if (direction.x != 0) {
		rotation = 90;
	}
}
void Laser::CollisonWithWall(GameObject* obj) {
	cout << name << " hit " << obj->name << " for " << to_string(damage) << endl;//Debug for showing damage
	if (isExplosive) {
		Explosion* newExplosion = new Explosion("BOOM", obj->tilePosition, tileSize * 2, 25);
	}
	obj->RemoveHealth(damage);//Does damage to the object hit
	int scoreToAdd = baseScoreToAdd * tailLengthMultiplier * Game::GetInstance()->playerStats.scoreMultipler;
	Game::GetInstance()->AddScore(scoreToAdd);
	Text floatingTextValues = {
			to_string(scoreToAdd * Game::GetInstance()->playerStats.scoreMultipler),
			Vector2Add(obj->tilePosition, {((float)tileSize / 2) + -(tileSize * direction.x),((float)tileSize / 2) + -(tileSize * direction.y)}),
			tileSize / ((scoreToAdd >= 50)? 1 : 2),
			(scoreToAdd >= 50)? YELLOW : WHITE
	};
	Title* floatingText = new Title("Floating Text", floatingTextValues, 100, 5);
	PlaySound(Game::GetInstance()->sounds["WallHit"]);
	Death();
}

