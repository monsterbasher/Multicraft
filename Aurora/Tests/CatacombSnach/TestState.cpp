#include "TestState.h"

void SnachTest::Init()
{
	_fileManager = FileManager::Instance();
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);
	dt = 0.0f;

	//game init
	_player1 = new Player();
	_player1->SetPosition(100,100);
}

void SnachTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void SnachTest::CleanUp()
{
	//delete font;
}

void SnachTest::Pause()
{

}

void SnachTest::Resume()
{

}

void SnachTest::HandleEvents(GameManager* sManager)
{
	_systemManager->Update();

	//camera
	//rotate
	if(_systemManager->keyHold(Key::Left))
	{

	}
	if(_systemManager->keyHold(Key::Right))
	{

	}
	if(_systemManager->keyHold(Key::Up))
	{

	}
	if(_systemManager->keyHold(Key::Down))
	{

	}

	//move
	if(_systemManager->keyHold(Key::W))
	{

	}
	if(_systemManager->keyHold(Key::S))
	{

	}
	if(_systemManager->keyHold(Key::A))
	{

	}
	if(_systemManager->keyHold(Key::D))
	{

	}
}

void SnachTest::Update(GameManager* sManager)
{
	//delta time
	dt = _clock.getTime();
	_clock.Reset();

	_player1->Upadte(dt);
}
void SnachTest::Draw(GameManager* sManager)
{
	RenderManager::Instance()->StartFrame();
	RenderManager::Instance()->ClearScreen();

	RenderManager::Instance()->SetOrtho();
	
	_player1->Draw(_renderManager);


	//set text ortho
	RenderManager::Instance()->SetTextOrtho();

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	RenderManager::Instance()->EndFrame();
}

