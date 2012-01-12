#include <Aurora/Graphics/Camera.h>
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <Aurora/Utils/GameManager.h>
#include <Aurora/Utils/GameState.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/System/Clock.h>
#include <Aurora/Math/Vector3.h>
#include <Aurora/Graphics/ModelObj.h>

#include <enet/enet.h>

using namespace Aurora;
using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora::Math;

class EnetServerTest : public GameState
{
	private:

	TrueTypeFont* font;
	ModelObj* objModel;

	Camera *cam;

	RenderManager* _renderManager;
	SystemManager* _systemManager;

	Clock _clock;
	float dt;

	//network variables
	ENetAddress _address;
	ENetHost * _server;
	std::string _clientMessage;
	bool _serverRunning;

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

