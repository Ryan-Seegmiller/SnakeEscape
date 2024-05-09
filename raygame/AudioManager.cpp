#include "AudioManager.h"
#include <iostream>


AudioManager* AudioManager::instance = NULL;


AudioManager* AudioManager::GetInstance()
{
	if (instance == NULL) {
		instance = new AudioManager();
	}
	return instance;
}
