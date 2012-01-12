#include "EnetServerTest.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Network/NetworkManager.h>

void EnetServerTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	objModel  = new ModelObj();
	objModel->LoadObj("Assets/Models/Obj/portal_gun/portal_gun.obj");

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	cam = new Camera();
	cam->PositionCamera(0,0,0,0,0,-5,0,1,0);

	_renderManager->setCurrentCam(cam);

	dt = 0.0f;

	Network::NetworkManager::Instance()->Init();
	{
		enet_initialize();

		_address.host = ENET_HOST_ANY;
		_address.port = 2435;

		_server = enet_host_create (& _address /* the address to bind the server host to */, 
			32      /* allow up to 32 clients and/or outgoing connections */,
			2      /* allow up to 2 channels to be used, 0 and 1 */,
			0      /* assume any amount of incoming bandwidth */,
			0      /* assume any amount of outgoing bandwidth */);

		if (_server == NULL)
			_serverRunning = false;
		else
			_serverRunning = true;
	}
}

void EnetServerTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void EnetServerTest::CleanUp()
{
	
}

void EnetServerTest::Pause()
{

}

void EnetServerTest::Resume()
{

}

void EnetServerTest::HandleEvents(GameManager* sManager)
{
	_systemManager->Update();

	//camera
	//rotate
	if(_clientMessage == "Left")
	{
		cam->RotateView(2.0f * dt,0,1,0);
	}
	if(_clientMessage == "Right")
	{
		cam->RotateView(-(2.0f * dt),0,1,0);
	}
	if(_clientMessage == "Up")
	{
		cam->PitchView(2.0f * dt);
	}
	if(_clientMessage == "Down")
	{
		cam->PitchView(-(2.0f * dt));
	}

	//move
	if(_clientMessage == "W")
	{
		cam->Move(6.0f * dt);
	}
	if(_clientMessage == "S")
	{
		cam->Move(-(6.0f * dt));
	}
	if(_clientMessage == "A")
	{
		cam->Strafe(-(6.0f * dt));
	}
	if(_clientMessage == "D")
	{
		cam->Strafe(6.0f * dt);
	}
}
void EnetServerTest::Update(GameManager* sManager)
{
	//network stuff
	if (_serverRunning)
	{
		ENetEvent event;

		/* Wait up to 1000 milliseconds for an event. */
		while (enet_host_service (_server, &event, 10 ) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				/* Store any relevant client information here. */
				event.peer -> data = "Client information";

				break;

			case ENET_EVENT_TYPE_RECEIVE:

				char text[50];
				sprintf(text,"%s",event.packet -> data);
				_clientMessage = text;

				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy (event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				/* Reset the peer's client information. */
				event.peer -> data = NULL;
			}
		}
	}

	//delta time
	dt = _clock.getTime();
	_clock.Reset();
}
void EnetServerTest::Draw(GameManager* sManager)
{
	RenderManager::Instance()->StartFrame();
	RenderManager::Instance()->SetPerspective();
	RenderManager::Instance()->ClearScreen();

	RenderManager::Instance()->UpdateCurrentCamera();

	//draw 3d model
	RenderManager::Instance()->DrawModelObj(objModel,Vector3(0,0,-3),Vector3(1,1,1),Vector3(0,0,0));

	//change ortho for text
	RenderManager::Instance()->SetOrtho();
	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//client message
	RenderManager::Instance()->drawText(font,1,30,_clientMessage.c_str(),Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	RenderManager::Instance()->EndFrame();
}

