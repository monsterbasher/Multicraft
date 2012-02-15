#include "SimpleTest.h"

void SimpleTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	dt = 0.0f;
}

void SimpleTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void SimpleTest::CleanUp()
{
//delete font;
}

void SimpleTest::Pause()
{

}

void SimpleTest::Resume()
{

}

void SimpleTest::HandleEvents(GameManager* sManager)
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

void SimpleTest::Update(GameManager* sManager)
{
	//delta time
	dt = _clock.getTime();
	_clock.Reset();
}
void SimpleTest::Draw(GameManager* sManager)
{
	RenderManager::Instance()->StartFrame();
	RenderManager::Instance()->ClearScreen();

	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	RenderManager::Instance()->EndFrame();
}

