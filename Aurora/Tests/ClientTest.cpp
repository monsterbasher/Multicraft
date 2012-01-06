#include "ClientTest.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>

void ClientTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	cam = new Camera();
	cam->PositionCamera(0,0,0,0,0,-5,0,1,0);

	_renderManager->setCurrentCam(cam);

	dt = 0.0f;

	//server address
	//192.168.1.102
	_serverAddress = Network::IPAddress("192.168.1.102");

	//init connection
	if (_socket.Connect(2435, _serverAddress) == Network::Socket::Done)
		serverConnected = true;
	else
		serverConnected = false;

	serverMessage = "";
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
		if (serverMessage != "")
		{
			Network::Packet newPacket;
			newPacket << serverMessage;
			_socket.Send(newPacket);

			serverMessage = "";
		}		 
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

	RenderManager::Instance()->UpdateCurrentCamera();

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

	//netork info


	RenderManager::Instance()->EndFrame();
}

void ClientTestGameManager::Configure()
{
	//init render manager properties
	RenderManager::Instance()->setSesize(480,272);
}

void ClientTestGameManager::Init()
{
	//init whatever you need
	exampleState = new ClientTest();
	exampleState->Init();

	ChangeState(exampleState);
}

void ClientTestGameManager::CleanUp()
{
	exampleState->CleanUp();
	delete exampleState;
}

