#include "ServerTest.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>

void ServerTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	cam = new Camera();
	cam->PositionCamera(0,0,0,0,0,-5,0,1,0);

	_renderManager->setCurrentCam(cam);

	dt = 0.0f;

	//
	listening = false;
	clientConnected = false;
	clientMessage = "";


	if (enet_initialize () == 0)
	{
		listening = true;

		address.host = ENET_HOST_ANY;
		address.port = 2573;

		server = enet_host_create (& address /* the address to bind the server host to */, 
									32      /* allow up to 32 clients and/or outgoing connections */,
									2      /* allow up to 2 channels to be used, 0 and 1 */,
									0      /* assume any amount of incoming bandwidth */,
									0      /* assume any amount of outgoing bandwidth */);
		if (server == NULL)
		{
			listening = false;
		}
	}
}

void ServerTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void ServerTest::CleanUp()
{
	//delete font;
}

void ServerTest::Pause()
{

}

void ServerTest::Resume()
{

}

void ServerTest::HandleEvents(GameManager* sManager)
{
	_systemManager->Update();

	//camera
	//rotate
	if(_systemManager->keyHold(Key::Left))
	{
		cam->RotateView(2.0f * dt,0,1,0);
	}
	if(_systemManager->keyHold(Key::Right))
	{
		cam->RotateView(-(2.0f * dt),0,1,0);
	}
	if(_systemManager->keyHold(Key::Up))
	{
		cam->PitchView(2.0f * dt);
	}
	if(_systemManager->keyHold(Key::Down))
	{
		cam->PitchView(-(2.0f * dt));
	}

	//move
	if(_systemManager->keyHold(Key::W))
	{
		cam->Move(6.0f * dt);
	}
	if(_systemManager->keyHold(Key::S))
	{
		cam->Move(-(6.0f * dt));
	}
	if(_systemManager->keyHold(Key::A))
	{
		cam->Strafe(-(6.0f * dt));
	}
	if(_systemManager->keyHold(Key::D))
	{
		cam->Strafe(6.0f * dt);
	}
}
void ServerTest::Update(GameManager* sManager)
{
	//network shit
	if (listening)
	{
		ENetEvent event;

		while (enet_host_service (server, & event, 15) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf ("A new client connected from %x:%u.\n", 
					event.peer -> address.host,
					event.peer -> address.port);

				/* Store any relevant client information here. */
				event.peer -> data = "Client information";

				clientConnected = true;

				break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet -> dataLength,
					event.packet -> data,
					event.peer -> data,
					event.channelID);

				char message[20];
				sprintf(message,"%s",event.packet->data);
				clientMessage = message;

				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy (event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf ("%s disconected.\n", event.peer -> data);

				/* Reset the peer's client information. */

				event.peer -> data = NULL;
			}
		}
	}

	//delta time
	dt = _clock.getTime();
	_clock.Reset();
}
void ServerTest::Draw(GameManager* sManager)
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
	if (clientConnected)
	{
		RenderManager::Instance()->drawText(font,1,30,"Client Connected",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

		if (clientMessage != "")
		{
			RenderManager::Instance()->drawText(font,1,45,clientMessage.c_str(),Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
		}
	}

	//netork info


	RenderManager::Instance()->EndFrame();
}

void ServerTestGameManager::Configure()
{
	//init render manager properties
	RenderManager::Instance()->setSesize(480,272);
}

void ServerTestGameManager::Init()
{
	//init whatever you need
	exampleState = new ServerTest();
	exampleState->Init();

	ChangeState(exampleState);
}

void ServerTestGameManager::CleanUp()
{
	exampleState->CleanUp();
	delete exampleState;
}

