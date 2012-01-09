#include "Demo_ObjLoading.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>

void Demo_ObjLoading::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	//load model
	objModel  = new ModelObj();
	//objModel->LoadObj("Assets/Models/Obj/companion_cube/companion_cube.obj");
	//objModel->LoadObj("Assets/Models/Obj/apc_obj/apc.obj");
	//objModel->LoadObj("Assets/Models/Obj/tanker_obj/tanker.obj");
	objModel->LoadObj("Assets/Models/Obj/portal_gun/portal_gun.obj");
	//objModel->LoadObj("Assets/Models/Obj/room1/room1.obj");

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	cam = new Camera();
	cam->PositionCamera(0,0,0,0,0,-5,0,1,0);

	_renderManager->setCurrentCam(cam);

	dt = 0.0f;
}

void Demo_ObjLoading::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void Demo_ObjLoading::CleanUp()
{
	//delete font;
}

void Demo_ObjLoading::Pause()
{

}

void Demo_ObjLoading::Resume()
{

}

void Demo_ObjLoading::HandleEvents(GameManager* sManager)
{
	_systemManager->Update();

	//camera
	//rotate
	if(_systemManager->keyHold(Key::Left))
	{
		cam->RotateView(2.0f * dt,0,1,0);
	}
	if(_systemManager->keyHold(Key::Right))
	{
		cam->RotateView(-(2.0f * dt),0,1,0);
	}
	if(_systemManager->keyHold(Key::Up))
	{
		cam->PitchView(2.0f * dt);
	}
	if(_systemManager->keyHold(Key::Down))
	{
		cam->PitchView(-(2.0f * dt));
	}

	//move
	if(_systemManager->keyHold(Key::W))
	{
		cam->Move(6.0f * dt);
	}
	if(_systemManager->keyHold(Key::S))
	{
		cam->Move(-(6.0f * dt));
	}
	if(_systemManager->keyHold(Key::A))
	{
		cam->Strafe(-(6.0f * dt));
	}
	if(_systemManager->keyHold(Key::D))
	{
		cam->Strafe(6.0f * dt);
	}
}
void Demo_ObjLoading::Update(GameManager* sManager)
{
	dt = _clock.getTime();
	_clock.Reset();
}
void Demo_ObjLoading::Draw(GameManager* sManager)
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

	RenderManager::Instance()->EndFrame();
}
