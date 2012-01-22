#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <Aurora/Audio/Sound.h>
#include <Aurora/Audio/AudioManager.h>
#include <Aurora/Utils/GameManager.h>
#include <Aurora/Utils/GameState.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/System/Clock.h>

using namespace Aurora;
using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora::Math;
using namespace Aurora::Audio;

class AudioTest : public GameState
{
private:
	 
	TrueTypeFont* font;
	Sound* sound;

	RenderManager* _renderManager;
	SystemManager* _systemManager;
	AudioManager* _audioManager;

	Clock _clock;
	float dt;

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

