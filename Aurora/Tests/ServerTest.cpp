#include "ServerTest.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Network/NetworkManager.h>

void ServerTest::Init()
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

	//my address
	Network::IPAddress Address = Network::IPAddress::GetLocalAddress();
	std::string IP = Address.ToString();
	//192.168.1.102

	//init listener
	listening = _listener.Listen(2435);

	if (listening)
	{
		//add selector
		_selector.Add(_listener);
	}

	clientConnected = false;
	clientMessage = "";
}

void ServerTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void ServerTest::CleanUp()
{
	
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
	if(clientMessage == "Left")
	{
		cam->RotateView(2.0f * dt,0,1,0);
	}
	if(clientMessage == "Right")
	{
		cam->RotateView(-(2.0f * dt),0,1,0);
	}
	if(clientMessage == "Up")
	{
		cam->PitchView(2.0f * dt);
	}
	if(clientMessage == "Down")
	{
		cam->PitchView(-(2.0f * dt));
	}

	//move
	if(clientMessage == "W")
	{
		cam->Move(6.0f * dt);
	}
	if(clientMessage == "S")
	{
		cam->Move(-(6.0f * dt));
	}
	if(clientMessage == "A")
	{
		cam->Strafe(-(6.0f * dt));
	}
	if(clientMessage == "D")
	{
		cam->Strafe(6.0f * dt);
	}
}
void ServerTest::Update(GameManager* sManager)
{
	//network shit
	if (listening)
	{
		unsigned int NbSockets = _selector.Wait(0.010f);

		// We can read from each returned socket
		for (unsigned int i = 0; i < NbSockets; ++i)
		{
			// Get the current socket
			Network::SocketTCP readySocket = _selector.GetSocketReady(i);

			if (readySocket == _listener)//meaning if our main socket have some connection?
			{
				//add new client
				Network::IPAddress clientAddress;
				Network::SocketTCP clientSocket;
				_listener.Accept(clientSocket, &clientAddress);

				clientConnected = true;

				// Add it to the selector
				_selector.Add(clientSocket);
			}else
			{
				Network::Packet newPacket;
				if (readySocket.Receive(newPacket) == Network::Socket::Done)
				{
					// Extract the message and display it
					newPacket >> clientMessage;
				}
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

	//draw client message
	if (clientConnected)
	{
		if (clientMessage != "")
		{
			RenderManager::Instance()->drawText(font,1,30,clientMessage.c_str(),Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
		}
	}

	RenderManager::Instance()->EndFrame();
}

