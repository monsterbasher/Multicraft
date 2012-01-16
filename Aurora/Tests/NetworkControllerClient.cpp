#include "NetworkControllerClient.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Network/NetworkManager.h>


NetworkInputControllerClient::NetworkInputControllerClient(int listenPort)
{
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
	return false;
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
				receivedPacket >> packetType >> serverName;

				//check if we found already this server
				if (_foundServers.find(serverName) == _foundServers.end())
				{
					//add new one
					_foundServers.insert(std::pair<std::string,Network::IPAddress>(serverName,senderIP));
				}
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////

void NetworkControllerClient::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	cam = new Camera();
	cam->PositionCamera(0,0,0,0,0,-5,0,1,0);

	_renderManager->setCurrentCam(cam);

	dt = 0.0f;

	Network::NetworkManager::Instance()->Init();

	//controller init
	networkInput = new NetworkInputControllerClient(2634);
	networkInput->Start();

	serverMessage = "";
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

	serverMessage = "";

	//camera
	//rotate
	if(_systemManager->keyHold(Key::Left))
	{
		serverMessage = "Left";
	}
	if(_systemManager->keyHold(Key::Right))
	{
		serverMessage = "Right";
	}
	if(_systemManager->keyHold(Key::Up))
	{
		serverMessage = "Up";
	}
	if(_systemManager->keyHold(Key::Down))
	{
		serverMessage = "Down";
	}

	//move
	if(_systemManager->keyHold(Key::W))
	{
		serverMessage = "W";
	}
	if(_systemManager->keyHold(Key::S))
	{
		serverMessage = "S";
	}
	if(_systemManager->keyHold(Key::A))
	{
		serverMessage = "A";
	}
	if(_systemManager->keyHold(Key::D))
	{
		serverMessage = "D";
	}
}

void NetworkControllerClient::Update(GameManager* sManager)
{
	networkInput->Update(dt);

	//delta time
	dt = _clock.getTime();
	_clock.Reset();	
}
void NetworkControllerClient::Draw(GameManager* sManager)
{
	RenderManager::Instance()->StartFrame();
	RenderManager::Instance()->SetPerspective();
	RenderManager::Instance()->ClearScreen();

	//RenderManager::Instance()->UpdateCurrentCamera();

	//change ortho for text
	RenderManager::Instance()->SetOrtho();
	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw client message
	if (networkInput->GetControllerState() == CONNECTED)
	{
		RenderManager::Instance()->drawText(font,1,30,"Connected to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else if (networkInput->GetControllerState() == SEARCHING)
	{
		RenderManager::Instance()->drawText(font,1,30,"Searching server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

		char serverNumber[50];
		sprintf(serverNumber,"Servers found: %d",networkInput->NumberOfFoundServers());
		RenderManager::Instance()->drawText(font,1,45,serverNumber,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else
	{
		RenderManager::Instance()->drawText(font,1,30,"Not connected to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}


	RenderManager::Instance()->EndFrame();
}

