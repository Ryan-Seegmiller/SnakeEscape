#pragma once
#include "GameObject.h"

enum CollectibleType {
	BASE = 1,
	WALLBREAKER = 2,
	DOUBLEFIRE = 3,
	EXPLOSIVEROUNDS = 4,
	SLOWMOTION = 5,
	SPEEDBOOST = 6,
	COLLECTIBLE_END = 7
};
typedef struct PowerUp
{
	CollectibleType type;
	bool isActive;
	bool hasntActivated;
	float duration;
	int durationInt;
	int uses;
};
class Collectible : GameObject
{
public:
	//Constructor
	Collectible(string, Vector2);
	Collectible(string, Vector2, CollectibleType);


	//Base Methods
	void Start();
	void Update();
	void Draw();
	void Death();

	CollectibleType powerType;//PowerType of the collectible

protected:

private:
	Texture2D texture;//Texture of sprite
	Rectangle source;//Source image
	Rectangle dest;//Destination of image
	Vector2 origin;//Origin within image
	int baseScoreToAdd;

	Text deathText;
	Text scoreText;
	PowerUp powerUp;
	
	void WallBreaker();
	void DoubleFire();
	void ExplosiveRounds();
	void SlowMotion();
	void SpeedBoost();
	void SetTexture();

	void CreateDeathText();
};

