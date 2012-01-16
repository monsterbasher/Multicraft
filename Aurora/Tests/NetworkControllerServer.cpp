#include "NetworkControllerServer.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Network/NetworkManager.h>

void NetworkControllerServer::Init()
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

	//set broadcast data
	//_broadcadtAddress = Network::IPAddress::GetLocalAddress();
	_broadcadtAddress = Network::IPAddress("192.168.1.255");//last 255 to broadcast to whole sub network

	_broadcastTimer = 0.0f;
	_boradcastPort = 2634;

	//init listener
	clientMessage = "";
}

void NetworkControllerServer::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void NetworkControllerServer::CleanUp()
{
	
}

void NetworkControllerServer::Pause()
{

}

void NetworkControllerServer::Resume()
{

}

void NetworkControllerServer::HandleEvents(GameManager* sManager)
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
void NetworkControllerServer::Update(GameManager* sManager)
{
	//broadcasting
	_broadcastTimer += dt;
	if (_broadcastTimer > 2)
	{
		Network::Packet sendPacket;
		std::string serverName = "Drakon1";
		sendPacket << 20 << serverName;

		if(_broadcastSocket.Send(sendPacket,_broadcadtAddress,_boradcastPort) != Network::Socket::Done)
		{
			//error
			int errorSending = 1;
		}
		_broadcastTimer = 0.0f;
	}
	
	//delta time
	dt = _clock.getTime();
	_clock.Reset();
}
void NetworkControllerServer::Draw(GameManager* sManager)
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
	

	RenderManager::Instance()->EndFrame();
}

