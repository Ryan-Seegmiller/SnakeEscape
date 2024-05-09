#pragma once
#include "GameObject.h"

using namespace std;

class Laser : GameObject
{
public:
	//Constructor
	Laser(string, Vector2, Vector2);
	Laser(string, Vector2, Vector2, int);

	Vector2 direction;//direction

	//Base methods
	void Draw();
	void Start();
	void Update();

private:
	void Move();//Moves the laser
	void CollisonCheck();//Checks for collisions
	void CollisonWithWall(GameObject*);//Behaviour for colliding with wall
	void CalcRotation();//Calculates thr rotation for the laser
	int damage;//Damage the laser does to an object
	int rotation;//Rotation for the laser
	int speed;//Rate at which the laser travels
	int tailLengthAtSpawn;//Sets the tail length when the laser was shot
	int tailLengthMultiplier;//Sets the multiplier of the score
	int baseScoreToAdd;//Sets the base score to add
	bool isExplosive;

protected:

};

