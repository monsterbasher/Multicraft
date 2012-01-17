#include "NetworkControllerServer.h"

void NetworkControllerServer::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	dt = 0.0f;

	Network::NetworkManager::Instance()->Init();

	_inputServer = new Network::NetworkInputControllerServer("TestServer1",2634);
	_inputServer->Start();
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
}
void NetworkControllerServer::Update(GameManager* sManager)
{
	_inputServer->Update(dt);
	
	//delta time
	dt = _clock.getTime();
	_clock.Reset();
}
void NetworkControllerServer::Draw(GameManager* sManager)
{
	//start frame
	RenderManager::Instance()->StartFrame();
	RenderManager::Instance()->ClearScreen();

	//change ortho for text
	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//clients number
	char clinetsNumber[40];
	sprintf(clinetsNumber,"Clients number: %d",_inputServer->GetClientsCount());
	RenderManager::Instance()->drawText(font,1,30,clinetsNumber,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//name of each clients
	int startPos = 45;
	for(int i = 0; i < _inputServer->GetClientsCount(); i++)
	{
		if (_inputServer->GetClientByNumber(i)->IsLogged())
		{
			char clinetsInfo[50];
			sprintf(clinetsInfo,"Client %d with name: %s",i,_inputServer->GetClientByNumber(i)->GetName().c_str());
			RenderManager::Instance()->drawText(font,1,startPos,clinetsInfo,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

			startPos += 15;
		}		
	}	

	RenderManager::Instance()->EndFrame();
}

