#include "NetworkControllerClient.h"

void NetworkControllerClient::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	dt = 0.0f;

	Network::NetworkManager::Instance()->Init();
	//controller init
#ifdef AURORA_PC
	networkInput = new Network::NetworkInputControllerClient("Client_PC",2634);
#endif

#ifdef AURORA_PSP
	networkInput = new Network::NetworkInputControllerClient("Client_PSP",2634);
#endif

#ifdef AURORA_IOS
	networkInput = new Network::NetworkInputControllerClient("Client_IOS",2634);
#endif

	_address = Network::IPAddress::GetLocalAddress();

	networkInput->Start();

	serverName = "";
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
}

void NetworkControllerClient::Update(GameManager* sManager)
{
	networkInput->Update(dt);

	//for test purpose we connect to the first found server
	if (networkInput->GetControllerState() == Network::SEARCHING && networkInput->NumberOfFoundServers() > 0)
	{
		serverName = networkInput->GetServerNameAtPos(0);
		networkInput->ConnectToServer(serverName);
	}

	//delta time
	dt = _clock.getTime();
	_clock.Reset();	
}
void NetworkControllerClient::Draw(GameManager* sManager)
{
	RenderManager::Instance()->StartFrame();
	RenderManager::Instance()->ClearScreen();

	//change ortho for text
	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw client message
	if (networkInput->GetControllerState() == Network::CONNECTED)
	{
		char serverN[50];
		sprintf(serverN,"Connected to server: %s",serverName.c_str());
		RenderManager::Instance()->drawText(font,1,30,serverN,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else if (networkInput->GetControllerState() == Network::SEARCHING)
	{
		RenderManager::Instance()->drawText(font,1,30,"Searching for server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

		char serverNumber[50];
		sprintf(serverNumber,"Servers found: %d",networkInput->NumberOfFoundServers());
		RenderManager::Instance()->drawText(font,1,45,serverNumber,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else if (networkInput->GetControllerState() == Network::CONNECTING)
	{
		RenderManager::Instance()->drawText(font,1,30,"Connecting to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else if (networkInput->GetControllerState() == Network::WAITING)
	{
		RenderManager::Instance()->drawText(font,1,30,"Waiting for login to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}
	else
	{
		RenderManager::Instance()->drawText(font,1,30,"Not connected to server!",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	}

	RenderManager::Instance()->drawText(font,1,60,_address.ToString().c_str(),Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));
	

	RenderManager::Instance()->EndFrame();
}

