#include "UIManager.h"
#include "Game.h"
#include "raymath.h"
#include "raylib.h"

UIManager* UIManager::instance = NULL;

void UIManager::Start() {
	centerScreen = Game::GetInstance()->screenWidth/2;
	menuButtonSpacing = 10;
	menuButtonHeight = Game::GetInstance()->screenHeight / 5;
	menuButtonWidth = Game::GetInstance()->screenWidth / 3;
	endButtonY = Game::GetInstance()->screenHeight / 2 + menuButtonHeight;

	MenuValues();
	CreateStartMenu();//Creates the start menu
}

UIManager* UIManager::GetInstance()
{
	if (instance == NULL) {
		instance = new UIManager();
	}
	return instance;
	
}

/// <summary>
/// Creates the start menu
/// </summary>
void UIManager::CreateStartMenu() {
	Game::GetInstance()->EraseGameObjects();//Deletes of the game objects

	Game::GetInstance()->currentState = MENU;//Sets the state

	//Text and sprites
	Title* startTextObj = new Title("Start Text", startTextMenu);
	ImageUI* logoImage = new ImageUI("Logo", logoSprite);


	//Buttons
	Button* startButtonObj = new Button("Start Button", startButtonMenu, startButtonMenuSprite);
	Button* quitButtonObj = new Button("Quit Button", quitButtonMenu, quitButtonMenuSprite);
	Button* easy = new Button("Difficulty easy", easyModeButton, easyModeButtonSprite);

	easy->selected = (Game::GetInstance()->level == EASY);

	Button* medium = new Button("Difficulty medium", mediumModeButton, mediumModeButtonSprite);
	medium->selected = (Game::GetInstance()->level == MEDIUM);

	Button* hard = new Button("Difficulty hard", hardModeButton, hardModeButtonSprite);
	hard->selected = (Game::GetInstance()->level == HARD);

	//Button* upgradesButtonObj = new Button("Upgrade Button", upgradesButtonMenu, upgradesButtonMenuSprite);

}

/// <summary>
/// Creates the end menu
/// </summary>
void UIManager::CreateEndMenu() {
	//Sets the values for the score and length
	scoreText.text = scoreDisplayText + to_string(Game::GetInstance()->playerStats.score);
	maxLengthText.text = maxLengthDisplayText + to_string(Game::GetInstance()->playerStats.maxLength);

	//Text
	ImageUI* gameOverSprite = new ImageUI("Game Over Sprite", gameOverLogo);
	Title* scoreTextObj = new Title("Score Text", scoreText);
	Title* maxLengthTextObj = new Title("Max length text", maxLengthText);
	Title* startTextObj = new Title("Start Text", startTextEnd);

	//Buttons
	Button* restartButtonObj = new Button("Restart Button", restartButtonEnd, restartButtonSpriteEnd);
	Button* continueButtonObj = new Button("Continue Button", continueButtonEnd, continueButtonSpriteEnd);
}

void UIManager::CreateSettingsMenu() {
	Game::GetInstance()->EraseGameObjects();//Deletes of the game objects

	Game::GetInstance()->currentState = SETTINGS;//Sets the current state

	Title* settingsTitleObj = new Title("Settings Title", settingsTitle);
	Title* settingsBackToMenuTextObj = new Title("Settings Back to menu text", settingsBackToMenuText);

	Button* menuButtonObj = new Button("Back To Menu Button", backToMenu);

}
void UIManager::CreateDifficulty() {
	Game::GetInstance()->EraseGameObjects();
	Game::GetInstance()->currentState = SETTINGS;

	Title* diificultTitle = new Title("Difficulty title", dificultyTitle);
	Button* backToMenu = new Button("Difficulty title", difficultyBackToMenu);

	
	Button* easy = new Button("Difficulty title", easyModeButton, easyModeButtonSprite);
	Button* medium = new Button("Difficulty title", mediumModeButton, mediumModeButtonSprite);
	Button* hard = new Button("Difficulty title", hardModeButton, hardModeButtonSprite);

}
void UIManager::CreatePlayUI() {

	Title* scoreTextObj = new Title("Score Text", scorePlayerStatText, Color{ 0,0,0,150 }, 150);
	playerStatsUI.insert(pair<Title*, pair<string, int*>>(scoreTextObj, pair<string, int*>(baseScoreText, &Game::GetInstance()->playerStats.score)));//Adds it to the list
	Title* lengthTextObj = new Title("Length Text", lengthPlayerStatText, Color{ 0,0,0,150 }, 150);
	playerStatsUI.insert(pair<Title*, pair<string, int*>>(lengthTextObj, pair<string, int*>(baseLengthText, &Game::GetInstance()->playerStats.length)));//Adds it to the list
	Title* maxLengthTextObj = new Title("Max Length Text", maxLengthPlayerStatText, Color{ 0,0,0,150 }, 150);
	playerStatsUI.insert(pair<Title*, pair<string, int*>>(maxLengthTextObj, pair<string, int*>(baseMaxLengthText, &Game::GetInstance()->playerStats.maxLength)));//Adds it to the list

	ImageUI* wallBreakerImage = new ImageUI("WallBreakerIcon", wallBreakerIcon);
	Title* wallBreakerCountText = new Title("WallBreakerText", wallBreakerText);

	ImageUI* doubleFireImage = new ImageUI("DoubleFireIcon", doubleFireIcon);
	Title* doubleFireTextObj = new Title("DoubleFireText", doubleFireText);

	ImageUI* explosiveRoundsImage = new ImageUI("EplosiveRoundsIcon", exlopiveRoundsIcon);
	Title* sxplosiveRoundsText = new Title("ExplosiveRoundsText", explosiveRoundsText);

	ImageUI* slowMotionImage = new ImageUI("SlowMotionIcon", slowMotionIcon);
	Title* slowMotionTextOBJ = new Title("SlowMotionText", slowMotionText);

	ImageUI* speedBoostImage = new ImageUI("SpeedBoostIcon", speedBoostIcon);
	Title* speedBoostTextOBJ = new Title("SpeedBoostText", speedBoostText);

	Text bgText = { "20000", Vector2{(float)centerScreen,(float)Game::GetInstance()->screenHeight / 2}, 100000, {0,0,0,0} };
	Title* title = new Title("Clear", bgText, { 160,160,160,100 }, 200);
	tutorialObjs.push_back(title);

	Title* arrowControlsTextObj = new Title("Arrow Keys Control Text", arrowControlsText);
	tutorialObjs.push_back(arrowControlsTextObj);
	Title* wasdControlsTextObj = new Title("wasd Keys Control Text", wasdControlsText);
	tutorialObjs.push_back(wasdControlsTextObj);
	Title* shootControlsTextObj = new Title("shoot Keys Control Text", shootControlsText);
	tutorialObjs.push_back(shootControlsTextObj);


	//TODO: make text for the controls for the speedboost and slow motion
	//TODO: make the text blink when active
	//TODO: make screen tint based on powerup (OPTIONAL)
	for (int i = 0; i < Game::GetInstance()->playerStats.powerUps.size(); i++) {
		switch (Game::GetInstance()->playerStats.powerUps[i].type) {
		case WALLBREAKER:
			playerStatsUI.insert(pair<Title*, pair<string, int*>>(wallBreakerCountText, pair<string, int*>("", &Game::GetInstance()->playerStats.powerUps[i].uses)));
			break;
		case EXPLOSIVEROUNDS:
			playerStatsUI.insert(pair<Title*, pair<string, int*>>(sxplosiveRoundsText, pair<string, int*>("", &Game::GetInstance()->playerStats.powerUps[i].durationInt)));
			break;

		case DOUBLEFIRE:
			playerStatsUI.insert(pair<Title*, pair<string, int*>>(doubleFireTextObj, pair<string, int*>("", &Game::GetInstance()->playerStats.powerUps[i].durationInt)));
			break;

		case SLOWMOTION:
			playerStatsUI.insert(pair<Title*, pair<string, int*>>(slowMotionTextOBJ, pair<string, int*>("", &Game::GetInstance()->playerStats.powerUps[i].durationInt)));

			break;
		case SPEEDBOOST:
			playerStatsUI.insert(pair<Title*, pair<string, int*>>(speedBoostTextOBJ, pair<string, int*>("", &Game::GetInstance()->playerStats.powerUps[i].durationInt)));

			break;
		default:
			break;
		}
	}


}
void UIManager::EndDrawTutorial() {
	for (Title* title : tutorialObjs) {
		((GameObject*)title)->Death();
	}
	tutorialObjs.clear();
}

void UIManager::CreateDeathUI() {
	for (GameObject* obj : Game::GetInstance()->objects) {
		if (obj->classification == UI) {
			obj->Death();
		}
	}
	playerStatsUI.clear();

	Text bgText = { "20000", Vector2{(float)centerScreen,(float)Game::GetInstance()->screenHeight / 2}, 100000, {0,0,0,0} };
	Title* title = new Title("Clear", bgText, { 0,0,0,100 }, 100);
	CreateEndMenu();
}
void UIManager::MenuValues() {
	MenuUISetup();
	SettingsUISetup();
	PlayUISetup();
	PowerUpUISetup();
	EndScreenUISetup();
}

void UIManager::MenuUISetup() {
	//Menu UI
	logoSprite = {
		Vector2{(float)centerScreen,75},
		Game::GetInstance()->gameTextures["Logo"],
		.5f
	};
	startTextMenu = {
		 "Press ENTER to play",
		 Vector2{(float)centerScreen,
		 (float)Game::GetInstance()->screenHeight - 50},
		 20,
		 YELLOW };

	startButtonMenu = {
		"Start",
		BLACK,
		Vector2{ (float)centerScreen, (float)Game::GetInstance()->screenHeight / 2 - menuButtonHeight },
		Vector2{menuButtonWidth, menuButtonHeight},
		[] {Game::GetInstance()->GetInstance()->StartPlay(); } };
	startButtonMenuSprite = {
		startButtonMenu.position,
		Game::GetInstance()->gameTextures["ButtonTexture0"],
		1
	};
	//EASY MODE
	easyModeButton = {
		"Easy",
		BLACK,
		Vector2{ (float)centerScreen - menuButtonWidth/2 - menuButtonSpacing,(float)Game::GetInstance()->screenHeight / 2 },
		Vector2{menuButtonWidth/2, menuButtonHeight/2},
		[] {Game::GetInstance()->EasyMode();
			}
	};
	easyModeButtonSprite = {
		easyModeButton.position,
		Game::GetInstance()->gameTextures["ButtonTexture0"],
		1
	};
	//MEDIUM MODE
	mediumModeButton = {
		"Medium",
		BLACK,
		Vector2{ (float)centerScreen, (float)Game::GetInstance()->screenHeight / 2 },
		Vector2{menuButtonWidth/2, menuButtonHeight/2},
		[] {Game::GetInstance()->MediumMode();
			}
	};
	mediumModeButtonSprite = {
		mediumModeButton.position,
		Game::GetInstance()->gameTextures["ButtonTexture1"],
		1
	};
	//HARD MODE
	hardModeButton = {
		"Hard",
		BLACK,
		Vector2{ (float)centerScreen + menuButtonWidth/2 + menuButtonSpacing, (float)Game::GetInstance()->screenHeight / 2 },
		Vector2{menuButtonWidth/2, menuButtonHeight/2},
		[] {Game::GetInstance()->HardMode();
			 }
	};
	hardModeButtonSprite = {
		hardModeButton.position,
		Game::GetInstance()->gameTextures["ButtonTexture2"],
		1
	};
	//settingsButtonMenu = {
	//	"Difficulty",
	//	BLACK,
	//	Vector2{ (float)centerScreen, (float)Game::GetInstance()->screenHeight / 2 + menuButtonSpacing },
	//	Vector2{menuButtonWidth, menuButtonHeight},
	//	[] {GetInstance()->CreateDifficulty(); } };
	//settingsButtonMenuSprite = {
	//	settingsButtonMenu.position,
	//	Game::GetInstance()->gameTextures["ButtonTexture0"],
	//	1
	//};
	//upgradesButtonMenu = {
	//	"Upgrades",
	//	BLACK,
	//	Vector2{ (float)centerScreen, (float)Game::GetInstance()->screenHeight / 2 + menuButtonHeight + (menuButtonSpacing * 2)},
	//	Vector2{menuButtonWidth, menuButtonHeight},
	//	[] {Game::GetInstance()->GetInstance()->StartPlay(); } };
	//upgradesButtonMenuSprite = {
	//	upgradesButtonMenu.position,
	//	Game::GetInstance()->gameTextures["ButtonTexture0"],
	//	1
	//};
	quitButtonMenu = {
		"QUIT",
		BLACK,
		Vector2{ (float)centerScreen, (float)Game::GetInstance()->screenHeight / 2 + menuButtonHeight + (menuButtonSpacing * 2) },
		Vector2{menuButtonWidth, menuButtonHeight},
		[] {Game::GetInstance()->windowShouldClose = true; }
	};
	quitButtonMenuSprite = {
		quitButtonMenu.position,
		Game::GetInstance()->gameTextures["ButtonTexture0"],
		1
	};
}
void UIManager::SettingsUISetup() {

///UNUSED AS OF RIGHT NOW
#pragma region Settings UI
	settingsTitle = {
		"Settings",
		Vector2{(float)centerScreen, 50},
		75,
		BLACK };
	settingsBackToMenuText = {
		"Press ENTER to go back",
		 Vector2{(float)centerScreen,
		 (float)Game::GetInstance()->screenHeight - 50},
		 20,
		 YELLOW };
	backToMenu = {
		"Back to menu",
		RED,
		Vector2{(float)centerScreen, endButtonY},
		Vector2{menuButtonWidth, menuButtonHeight},
		[] {GetInstance()->CreateStartMenu(); } };

#pragma endregion
}

void UIManager::PlayUISetup() {
#pragma region Play UI
	arrowControlsText = {
		"Use ARROW KEYS to change directions",
		Vector2{(float)centerScreen, (float)Game::GetInstance()->screenHeight / 2},
		Game::GetInstance()->GRID_TILE_SIZE,
		BLACK
	};

	wasdControlsText = {
		"Use WASD keys to change directions",
		Vector2{((float)Game::GetInstance()->screenWidth - (float)centerScreen), (float)Game::GetInstance()->screenHeight / 3},
		Game::GetInstance()->GRID_TILE_SIZE,
		BLACK
	};

	shootControlsText = {
		"Use SPACE to shoot\n\n\n\n CRTL for slow motion \n\n\n\nSHIFT for speedboost",
		Vector2{((float)centerScreen), (float)Game::GetInstance()->screenHeight / 6},
		Game::GetInstance()->GRID_TILE_SIZE,
		BLACK
	};

	baseScoreText = "Score: ";
	scorePlayerStatText = {
		baseScoreText,
		Vector2{((float)centerScreen), (float)Game::GetInstance()->GRID_TILE_SIZE},
		Game::GetInstance()->GRID_TILE_SIZE,
		WHITE
	};

	baseLengthText = "Length: ";
	lengthPlayerStatText = {
		baseLengthText,
		Vector2{(float)Game::GetInstance()->screenWidth / 5, (float)Game::GetInstance()->GRID_TILE_SIZE},
		Game::GetInstance()->GRID_TILE_SIZE,
		WHITE
	};

	baseMaxLengthText = "Highest Length: ";
	maxLengthPlayerStatText = {
		baseMaxLengthText,
		Vector2{((float)Game::GetInstance()->screenWidth - (float)Game::GetInstance()->screenWidth / 5), (float)Game::GetInstance()->GRID_TILE_SIZE},
		Game::GetInstance()->GRID_TILE_SIZE,
		WHITE
	};


#pragma endregion
}
void UIManager::PowerUpUISetup() {
	wallBreakerIcon = {
		Vector2{(float)Game::GetInstance()->screenWidth - (Game::GetInstance()->GRID_TILE_SIZE + Game::GetInstance()->GRID_TILE_SIZE / 2),(float)Game::GetInstance()->GRID_TILE_SIZE * 4 },
		Game::GetInstance()->gameTextures["BatteringRam"],
		1
	};
	wallBreakerText = {
		"0",
		Vector2Add(wallBreakerIcon.position, Vector2{(float)Game::GetInstance()->GRID_TILE_SIZE, 0}),
		20,
		WHITE
	};
	doubleFireIcon = {
		Vector2{(float)Game::GetInstance()->screenWidth - (Game::GetInstance()->GRID_TILE_SIZE + Game::GetInstance()->GRID_TILE_SIZE / 2),(float)Game::GetInstance()->GRID_TILE_SIZE * 6 },
		Game::GetInstance()->gameTextures["DoubleFireIcon"],
		2
	};
	doubleFireText = {
		"0",
		Vector2Add(doubleFireIcon.position, Vector2{(float)Game::GetInstance()->GRID_TILE_SIZE, 0}),
		20,
		WHITE
	};
	exlopiveRoundsIcon = {
		Vector2{(float)Game::GetInstance()->screenWidth - (Game::GetInstance()->GRID_TILE_SIZE + Game::GetInstance()->GRID_TILE_SIZE / 2),(float)Game::GetInstance()->GRID_TILE_SIZE * 8 },
		Game::GetInstance()->gameTextures["ExplosionIconTexture"],
		.25
	};
	explosiveRoundsText = {
		"0",
		Vector2Add(exlopiveRoundsIcon.position, Vector2{(float)Game::GetInstance()->GRID_TILE_SIZE, 0}),
		20,
		WHITE
	};
	speedBoostIcon = {
		Vector2{(float)Game::GetInstance()->screenWidth - Game::GetInstance()->screenWidth / 4 ,(float)Game::GetInstance()->screenHeight - Game::GetInstance()->GRID_TILE_SIZE },
		Game::GetInstance()->gameTextures["SpeedIcon"],
		.25
	};
	speedBoostText = {
		"0",
		Vector2Add(speedBoostIcon.position, Vector2{(float)Game::GetInstance()->GRID_TILE_SIZE, 0}),
		20,
		WHITE
	};
	slowMotionIcon = {
		Vector2{(float)Game::GetInstance()->screenWidth / 4,(float)Game::GetInstance()->screenHeight - Game::GetInstance()->GRID_TILE_SIZE },
		Game::GetInstance()->gameTextures["FreezeIcon"],
		.25
	};
	slowMotionText = {
		"0",
		Vector2Add(slowMotionIcon.position, Vector2{(float)Game::GetInstance()->GRID_TILE_SIZE, 0}),
		20,
		WHITE
	};
}
void UIManager::EndScreenUISetup() {
	
#pragma region EndScreen UI
	gameOverLogo = {
		Vector2{(float)centerScreen,75},
		Game::GetInstance()->gameTextures["GameOver"],
		.5f
	};
	startTextEnd = {
		 "Press ENTER to go to the menu OR R to restart",
		 Vector2{(float)centerScreen,
		 (float)Game::GetInstance()->screenHeight - 50},
		 20,
		 YELLOW };
	scoreDisplayText = "Score: ";
	scoreText = {
		 scoreDisplayText,
		 Vector2{ (float)centerScreen, 300 },
		 50,
		 BLACK };
	maxLengthDisplayText = "Highest tail length: ";
	maxLengthText = {
	maxLengthDisplayText,
		Vector2{ (float)centerScreen, 500 },
			50,
			BLACK };
	//End UI Buttons
	float endButtonY = Game::GetInstance()->screenHeight / 2 + menuButtonHeight;
	float endButtonXOffset = Game::GetInstance()->GRID_TILE_SIZE * 2;
	restartButtonEnd = {
		"Restart",
		BLACK,
		Vector2{centerScreen - menuButtonWidth + endButtonXOffset, endButtonY},
		Vector2{menuButtonWidth, menuButtonHeight},
		[] {Game::GetInstance()->StartPlay(); } };
	restartButtonSpriteEnd = {
		restartButtonEnd.position,
		Game::GetInstance()->gameTextures["ButtonTexture2"],
		1
	};
	continueButtonEnd = {
		"Continue",
		BLACK,
		Vector2{centerScreen + menuButtonWidth - endButtonXOffset, endButtonY},
		Vector2{menuButtonWidth, menuButtonHeight},
		[] {GetInstance()->CreateStartMenu(); } };
	continueButtonSpriteEnd = {
		continueButtonEnd.position,
		Game::GetInstance()->gameTextures["ButtonTexture2"],
		1
	};
}
void UIManager::UpdatePlayUI() {
	for (GameObject* obj : Game::GetInstance()->objects) {
		//Makes sure the object is not deleted before updtatig text
		if (obj->classification != UI) { continue; }
		if (obj->markedForDeletion) { continue; }

		auto stat = playerStatsUI.begin();
		while (stat != playerStatsUI.end()) {
			if (((Title*)obj)->displayText == stat->first->displayText) {
				stat->first->displayText = stat->second.first + to_string((int)*(stat->second.second));
			}
			stat++;

		}

	}
}