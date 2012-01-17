#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <Aurora/Utils/GameManager.h>
#include <Aurora/Utils/GameState.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/System/Clock.h>

#include <Aurora/Network/NetworkInputControllerClient.h>
#include <Aurora/Network/NetworkManager.h>
#include <Aurora/Network/IPAddress.hpp>
#include <string>

using namespace Aurora;
using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora::Math;

class NetworkControllerClient : public GameState
{
private:

	TrueTypeFont* font;

	RenderManager* _renderManager;
	SystemManager* _systemManager;

	Clock _clock;
	float dt;

	//network variables
	Network::NetworkInputControllerClient* networkInput;

	Network::IPAddress _address;
	std::string serverName;

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

