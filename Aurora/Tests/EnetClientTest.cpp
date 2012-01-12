#include "EnetClientTest.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Network/NetworkManager.h>

void EnetClientTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	cam = new Camera();
	cam->PositionCamera(0,0,0,0,0,-5,0,1,0);

	_renderManager->setCurrentCam(cam);

	dt = 0.0f;
	sendingTimeCounter = 0.0f;
	serverMessage = "";

	Network::NetworkManager::Instance()->Init();
	{
		enet_initialize();

		//create client host
		_client = enet_host_create (NULL /* create a client host */,
			1 /* only allow 1 outgoing connection */,
			2 /* allow up 2 channels to be used, 0 and 1 */,
			57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
			14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

		if (_client == NULL)
			_clientCreated = false;
		else
			_clientCreated = true;

		if (_clientCreated)
		{
			//connect to server
			enet_address_set_host (&_address, "192.168.1.64");
			_address.port = 2435;

			//init connection
			_peer = enet_host_connect (_client, &_address, 2, 0);

			if (_peer != NULL)
			{
				//connect to server - 5 seconds time to wait
				if (enet_host_service (_client, &_event, 5000) > 0 && _event.type == ENET_EVENT_TYPE_CONNECT)
				{
					_connectedToServer = true;
				}
				else
				{
					/* Either the 5 seconds are up or a disconnect event was */
					/* received. Reset the peer in the event the 5 seconds   */
					/* had run out without any significant event.            */
					enet_peer_reset (_peer);

					_connectedToServer = false;
				}
			}else
			{
				_connectedToServer = false;
			}			
		}
	}
}

void EnetClientTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void EnetClientTest::CleanUp()
{
	//delete font;
}

void EnetClientTest::Pause()
{

}

void EnetClientTest::Resume()
{

}

void EnetClientTest::HandleEvents(GameManager* sManager)
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

void EnetClientTest::Update(GameManager* sManager)
{
	//network shit
	if (_connectedToServer)
	{
		//if (serverMessage != "")
		//{
			sendingTimeCounter += dt;
			if (sendingTimeCounter > 0.04f)//25fps
			{
				ENetPacket * packet = enet_packet_create (serverMessage.c_str(),strlen (serverMessage.c_str()) + 1,ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send (_peer, 0, packet);
				enet_host_flush (_client);

				serverMessage = "";
				sendingTimeCounter = 0.0f;
			}			
		//}
	}

	//delta time
	dt = _clock.getTime();
	_clock.Reset();	
}
void EnetClientTest::Draw(GameManager* sManager)
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
	if (_connectedToServer)
	{
		RenderManager::Instance()->drawText(font,1,30,"Connected to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}else
	{
		RenderManager::Instance()->drawText(font,1,30,"Not connected to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}

	RenderManager::Instance()->drawText(font,1,45,serverMessage.c_str(),Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	RenderManager::Instance()->EndFrame();
}

