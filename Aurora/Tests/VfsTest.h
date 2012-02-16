#include <Aurora/Graphics/Image.h>
#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <Aurora/Graphics/TextureManager.h>
#include <Aurora/Utils/GameManager.h>
#include <Aurora/Utils/GameState.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/System/Clock.h>
#include <Aurora/System/FileManager.h>

using namespace Aurora;
using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora::Math;

class VfsTest : public GameState
{
private:

	TrueTypeFont* font;

	RenderManager* _renderManager;
	SystemManager* _systemManager;
	FileManager* _fileManager;

	Image* itemImage;
	Sprite* itemSprite;

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

