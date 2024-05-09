/*******************************************************************************************
*
*   raylib [core] example - basic window
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "Game.h"
#include "Player.h"
#include "Wall.h"
#include <iostream>
#include "UIManager.h"
using namespace std;


int main()
{
	// Initialization
	Game* game = Game::GetInstance();
	UIManager* UI = UIManager::GetInstance();
	string path = "Resources//Snake//snake0.png";
	

	

	//--------------------------------------------------------------------------------------
	int screenWidth = 1600;
	int screenHeight = 928;

	InitWindow(screenWidth, screenHeight, "Snake Escape");
	SetWindowIcon(LoadImage(path.c_str()));
	InitAudioDevice();
	
	Game::GetInstance()->Start();
	UIManager::GetInstance()->Start();

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose() && !Game::GetInstance()->windowShouldClose)    // Detect window close button or ESC key
	{
		 
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(DARKGREEN);

		// Update
		
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
		
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(DARKGREEN);
		game->UpdateGameObjects();
		//DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	game->Unload();
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}