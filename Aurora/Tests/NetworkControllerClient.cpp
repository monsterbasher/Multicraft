#include "NetworkControllerClient.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Network/NetworkManager.h>


NetworkInputControllerClient::NetworkInputControllerClient(std::string clientName,int listenPort)
{
	//name
	_clientName = clientName;

	//listening port
	_serverListeningPort = listenPort;

	//set searching servers socket to non blocking
	_serverSearcherSocket.SetBlocking(false);

	//bind it to correct port
	if(!_serverSearcherSocket.Bind(_serverListeningPort))
	{
		_controllerState = NOTBIND;
	}else
	{
		_controllerState = IDLE;
	}

	//clear servers info
	_foundServers.clear();
}

int NetworkInputControllerClient::NumberOfFoundServers()
{
	return _foundServers.size();
}

NetworControllerState NetworkInputControllerClient::GetControllerState()
{
	return _controllerState;
}

bool NetworkInputControllerClient::IsConnectedToServer()
{
	return _controllerState == CONNECTED;
}

void NetworkInputControllerClient::Start()
{
	//clear servers info
	_foundServers.clear();

	//switch to search mode
	_controllerState = SEARCHING;
}

void NetworkInputControllerClient::Stop()
{

}

std::string NetworkInputControllerClient::GetServerNameAtPos(int pos)
{
	return _foundServers[pos].Name;
}

void NetworkInputControllerClient::ConnectToServer(std::string serverName)
{
	for (unsigned int i = 0; i < _foundServers.size();i++)
	{
		if (_foundServers[i].Name == serverName)
		{
			_defaulServerAddress = _foundServers[i].Address;
		}		
	}

	_controllerState = CONNECTING;
}

void NetworkInputControllerClient::Update(float dt)
{
	if (_controllerState == SEARCHING)
	{
		Network::IPAddress senderIP;
		unsigned short senderPort;
		Network::Packet receivedPacket;

		if (_serverSearcherSocket.Receive(receivedPacket,senderIP,senderPort) == Network::Socket::Done)
		{
			//check if it's "Hello World" message from broadcasting server
			int packetType = -1;
			receivedPacket >> packetType;

			if (packetType == 20)//server broadcast
			{
				std::string serverName = "";
				receivedPacket >> serverName;

				//if there is no servers make first one default
				if (_foundServers.size() == 0)
				{
					_defaulServerAddress = senderIP;
				}

				//check if we found already this server
				bool serverFound = false;
				for (unsigned int i = 0; i < _foundServers.size();i++)
				{
					if (_foundServers[i].Name == serverName)
					{
						serverFound = true;
					}
				}

				if (!serverFound)
				{
					//add new one
					NetworkInputServerInfo serverInfo;
					serverInfo.Address = senderIP;
					serverInfo.Name = serverName;

					_foundServers.push_back(serverInfo);
				}
			}
		}
	}
	else if (_controllerState == CONNECTING)
	{
		//try to connect to server
		if (_clientSocket.Connect(_serverListeningPort, _defaulServerAddress) != Network::Socket::Done)
			_controllerState = NOTCONNECTED;
		else
		{
			_controllerState = CONNECTED;

			//send our name there :>
			Network::Packet namePacket;
			namePacket << 21 << _clientName;//21 clientName packet
			
			_clientSocket.Send(namePacket);
		}
	}
	else if (_controllerState == CONNECTED)
	{

	}
}


///////////////////////////////////////////////////////////////////////////////////////

void NetworkControllerClient::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	dt = 0.0f;

	Network::NetworkManager::Instance()->Init();
	//controller init
	networkInput = new NetworkInputControllerClient("Client1",2634);
	networkInput->Start();

	serverName = "";
}

void NetworkControllerClient::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void NetworkControllerClient::CleanUp()
{
	//delete font;
}

void NetworkControllerClient::Pause()
{

}

void NetworkControllerClient::Resume()
{

}

void NetworkControllerClient::HandleEvents(GameManager* sManager)
{
	_systemManager->Update();
}

void NetworkControllerClient::Update(GameManager* sManager)
{
	networkInput->Update(dt);

	//for test purpose we connect to the first found server
	if (networkInput->GetControllerState() == SEARCHING && networkInput->NumberOfFoundServers() > 0)
	{
		serverName = networkInput->GetServerNameAtPos(0);
		networkInput->ConnectToServer(serverName);
	}

	//delta time
	dt = _clock.getTime();
	_clock.Reset();	
}
void NetworkControllerClient::Draw(GameManager* sManager)
{
	RenderManager::Instance()->StartFrame();
	RenderManager::Instance()->ClearScreen();

	//change ortho for text
	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw client message
	if (networkInput->GetControllerState() == CONNECTED)
	{
		char serverN[50];
		sprintf(serverN,"Connected to server: %s",serverName.c_str());
		RenderManager::Instance()->drawText(font,1,30,serverN,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else if (networkInput->GetControllerState() == SEARCHING)
	{
		RenderManager::Instance()->drawText(font,1,30,"Searching for server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

		char serverNumber[50];
		sprintf(serverNumber,"Servers found: %d",networkInput->NumberOfFoundServers());
		RenderManager::Instance()->drawText(font,1,45,serverNumber,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else if (networkInput->GetControllerState() == CONNECTING)
	{
		RenderManager::Instance()->drawText(font,1,30,"Connecting to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else
	{
		RenderManager::Instance()->drawText(font,1,30,"Not connected to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}


	RenderManager::Instance()->EndFrame();
}

