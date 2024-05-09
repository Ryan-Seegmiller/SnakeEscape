#include <vector>
#include "GameObject.h"
#include <map>
#include "Player.h"
#include "Collectible.h"
#include "Button.h"
#include "Title.h"
#include "ImageUI.h"

using namespace std;

#pragma once
class UIManager
{
public:
	static UIManager* GetInstance();

	void Start();

	//UI
	void CreateStartMenu();//Creates the menu
	void CreateSettingsMenu();//Creates the settings menu
	void CreateEndMenu();//Create the end screen
	void CreateDifficulty();
	void CreatePlayUI();//Creates the lay mode UI
	void UpdatePlayUI();//Updates the play UI
	void MenuValues();//Sets the menu values
	void EndDrawTutorial();
	void CreateDeathUI();
	bool clicked;


private:
	float menuButtonSpacing;
	float menuButtonHeight;
	float menuButtonWidth;
	float endButtonY;
	float centerScreen;

	//End screen UI
	void EndScreenUISetup();
	Sprite gameOverLogo;
	Text startTextEnd;//Tells the player how to go to menu the game
	Text scoreText;//Score text 
	string scoreDisplayText;//Base text to display
	Text maxLengthText;//Max length the tail reached text
	string maxLengthDisplayText;//Base text to display

	ButtonBounds restartButtonEnd;//restart Button
	Sprite restartButtonSpriteEnd;//Restart button sprite

	ButtonBounds continueButtonEnd;// Button
	Sprite continueButtonSpriteEnd;//Restart button sprite


	//Menu UI
	void MenuUISetup();
	Sprite logoSprite;//Logo Sprite
	Text startTextMenu;//Tells the player how to start the game

	ButtonBounds startButtonMenu;//Start button
	Sprite startButtonMenuSprite;//Start button Sprite

	ButtonBounds settingsButtonMenu;//Settings button
	Sprite settingsButtonMenuSprite;//Settings button Sprite

	ButtonBounds upgradesButtonMenu;//Upgrades button
	Sprite upgradesButtonMenuSprite;//Upgrades button Sprite

	ButtonBounds quitButtonMenu;
	Sprite quitButtonMenuSprite;

	bool windowShouldClose;


	//Settings UI 
	void SettingsUISetup();
	Text settingsTitle;
	Text settingsBackToMenuText;
	ButtonBounds backToMenu;

	//Difficulty
	void DifficutlySettingsSetup();
	Text dificultyTitle;

	ButtonBounds easyModeButton;
	Sprite easyModeButtonSprite;

	ButtonBounds mediumModeButton;
	Sprite mediumModeButtonSprite;

	ButtonBounds hardModeButton;
	Sprite hardModeButtonSprite;

	ButtonBounds difficultyBackToMenu;
	Sprite difficultyBackToMenuSprite;

	//Play UI
	Text arrowControlsText;
	Text wasdControlsText;
	Text shootControlsText;


	//Player stats UI
	void PlayUISetup();
	void PowerUpUISetup();
	Text scorePlayerStatText;
	string baseScoreText;//Text for the score to use
	Text lengthPlayerStatText;
	string baseLengthText;//Text for the length to use
	Text maxLengthPlayerStatText;
	string baseMaxLengthText;//Text for the max length to use
	map<Title*, pair<string, int*>> playerStatsUI;//Map of the text and their respective base text and values

	Sprite wallBreakerIcon;
	Text wallBreakerText;

	Sprite doubleFireIcon;
	Text doubleFireText;

	Sprite exlopiveRoundsIcon;
	Text explosiveRoundsText;

	Sprite speedBoostIcon;
	Text speedBoostText;

	Sprite slowMotionIcon;
	Text slowMotionText;

	vector<Title*> tutorialObjs;//Things to be added


protected:
	static UIManager* instance;
};

