#include "ClientTest.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Network/NetworkManager.h>

void ClientTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	cam = new Camera();
	cam->PositionCamera(0,0,0,0,0,-5,0,1,0);

	_renderManager->setCurrentCam(cam);

	dt = 0.0f;

	Network::NetworkManager::Instance()->Init();

	//server address
	//192.168.1.102
	_serverAddress = Network::IPAddress("192.168.1.102");

	//init connection
	if (_socket.Connect(2435, _serverAddress) == Network::Socket::Done)
		serverConnected = true;
	else
		serverConnected = false;

	serverMessage = "";
	sendingTimeCounter = 0.0f;
}

void ClientTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void ClientTest::CleanUp()
{
	//delete font;
}

void ClientTest::Pause()
{

}

void ClientTest::Resume()
{

}

void ClientTest::HandleEvents(GameManager* sManager)
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

void ClientTest::Update(GameManager* sManager)
{
	//network shit
	if (serverConnected)
	{
		//if (serverMessage != "")
		//{
			sendingTimeCounter += dt;
			if (sendingTimeCounter > 0.04f)//25fps
			{
				Network::Packet newPacket;
				newPacket << serverMessage;
				_socket.Send(newPacket);

				serverMessage = "";
				sendingTimeCounter = 0.0f;
			}			
		//}
	}

	//delta time
	dt = _clock.getTime();
	_clock.Reset();	
}
void ClientTest::Draw(GameManager* sManager)
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
	if (serverConnected)
	{
		RenderManager::Instance()->drawText(font,1,30,"Connected to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}else
	{
		RenderManager::Instance()->drawText(font,1,30,"Not connected to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}

	RenderManager::Instance()->EndFrame();
}

