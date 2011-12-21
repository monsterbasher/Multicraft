#include "ExampleGame.h"
#include <stdio.h>

#include <Aurora/Utils/ScriptManager.h>
#include <Aurora/Graphics/Sprite.h>

void ExampleState::Init()
{
	_renderManager = RenderManager::Instance();
	_systemManager = SystemManager::Instance();

	itemImage = TextureManager::Instance()->loadImageFromFile("Assets/Minecraft/gui/items.png");

	spriteX = spriteY = 50;
	itemSprite = new Sprite("Assets/Minecraft/gui/items.png",0,0,16,16);
	//itemSprite->Scale(3.0f,3.0f);
	//itemSprite->SetPosition(spriteX,spriteY);

	sprite3d = new Sprite3D("Assets/Minecraft/gui/items.png",16*3,16*4,16*4,16*5);
	sprite3d->setPosition(0,0,-20);

	font = new TrueTypeFont("Assets/Minecraft/font.ttf",16);

	mouseX = mouseY = 0;
	analogX = analogY = 0.0f;

	dt = 0.0f;

	lua_State* _luaState = lua_open();
	luabind::open(_luaState);

	luabind::module(_luaState)
		[
			luabind::class_<Aurora::Graphics::Sprite>("Sprite")
			.def(luabind::constructor<const char*,int,int,int,int>())
			.def("setScale", &Aurora::Graphics::Sprite::Scale)
			.def("setPosition", &Aurora::Graphics::Sprite::SetPosition)
		];

	luabind::globals(_luaState)["itemSprite"] = itemSprite;

	luaL_dostring(
		_luaState,
		"itemSprite:setScale(3,3)\n"
		"itemSprite:setPosition(200,200)\n"
		);

	lua_close(_luaState);
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

	//set depth min and max for perspective
	RenderManager::Instance()->setZminMax(0.1f,100.0f);
	RenderManager::Instance()->SetPerspective();

	//draw 3d model
	RenderManager::Instance()->drawSprite3D(sprite3d);

	//change back to ortho
	RenderManager::Instance()->setZminMax(-10,10);
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
	exampleState->CleanUp();
	delete exampleState;
}

