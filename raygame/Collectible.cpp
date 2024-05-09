#include "Collectible.h"
#include "Game.h";

Collectible::Collectible(string newName,Vector2 pos) {
	name = newName;//Sets the name for the object
	position = pos;//Sets the position of the object
	classification = COLLECTIBLE;//Sets the classifictaion
	powerType = BASE;

	Start();//Starts the game object
}
Collectible::Collectible(string newName,Vector2 pos, CollectibleType type) {
	name = newName;//Sets the name for the object
	position = pos;//Sets the position of the object
	classification = COLLECTIBLE;//Sets the classifictaion
	powerType = type;

	Start();//Starts the game object
}

void Collectible::Start()
{
	GameObject::Start();//Base start behaviour
	SetTexture();
	source = { 0,0,(float)texture.width,(float)texture.height };//The source of the rectangle
	dest = { position.x,position.y,(float)tileSize, (float)tileSize };//Destination of the rectangle
	origin = { 0, 0 };//Origin
	deathText = {
		"PLACEHOLDER",
		Vector2Add(tilePosition, {((float)tileSize / 2),((float)tileSize + tileSize / 2)}),
		tileSize,
		PURPLE
	};
	baseScoreToAdd = 50;

	scoreText = {
		"PLACEHOLDER",
		Vector2Add(tilePosition, {((float)tileSize / 2),(powerType != BASE)? ((float)tileSize / 2) : ((float)tileSize + tileSize / 2)}),
		tileSize,
		PURPLE
	};
}

void Collectible::Update()
{
	GameObject::Update();//Base Update
}

void Collectible::Draw()
{
	//Draws a gryscaled version of the pickup
	DrawTexturePro(texture,source,dest,origin,0, WHITE);
	
}
void Collectible::SetTexture() {
		switch (powerType) {
		case WALLBREAKER:
			texture = Game::GetInstance()->gameTextures["BatteringRam"];
			break;

		case DOUBLEFIRE:
			texture = Game::GetInstance()->gameTextures["DoubleFireIcon"];
			break;

		case EXPLOSIVEROUNDS:
			texture = Game::GetInstance()->gameTextures["ExplosionIconTexture"];
			break;

		case SLOWMOTION:
			texture = Game::GetInstance()->gameTextures["FreezeIcon"];
			break;

		case SPEEDBOOST:
			texture = Game::GetInstance()->gameTextures["SpeedIcon"];
			break;

		default:
			texture = Game::GetInstance()->gameTextures["CollectiblesTexture0"];
			break;
		}
	
}
void Collectible::Death() {
	if (powerType != BASE) {
		switch (powerType) {
		case WALLBREAKER:
			PlaySound(Game::GetInstance()->sounds["WallRam"]);
			WallBreaker();
			break;

		case DOUBLEFIRE:
			PlaySound(Game::GetInstance()->sounds["Collectible"]);
			DoubleFire();
			break;

		case EXPLOSIVEROUNDS:
			PlaySound(Game::GetInstance()->sounds["ExplosiveRounds"]);
			ExplosiveRounds();
			break;

		case SLOWMOTION:
			PlaySound(Game::GetInstance()->sounds["Freeze"]);
			SlowMotion();
			break;

		case SPEEDBOOST:
			PlaySound(Game::GetInstance()->sounds["SpeedBoost"]);
			SpeedBoost();
			break;

		default:
			
			break;
		}
	}
	else {
		PlaySound(Game::GetInstance()->sounds["Collectible"]);
	}

	CreateDeathText();

	GameObject::Death();
}


void Collectible::WallBreaker()
{
	powerUp = {
		WALLBREAKER,
		true,
		false,
		0,
		10,
		10
	};

	Game::GetInstance()->AddPowerUp(powerUp);

	deathText.text = "+10 Wall Breaks";
}

void Collectible::DoubleFire()
{
	powerUp = {
		DOUBLEFIRE,
		true,
		false,
		10,
		10,
		0
	};

	deathText.text = "Double Fire";

	Game::GetInstance()->AddPowerUp(powerUp);
}

void Collectible::ExplosiveRounds()
{
	powerUp = {
		EXPLOSIVEROUNDS,
		true,
		false,
		10,
		10,
		0
	};

	deathText.text = "Explosive Rounds";

	Game::GetInstance()->AddPowerUp(powerUp);
}

void Collectible::SlowMotion()
{
	bool isActive = false;
	for (PowerUp pup : Game::GetInstance()->playerStats.powerUps) {
		if (pup.isActive) {
			isActive = true;
		}
	}

	powerUp = {
		SLOWMOTION,
		isActive,
		!isActive,
		5,
		5,
		0
	};

	deathText.text = "Slow Motion";

	Game::GetInstance()->AddPowerUp(powerUp);
}

void Collectible::SpeedBoost()
{
	bool isActive = false;
	for (PowerUp pup : Game::GetInstance()->playerStats.powerUps) {
		if (pup.isActive) {
			isActive = true;
		}
	}
	
	powerUp = {
		SPEEDBOOST,
		isActive,
		!isActive,
		5,
		5,
		0
	};

	deathText.text = "Speed Boost";

	Game::GetInstance()->AddPowerUp(powerUp);
	
}

void Collectible::CreateDeathText() {

	int scoreToAdd = baseScoreToAdd * Game::GetInstance()->playerStats.length * Game::GetInstance()->playerStats.scoreMultipler;
	scoreText.text = to_string(scoreToAdd);
	Game::GetInstance()->AddScore(scoreToAdd * Game::GetInstance()->playerStats.scoreMultipler);

	if (powerType != BASE) {
		Title* deathTextTitle = new Title("Floating Text", deathText, 100, 5);
	}
	Title* scoreTextTitle = new Title("Floating Text", scoreText, 100, 5);

}