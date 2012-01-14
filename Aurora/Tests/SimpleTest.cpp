#include "SimpleTest.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Graphics/TextureManager.h>

void SimpleTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	itemImage = TextureManager::Instance()->loadImageFromFile("Assets/Minecraft/gui/items.png");
    
    itemSprite = new Sprite("Assets/Minecraft/gui/items.png",0,0,16,16);
	itemSprite->Scale(3.0f,3.0f);
	itemSprite->SetPosition(100,100);

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	cam = new Camera();
	cam->PositionCamera(0,0,0,0,0,-5,0,1,0);

	_renderManager->setCurrentCam(cam);

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

	//change ortho for text
	RenderManager::Instance()->SetOrtho();
    
    RenderManager::Instance()->drawSprite(itemSprite);
    
	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	RenderManager::Instance()->EndFrame();
}

