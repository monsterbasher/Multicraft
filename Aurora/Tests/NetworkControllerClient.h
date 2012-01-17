#include <Aurora/Graphics/Camera.h>
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <Aurora/Utils/GameManager.h>
#include <Aurora/Utils/GameState.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/System/Clock.h>

#include <Aurora/Network/IPAddress.hpp>
#include <Aurora/Network/SocketUDP.hpp>
#include <Aurora/Network/SocketTCP.hpp>
#include <Aurora/Network/Packet.hpp>

#include <string>
#include <map>

using namespace Aurora;
using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora::Math;

enum NetworControllerState
{
	IDLE,
	SEARCHING,
	CONNECTING,
	CONNECTED,
	NOTCONNECTED,
	WAITING,
	NOTBIND
};


class NetworkInputServerInfo
{
public:

	std::string Name;
	Network::IPAddress Address;
};

class NetworkInputControllerClient
{
private:

	NetworControllerState _controllerState;

	std::vector<NetworkInputServerInfo> _foundServers;

	int _serverListeningPort;
	Network::SocketUDP _serverSearcherSocket;
	
	std::string _clientName;
	Network::SocketTCP _clientSocket;
	Network::IPAddress _defaulServerAddress;

public:

	NetworkInputControllerClient(std::string clientName,int listenPort);

	int NumberOfFoundServers();
	NetworControllerState GetControllerState();
	
	std::string GetServerNameAtPos(int pos);
	void ConnectToServer(std::string serverName);
	bool IsConnectedToServer();
	
	void Start();
	void Stop();
	void Update(float dt);
};

class NetworkControllerClient : public GameState
{
private:

	TrueTypeFont* font;

	RenderManager* _renderManager;
	SystemManager* _systemManager;

	Clock _clock;
	float dt;

	//network variables
	NetworkInputControllerClient* networkInput;

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

