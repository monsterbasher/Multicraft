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
	serverConnected = false;
	serverMessage = "";


	enet_initialize ();
		

	client = enet_host_create (NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
		14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

	if (client != NULL)
	{
		 ENetEvent event;

		enet_address_set_host (& address, "");
		address.port = 2573;

		/* Initiate the connection, allocating the two channels 0 and 1. */
		peer = enet_host_connect (client, & address, 2, 0);    

		if (peer == NULL)
		{
			fprintf (stderr,"No available peers for initiating an ENet connection.\n");
			exit (EXIT_FAILURE);
		}

		/* Wait up to 5 seconds for the connection attempt to succeed. */
		if (enet_host_service (client, & event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			puts ("Connection to some.server.net:1234 succeeded.");
			serverConnected = true;
		}
		else
		{
			/* Either the 5 seconds are up or a disconnect event was */
			/* received. Reset the peer in the event the 5 seconds   */
			/* had run out without any significant event.            */
			enet_peer_reset (peer);

			puts ("Connection to some.server.net:1234 failed.");
		}
	}
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
			//Network::Packet newPacket;
			//newPacket << serverMessage;
			//_socket.Send(newPacket);

			if (client != NULL)
			{
				   ENetPacket * packet = enet_packet_create(serverMessage.c_str(),strlen(serverMessage.c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
				   enet_peer_send (peer, 0, packet);
				   enet_host_flush (client);
			}

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

