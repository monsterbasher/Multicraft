#include "FrustumTest.h"


void FrustumTest::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	_model = new ModelObj();
	_model->LoadObj("Assets/Models/Obj/sphere.obj");

	_camera = new Camera();
	_camera->PositionCamera(0,0,0,0,0,-5,0,1,0);
	_renderManager->setCurrentCam(_camera);

	dt = 0.0f;
}

void FrustumTest::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void FrustumTest::CleanUp()
{
	//delete font;
}

void FrustumTest::Pause()
{

}

void FrustumTest::Resume()
{

}

void FrustumTest::HandleEvents(GameManager* sManager)
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

void FrustumTest::Update(GameManager* sManager)
{
	//delta time
	dt = _clock.getTime();
	_clock.Reset();
}
void FrustumTest::Draw(GameManager* sManager)
{
	RenderManager::Instance()->StartFrame();
	RenderManager::Instance()->ClearScreen();

	RenderManager::Instance()->SetPerspective();
	RenderManager::Instance()->UpdateCurrentCamera();

	//draw 3d model
	RenderManager::Instance()->DrawModelObj(_model,Vector3(0,0,-3),Vector3(1,1,1),Vector3(0,0,0));

	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0x00, 0x00, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,13,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0x00, 0xff, 0x00, 0xff));

	RenderManager::Instance()->EndFrame();
}

