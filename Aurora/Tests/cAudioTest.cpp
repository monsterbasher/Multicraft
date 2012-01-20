#include "cAudioTest.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>
#include <Aurora/Graphics/TextureManager.h>

void cAudioTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	manager = cAudio::createAudioManager(true);
	mysound = manager->create("music","Assets/Audio/cAudioTheme2.ogg",true);

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	dt = 0.0f;

	if(mysound)
	{
		//Play our source in 2D once. 
		mysound->play2d(false);
	}
}

void cAudioTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void cAudioTest::CleanUp()
{
//delete font;
}

void cAudioTest::Pause()
{

}

void cAudioTest::Resume()
{

}

void cAudioTest::HandleEvents(GameManager* sManager)
{
	_systemManager->Update();
}

void cAudioTest::Update(GameManager* sManager)
{
	//delta time
	dt = _clock.getTime();
	_clock.Reset();
}
void cAudioTest::Draw(GameManager* sManager)
{
	RenderManager::Instance()->StartFrame();

	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	RenderManager::Instance()->EndFrame();
}

