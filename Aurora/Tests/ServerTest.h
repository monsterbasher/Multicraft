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

class ServerTest : public GameState
{
private:

TrueTypeFont* font;

Camera *cam;

RenderManager* _renderManager;
SystemManager* _systemManager;

Clock _clock;
float dt;

//network variables
bool listening;
Network::SocketTCP _listener;
Network::SelectorTCP _selector;

bool clientConnected;
std::string clientMessage;

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

class ServerTestGameManager : public GameManager
{
private:

ServerTest* exampleState;

public:

void Configure();
void Init();
void CleanUp();
};