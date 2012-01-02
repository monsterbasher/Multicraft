#include "ExampleGame.h"
#include <stdio.h>

#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Image.h>

void ExampleState::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	itemImage = TextureManager::Instance()->loadImageFromFile("Assets/Minecraft/gui/items.png");
	renderToImage = TextureManager::Instance()->createEmptyImage("renderToImage",256,256,Vram);

	spriteX = spriteY = 50;
	itemSprite = new Sprite("Assets/Minecraft/gui/items.png",0,0,16,16);
	itemSprite->Scale(3.0f,3.0f);
	itemSprite->SetPosition(spriteX,spriteY);

	sprite3d = new Sprite3D("Assets/Minecraft/gui/items.png",16*3,16*4,16*4,16*5);
	sprite3d->setPosition(0,0,-2.5f);
	sprite3d->setscale(0.1f,0.1f,0.1f);

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	cam = new Camera();
	cam->PositionCamera(0,0,0,0,0,-5,0,1,0);

	_renderManager->setCurrentCam(cam);

	mouseX = mouseY = 0;
	analogX = analogY = 0.0f;

	dt = 0.0f;
}

void ExampleState::Enter()
{
	RenderManager::Instance()->SetOrtho();
	_clock.Reset();
}

void ExampleState::CleanUp()
{
	delete itemSprite;
	delete sprite3d;
	//delete font;
}

void ExampleState::Pause()
{

}

void ExampleState::Resume()
{

}

void ExampleState::HandleEvents(GameManager* sManager)
{
	_systemManager->Update();

	mouseX = _systemManager->getMouseX();
	mouseY = _systemManager->getMouseY();

	analogX = _systemManager->getAnalogX();
	analogY = _systemManager->getAlanogY();

	if(_systemManager->platformUseMouse())
	{
		spriteX = mouseX;
		spriteY = mouseY;
	}else if(_systemManager->platformUseJoypad())
	{
		//if(analogX > 0.3f || analogX < -0.3f)
		//	spriteX += (analogX * 100) * dt;

		//if(analogY > 0.3f || analogY < -0.3f)
		//	spriteY += (analogY * 100) * dt;
	}

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

	itemSprite->SetPosition(spriteX,spriteY);
	sprite3d->rotationY+=(50.0f * dt);
}
void ExampleState::Update(GameManager* sManager)
{
	dt = _clock.getTime();
	_clock.Reset();
}
void ExampleState::Draw(GameManager* sManager)
{
	RenderManager::Instance()->StartFrame();

	RenderManager::Instance()->StartRenderToTexture(renderToImage);
	RenderManager::Instance()->SetPerspective(53,renderToImage->_width/renderToImage->_height,0.1f,256.0f);
	RenderManager::Instance()->ClearScreen();

	RenderManager::Instance()->drawSprite3D(sprite3d);

	RenderManager::Instance()->EndRenderToTexture(renderToImage);

	RenderManager::Instance()->RenderToScreen();
	RenderManager::Instance()->ClearScreen();

	RenderManager::Instance()->UpdateCurrentCamera();

	//draw 3d model
	//RenderManager::Instance()->drawSprite3D(sprite3d);
	RenderManager::Instance()->DrawCubeTextured(renderToImage,Vector3(0,0,-3),Vector3(1,1,1),Vector3(0,0,0));

	//change back to ortho
	RenderManager::Instance()->SetOrtho();

	//draw single image 
	RenderManager::Instance()->drawSprite(itemSprite);

	//change ortho for text
	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw mouse position
	char controlPos[30];
	_systemManager->platformUseMouse() == true ? sprintf(controlPos,"x: %d y: %d",mouseX,mouseY) : sprintf(controlPos,"x: %f y: %f",analogX,analogY);
	RenderManager::Instance()->drawText(font,1,12,controlPos,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	//draw fps
	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,27,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	RenderManager::Instance()->EndFrame();
}

void ExampleGameManager::Configure()
{
	//init render manager properties
	RenderManager::Instance()->setSesize(480,272);
}

void ExampleGameManager::Init()
{
	//init whatever you need
	exampleState = new ExampleState();
	exampleState->Init();

	ChangeState(exampleState);
}

void ExampleGameManager::CleanUp()
{
	exampleState->CleanUp();
	delete exampleState;
}

