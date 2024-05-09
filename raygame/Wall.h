#include "GameObject.h"
class Wall : public GameObject 
{
public:
	//Constructors
	Wall();
	Wall(string, Vector2, bool);

	//Texture for the wall
	Texture2D* wallTexture;

	//Base Methods
	void Draw();
	void Update();
	void Start();
	void Death();

	//Animation methods
	void GetWallTexture();//Gets the wall texture when hit
	void DeathAnimation();//Does the death animation
	void QuestionMarkDraw();

	//Score
	int baseScoreToAdd;
	
	//Animation
	int healthChange;//Records the change in health
	bool dead;//Determines if the block should die
	int animationTimes;//Determines how many frames have passed since death
	int animationFrameTime;//Determines if the next frame should change the texture
	int scale;//Scales the block according to the animation

private:
	Rectangle wallRec;
	Rectangle wallRec2;
	Vector2 origin;
	bool isPowerUp;
};