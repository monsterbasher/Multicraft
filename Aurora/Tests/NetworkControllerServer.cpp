#include "NetworkControllerServer.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Network/NetworkManager.h>


NetworkInputClientInfo::NetworkInputClientInfo(std::string name,Network::IPAddress ip,Network::SocketTCP socket)
{
	_clinetName = name;
	_clientIp = ip;
	_clientSocket = socket;
	_packetNew = false;
}

void NetworkInputClientInfo::MarkPackedReaded()
{
	_packetNew = false;
}

void NetworkInputClientInfo::MarkPackedNew()
{
	_packetNew = true;
}

Network::Packet NetworkInputClientInfo::GetPacket()
{
	return _packet;
}

void NetworkInputClientInfo::SetPacket(Network::Packet packet)
{
	_packet = packet;

	MarkPackedNew();
}

Network::IPAddress NetworkInputClientInfo::GetAddress()
{
	return _clientIp;
}

std::string NetworkInputClientInfo::GetName()
{
	return _clinetName;
}

void  NetworkInputClientInfo::SetName(std::string name)
{
	_clinetName = name;
}

Network::SocketTCP NetworkInputClientInfo::GetSocket()
{
	return _clientSocket;
}

NetworkInputServer::NetworkInputServer(std::string name,int serverPort)
{
	//broadcast data
	_broadcastName = name;
	_broadcastPort = serverPort;
	_broadcastTimer = 0;
	_serverRunning = false;

	//here i must get address of local network and make from it broadcast address
	_broadcadtAddress = Network::IPAddress("192.168.1.255");//last 255 to broadcast to whole sub network

	//server TCP socket and selector
	if (!_serverSocket.Listen(serverPort))
	{
		int erroro = 1;
	}
	
	//add server socket to selector
	_selector.Add(_serverSocket);

	//clear all clients
	_connectedClients.clear();
}

void NetworkInputServer::Start()
{
	_serverRunning = true;
}

void NetworkInputServer::Stop()
{
	_serverRunning = false;
}

bool NetworkInputServer::ContainsClientWithAddress(Network::IPAddress address)
{
	bool addressFound = false;

	for (unsigned int i = 0;i < _connectedClients.size();i++)
	{
		if (_connectedClients[i]->GetAddress() == address)
		{
			addressFound = true;
		}
	}

	return addressFound;
}



void NetworkInputServer::Update(float dt)
{
	if (_serverRunning)
	{
		//server info broadcasting broadcasting
		{
			_broadcastTimer += dt;
			if (_broadcastTimer > 2)//every 2 seconds
			{
				Network::Packet sendPacket;
				sendPacket << 20 << _broadcastName;//20 is packet number for server broadcasting

				if(_broadcastSocket.Send(sendPacket,_broadcadtAddress,_broadcastPort) != Network::Socket::Done)
				{
					//error
					int errorSending = 1;
				}
				_broadcastTimer = 0.0f;
			}
		}


		//clients support
		{
			// Get the sockets ready for reading
			unsigned int readySockets = _selector.Wait(0.01f);//10 milliseconds of timeout

			for (unsigned int i = 0; i < readySockets; i++)
			{
				//get ready socket
				Network::SocketTCP socket = _selector.GetSocketReady(i);

				if (socket == _serverSocket)//means that there is new connection to server
				{
					Network::IPAddress clientAddress;
					Network::SocketTCP clientSocket;

					_serverSocket.Accept(clientSocket, &clientAddress);

					if (!ContainsClientWithAddress(clientAddress))//we don't have client with this address
					{
						//so we add new client with uknow name right now
						NetworkInputClientInfo *newClient = new NetworkInputClientInfo("",clientAddress,clientSocket);
						_connectedClients.push_back(newClient);

						_selector.Add(clientSocket);
					}
				}else //there is message from client
				{
					Network::Packet receivedPacket;
					if(socket.Receive(receivedPacket) == Network::Socket::Done)
					{
						//find clinet that it's from 
						NetworkInputClientInfo *client = GetClientBySocket(socket);
						if(client != 0)
						{
							//ok let's check what that packet is
							int packetType = -1;
							receivedPacket >> packetType;

							if(packetType == 21)//clients name :>
							{
								std::string newName = "";
								receivedPacket >> newName;
								client->SetName(newName);
							}

						}
					}
				}
			}
		}
	}
}

int NetworkInputServer::GetClientsCount()
{
	return _connectedClients.size();
}

NetworkInputClientInfo *NetworkInputServer::GetClientByNumber(int i)
{
	return _connectedClients[i];
}

NetworkInputClientInfo *NetworkInputServer::GetClientByName(std::string name)
{
	for (int i = 0;i < _connectedClients.size();i++)
	{
		if (_connectedClients[i]->GetName() == name)
		{
			return _connectedClients[i];
		}
	}

	return 0;
}

NetworkInputClientInfo* NetworkInputServer::GetClientBySocket(Network::SocketTCP socket)
{
	for (int i = 0;i < _connectedClients.size();i++)
	{
		if (_connectedClients[i]->GetSocket() == socket)
		{
			return _connectedClients[i];
		}
	}

	return 0;
}

NetworkInputClientInfo* NetworkInputServer::GetClientByAddress(Network::IPAddress address)
{
	for (int i = 0;i < _connectedClients.size();i++)
	{
		if (_connectedClients[i]->GetAddress() == address)
		{
			return _connectedClients[i];
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkControllerServer::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	dt = 0.0f;

	Network::NetworkManager::Instance()->Init();

	_inputServer = new NetworkInputServer("TestServer1",2634);
	_inputServer->Start();
}

void NetworkControllerServer::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void NetworkControllerServer::CleanUp()
{
	
}

void NetworkControllerServer::Pause()
{

}

void NetworkControllerServer::Resume()
{

}

void NetworkControllerServer::HandleEvents(GameManager* sManager)
{
	_systemManager->Update();
}
void NetworkControllerServer::Update(GameManager* sManager)
{
	_inputServer->Update(dt);
	
	//delta time
	dt = _clock.getTime();
	_clock.Reset();
}
void NetworkControllerServer::Draw(GameManager* sManager)
{
	//start frame
	RenderManager::Instance()->StartFrame();
	RenderManager::Instance()->ClearScreen();

	//change ortho for text
	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//clients number
	char clinetsNumber[40];
	sprintf(clinetsNumber,"Clients number: %d",_inputServer->GetClientsCount());
	RenderManager::Instance()->drawText(font,1,30,clinetsNumber,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//name of each clients
	int startPos = 45;
	for(int i = 0; i < _inputServer->GetClientsCount(); i++)
	{
		char clinetsInfo[50];
		sprintf(clinetsInfo,"Client %d with name: %s",i,_inputServer->GetClientByNumber(0)->GetName().c_str());
		RenderManager::Instance()->drawText(font,1,startPos,clinetsInfo,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
		
		startPos += 15;
	}	

	RenderManager::Instance()->EndFrame();
}

