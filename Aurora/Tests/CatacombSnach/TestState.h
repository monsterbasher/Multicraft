#ifndef TESTSTATE_H
#define TESTSTATE_H

#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <Aurora/Utils/GameManager.h>
#include <Aurora/Utils/GameState.h>
#include <Aurora/System/FileManager.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/System/Clock.h>

using namespace Aurora;
using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora::Math;

#include "Player.h"

class SnachTest : public GameState
{
private:

	//framework stuff
	TrueTypeFont* font;

	FileManager* _fileManager;
	RenderManager* _renderManager;
	SystemManager* _systemManager;

	Clock _clock;
	float dt;

	//game stuff
	Player* _player1;

public:

	void Init();
	void Enter();
	void CleanUp();
	void Pause();
	void Resume();
	void HandleEvents(GameManager* sManager);
	void Update(GameManager* sManager);
	void Draw(GameManager* sManager);
};

#endif 
