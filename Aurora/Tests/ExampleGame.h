#include <Aurora/Graphics/Camera.h>
#include <Aurora/Graphics/TextureManager.h>
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <Aurora/Utils/GameManager.h>
#include <Aurora/Utils/GameState.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/System/Clock.h>
#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Sprite3D.h>
#include <Aurora/Math/Vector3.h>

using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora::Math;

class ExampleState : public GameState
{
private:

	Image* itemImage;
	Image* renderToImage;

	Sprite3D* sprite3d;
	Sprite* itemSprite;
	float spriteX,spriteY;

	TrueTypeFont* font;

	Camera *cam;

	RenderManager* _renderManager;
	SystemManager* _systemManager;

	Clock _clock;

	int mouseX,mouseY;
	float analogX,analogY;
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

