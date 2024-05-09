#include "Wall.h"
#include "Game.h"
#include "Title.h"

Wall::Wall() {
	name = "Wall";
}
Wall::Wall(string newName, Vector2 pos, bool powerup) {
	name = newName;//Sets the name of the object
	classification = WALL;//Sets the classification
	position = pos;//Sets the position of the wall
	isPowerUp = powerup;
	if (powerup) {
		canGlow = true;
	}
	else {
		tint = WHITE;
	}
	
	Start();//Starts the object
}
void Wall::Start() {
	GameObject::Start();//Base start
	wallTexture = &Game::GetInstance()->gameTextures["WallTexture0"];//Sets the wall texture to that of the first
	health = Game::GetInstance()->difficulty.wallHealth;//Determines the health 
	scale = 0;//Scale of the block
	healthChange = health;//Determines the health differences
	baseScoreToAdd = 50;
	wallRec = { 0,0, (float)(*wallTexture).width, (float)(*wallTexture).height };//Sets a rectangle for the texture to use
	wallRec2 = { position.x ,position.y,(float)tileSize + scale ,(float)tileSize + scale };// Sets the second one
	origin = { 0,0 };//Sets the orgin in the top left
	QuestionMarkDraw();
}
void Wall::Update() {
	GameObject::Update();//Base Update
	GetWallTexture();//Gets the wall texture if its been hit
	DeathAnimation();//Death animation
}
void Wall::Draw() {
	
	DrawTexturePro((*wallTexture), wallRec, wallRec2, origin, 0, tint);//Draws the game object
}
void Wall::QuestionMarkDraw() {
	if (!canGlow) { return; }
	wallTexture = &Game::GetInstance()->gameTextures["wallMystery"];
}
void Wall::Death() {
	dead = true;//Sets dead to true
	animationTimes = 0;//sets the amount of animation frames done to 0
	animationFrameTime = 2;//Sets the time in between frames for the animation
	if (isPowerUp) {
		Game::GetInstance()->CreatePowerUp(tilePosition);
	}
	tilePosition = { (float)-tileSize,(float)-tileSize };//Ensures no collisions will happen
	Game::GetInstance()->AddScore(baseScoreToAdd * Game::GetInstance()->playerStats.scoreMultipler);
	PlaySound(Game::GetInstance()->sounds["WallDeath"]);
}
/// <summary>
/// Gets the wall texture
/// </summary>
void Wall::GetWallTexture() {
	if (health == healthChange) { return; }
	int textureNumber = health % 6;//Decides how much health is left
	int nums[7] = {6,5,4,3,2,1,0};//reversed number list
	//Gets a new texture depending on the amount of health left
	wallTexture = &Game::GetInstance()->gameTextures["WallTexture" + to_string(nums[textureNumber])];
	healthChange = health;//Sets the new health change
}
/// <summary>
/// Does the death animation if its dead
/// </summary>
void Wall::DeathAnimation() {
	if (!dead) { return; }//Returns if niot dead
	//If the it is not the correct frame to play the animation on it wont
	if (animationTimes % animationFrameTime == 0) {
		//Sets the new wall texture
		wallTexture = &Game::GetInstance()->gameTextures["WallTexture" + to_string(animationTimes / animationFrameTime + 6)];
	}
	animationTimes++;//Increment the animation time
	scale = animationTimes;//Scales the object up to make it look explosive
	if (animationTimes >= animationFrameTime * 3) {
		Text floatingTextValues = {
			to_string(baseScoreToAdd * Game::GetInstance()->playerStats.length * Game::GetInstance()->playerStats.scoreMultipler),
			Vector2Add(position, {(float)tileSize / 2,(float)tileSize / 2}),
			tileSize*2,
			YELLOW
		};
		Title* floatingText = new Title("Floating Text", floatingTextValues, 100, 5);

		GameObject::Death();//Finishes the death sequence
	}
}