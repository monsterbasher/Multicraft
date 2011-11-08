#include "ExampleGame.h"


void ExampleState::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	mojang = TextureManager::Instance()->loadImageFromFile("Assets/Minecraft/title/mojang.png");
	itemImage = TextureManager::Instance()->loadImageFromFile("Assets/Minecraft/gui/items.png");

	spriteX = spriteY = 50;
	itemSprite = new Sprite("Assets/Minecraft/gui/items.png",0,0,16,16);
	itemSprite->Scale(3.0f,3.0f);
	itemSprite->SetPosition(spriteX,spriteY);

	sprite3d = new Sprite3D("Assets/Minecraft/gui/items.png",16*3,16*4,16*4,16*5);
	sprite3d->setPosition(0,0,-20);

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

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

	if(_systemManager->isPlatformPC())
	{
		spriteX = mouseX;
		spriteY = mouseY;
	}else if(_systemManager->isPlatformPSP())
	{
		//if(analogX > 0.3f || analogX < -0.3f)
		//	spriteX += (analogX * 100) * dt;

		//if(analogY > 0.3f || analogY < -0.3f)
		//	spriteY += (analogY * 100) * dt;
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

	RenderManager::Instance()->setZminMax(0.1f,100.0f);
	RenderManager::Instance()->SetPerspective();

	RenderManager::Instance()->drawSprite3D(sprite3d);

	RenderManager::Instance()->setZminMax(-10,10);
	RenderManager::Instance()->SetOrtho();

	//RenderManager::Instance()->drawImage(mojang,0,0,480,272);
	RenderManager::Instance()->drawSprite(itemSprite);

	RenderManager::Instance()->SetTextOrtho();
	RenderManager::Instance()->drawText(font,1,267,"Multicraft",Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));


	char controlPos[30];
	if(_systemManager->isPlatformPC())
		sprintf(controlPos,"x: %d y: %d",mouseX,mouseY);
	else if(_systemManager->isPlatformPSP())
		sprintf(controlPos,"x: %f y: %f",analogX,analogY);
	RenderManager::Instance()->drawText(font,1,12,controlPos,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	char deltaTime[30];
	sprintf(deltaTime,"dt: %f",dt);
	RenderManager::Instance()->drawText(font,1,27,deltaTime,Aurora::Graphics::ALIGN_LEFT,Aurora::Graphics::RenderManager::RGBA(0xff, 0xff, 0xff, 0xff));

	RenderManager::Instance()->EndFrame();
}

void ExampleGameManager::Configure()
{
	//init render manager properties
	RenderManager::Instance()->setSesize(480,272);
	RenderManager::Instance()->setZminMax(-10,10);
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
	delete exampleState;
}

