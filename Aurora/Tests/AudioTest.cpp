#include "AudioTest.h"

void AudioTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();
	_audioManager = AudioManager::Instance();

	_audioManager->Init();

	if (_audioManager->SupportedFormat(OGG))
	{
		sound = new Sound("music","Assets/Audio/music.ogg",true);
	}
	else if (_audioManager->SupportedFormat(MP3))
	{
		sound = new Sound("music","Assets/Audio/music.mp3",true);
	}

	sound->Play();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	dt = 0.0f;
}

void AudioTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void AudioTest::CleanUp()
{
//delete font;
}

void AudioTest::Pause()
{

}

void AudioTest::Resume()
{

}

void AudioTest::HandleEvents(GameManager* sManager)
{
	_systemManager->Update();
}

void AudioTest::Update(GameManager* sManager)
{
	//delta time
	dt = _clock.getTime();
	_clock.Reset();
}
void AudioTest::Draw(GameManager* sManager)
{
	_renderManager->StartFrame();
	_renderManager->ClearScreen();

	_renderManager->SetTextOrtho();
	_renderManager->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	_renderManager->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	_renderManager->EndFrame();
}

