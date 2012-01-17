#include <Aurora/Graphics/Camera.h>
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <Aurora/Utils/GameManager.h>
#include <Aurora/Utils/GameState.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/System/Clock.h>
#include <Aurora/Math/Vector3.h>
#include <Aurora/Graphics/ModelObj.h>

#include <Aurora/Network/IPAddress.hpp>
#include <Aurora/Network/SocketUDP.hpp>
#include <Aurora/Network/SocketTCP.hpp>
#include <Aurora/Network/Selector.hpp>
#include <Aurora/Network/Packet.hpp>

using namespace Aurora;
using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora::Math;


class NetworkInputClientInfo
{
private:

	std::string _clinetName;
	Network::IPAddress _clientIp;
	Network::SocketTCP _clientSocket;
	Network::Packet _packet;

	bool _packetNew;

	void MarkPackedNew();

public:

	NetworkInputClientInfo(std::string name,Network::IPAddress ip,Network::SocketTCP socket);

	std::string GetName();
	void SetName(std::string name);
	Network::Packet GetPacket();
	Network::IPAddress GetAddress();
	Network::SocketTCP GetSocket();

	void SetPacket(Network::Packet packet);
	void MarkPackedReaded();	
};

class NetworkInputServer
{

private:

	int _broadcastPort;
	float _broadcastTimer;
	std::string _broadcastName;

	Network::SocketUDP _broadcastSocket;
	Network::IPAddress _broadcadtAddress;

	Network::SocketTCP _serverSocket;
	Network::SelectorTCP _selector;

	std::vector<NetworkInputClientInfo*> _connectedClients;

	bool _serverRunning;

public:

	NetworkInputServer(std::string name,int serverPort);

	void Start();
	void Stop();
	void Update(float dt);

	bool ContainsClientWithAddress(Network::IPAddress address);
	int GetClientsCount();

	NetworkInputClientInfo* GetClientByNumber(int i);
	NetworkInputClientInfo* GetClientByName(std::string name);
	NetworkInputClientInfo* GetClientBySocket(Network::SocketTCP socket);
	NetworkInputClientInfo* GetClientByAddress(Network::IPAddress address);
};

class NetworkControllerServer : public GameState
{
private:

	TrueTypeFont* font;

	RenderManager* _renderManager;
	SystemManager* _systemManager;

	Clock _clock;
	float dt;

	//network variables
	NetworkInputServer *_inputServer;

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

