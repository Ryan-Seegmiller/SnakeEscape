#include "Explosion.h"
#include "Game.h"

Explosion::Explosion(string newName, Vector2 pos, int expRadius, int damageToDeal)
{
	name = newName;//Sets the name of the object
	position = pos;//Sets the position of the object
	radius = expRadius;//Sets the radius of the explosion
	damage = damageToDeal;//Sets the damage to deal with the explosion
	classification = PROJECTILE;//Sets the classification

	Start();//Starts the gameobjet
}


void Explosion::Start()
{
	GameObject::Start();//Base start 

	//Sets the position to be the poisition to be centered in the tile
	currentTexture = Game::GetInstance()->gameTextures["ExplosionTexture0"]; 
	PlaySound(Game::GetInstance()->sounds["Explosion"]);
	position.x = tilePosition.x - currentTexture.width / 2 + tileSize/2;
	position.y = tilePosition.y - currentTexture.height / 2 + tileSize/2;

	animationFrame = 5;
	frame = 0;
	textureNum = 1;

	CollisonCheck();//Checks collision on once
	tempColor = ORANGE;
}

void Explosion::Update()
{
	GameObject::Update();//Base update
	
}

void Explosion::Draw()
{
	////Lerps values to make the sxplosion fade
	//tempColor.a = Lerp(tempColor.a, 0, .1f * GetFrameTime());
	//radius = Lerp(radius, 0, .1f * GetFrameTime());

	//DrawCircle(position.x,position.y, radius, tempColor);

	////If the radius is less than visible destroy the object
	//if (radius <= 0.5) {
	//	Death();
	//}
	Animation();
}
void Explosion::Animation() {
	frame++;
	if (frame % animationFrame == 0) {
		currentTexture = Game::GetInstance()->gameTextures["ExplosionTexture" + to_string(textureNum++)];
	}
	DrawTexture(currentTexture,position.x,position.y,WHITE);
	if (textureNum >= 7) {
		Death();
	}
}
void Explosion::CollisonCheck()
{
	for (GameObject* obj : gameList) {
		//Makes sure its not damaging anything but walls 
		if (obj->classification != WALL || obj == this ) { continue; }

		//Gets the distance from the center of the explosion to the object
		int distanceToObj = Vector2Length(Vector2Subtract(position, obj->tilePosition));

		//Checks if the object is within the bounds of the explosion
		if (distanceToObj > radius + tileSize || obj->health <= 0) { continue; }
		obj->RemoveHealth(damage);//Does damage toi the object
	}
}
