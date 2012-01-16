#include "NetworkControllerClient.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Network/NetworkManager.h>

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
	_controllerState = SEARCHING;
	_serverPort = 2634;

	_serverSearcherSocket.SetBlocking(false);
	if(!_serverSearcherSocket.Bind(_serverPort))
	{
		int erroror = 1;
	}
	

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
	//switch states
	if (_controllerState == SEARCHING)
	{
		char buffer[128];
		std::size_t received;
		Network::IPAddress sender;
		unsigned short port;

		if (_serverSearcherSocket.Receive(buffer, sizeof(buffer), received, sender, port) == Network::Socket::Done)
		{
			int test = 1;
			std::string bufferMessage = buffer;
			if (bufferMessage == "ServerHere")
			{
				bool serverInList = false;
				for (int i = 0 ; i < _foundServers.size();i++)
				{
					if (_foundServers[i].ToString() == sender.ToString())
					{
						serverInList = true;
					}
				}

				if (!serverInList)
				{
					_foundServers.push_back(sender);
				}
			}
		}
	}

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
	if (_controllerState == CONNECTED)
	{
		RenderManager::Instance()->drawText(font,1,30,"Connected to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else if (_controllerState == SEARCHING)
	{
		RenderManager::Instance()->drawText(font,1,30,"Searching server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

		char serverNumber[50];
		sprintf(serverNumber,"Servers found: %d",_foundServers.size());
		RenderManager::Instance()->drawText(font,1,45,serverNumber,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else
	{
		RenderManager::Instance()->drawText(font,1,30,"Not connected to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}


	RenderManager::Instance()->EndFrame();
}

