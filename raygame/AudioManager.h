#pragma once
class AudioManager
{
public:
	static AudioManager* GetInstance();
private:
protected:
	static AudioManager* instance;
};

