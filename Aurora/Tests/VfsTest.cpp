#include "VfsTest.h"

#include <Aurora/System/VFSPack.h>

void VfsTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	dt = 0.0f;

	//vfs pack test
	VFSPack _packedFiles;

	_packedFiles.CreateNewPack("pack.bin");
	_packedFiles.SetEncryptKey("1qaz2wsx3edc4rfv");
	_packedFiles.AddFile("Assets/Minecraft/gui/items.png",true,true);
	_packedFiles.AddFile("Assets/Minecraft/font.ttf",true,true);
	_packedFiles.SavePack();

	_packedFiles.LoadPack("pack.bin");

	VFSFile myFile = _packedFiles.GetData("Assets/Minecraft/gui/items.png");
	myFile.SaveToDisk("items.png");
}

void VfsTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void VfsTest::CleanUp()
{
//delete font;
}

void VfsTest::Pause()
{

}

void VfsTest::Resume()
{

}

void VfsTest::HandleEvents(GameManager* sManager)
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

void VfsTest::Update(GameManager* sManager)
{
	//delta time
	dt = _clock.getTime();
	_clock.Reset();
}
void VfsTest::Draw(GameManager* sManager)
{
	RenderManager::Instance()->StartFrame();
	RenderManager::Instance()->ClearScreen();

	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0x00, 0x00, 0x00, 0xff));

	RenderManager::Instance()->EndFrame();
}

