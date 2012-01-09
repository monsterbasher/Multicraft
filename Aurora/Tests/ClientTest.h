#include <Aurora/Graphics/Camera.h>
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <Aurora/Utils/GameManager.h>
#include <Aurora/Utils/GameState.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/System/Clock.h>

#include <Aurora/Network/IPAddress.hpp>
#include <Aurora/Network/Selector.hpp>
#include <Aurora/Network/SocketTCP.hpp>
#include <Aurora/Network/Packet.hpp>

using namespace Aurora;
using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora::Math;

class ClientTest : public GameState
{
	private:

	TrueTypeFont* font;

	Camera *cam;

	RenderManager* _renderManager;
	SystemManager* _systemManager;

	Clock _clock;
	float dt;

	//network variables
	Network::IPAddress _serverAddress;
	Network::SocketTCP _socket;
	bool serverConnected;
	std::string serverMessage;

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

