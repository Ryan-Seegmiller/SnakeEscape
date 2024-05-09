#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "UI.h"

using namespace std;

enum ObjectType {
	PLAYER,
	WALL,
	COLLECTIBLE,
	UI,
	PROJECTILE
};
class GameObject
{
	public:
		GameObject();
		GameObject(string);
		~GameObject();
		
		string name;
		
		ObjectType classification;
		vector<GameObject*> gameList;
		//Positional data
		Vector2 position;
		Vector2 tilePosition;
		Vector2 gridSpot;
		int tileSize;
		float rotation;
		int health;
		bool markedForDeletion;
		bool markedForCreation;

		//Glow Values
		bool canGlow;
		Color tint;
		float glowSpeed;
		bool reverseGlow;
		float currentLerpingTime;

		//Basic game object methods
		virtual void Start();
		virtual void Update();
		virtual void Draw();
		void Destroy();
		void ResetTileSize();
		void ResetObjectList();
		bool OutsideBoundsCheck();
		bool Colliding(Vector2);
		void RemoveHealth(int);
		void Glowing();
		virtual void Death();
		

};

