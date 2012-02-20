#include "Player.h"

Player::Player()
{
	_playerName = "Lord Drak";
	_playerImage = "Assets/CatacombSnatch/lord_lard_sheet.png";

	_playerSprite = new Sprite(_playerImage.c_str(),0,0,32,32);
	_playerSprite->Scale(1.5f,1.5f);

	_prepareAnimations();
}

Player::Player(std::string playerName,std::string playerImage)
{
	_playerName = playerName;
	_playerImage = playerImage;

	_playerSprite = new Sprite(_playerImage.c_str(),0,0,32,32);

	_prepareAnimations();
}

void Player::SetPosition(float x,float y)
{
	Entity::SetPosition(x,y);

	_walkDownAnimation->SetPosition(x,y);
}

void Player::SetSize(float width,float height)
{
	Entity::SetSize(width,height);
}

void Player::Upadte(float dt)
{
	_walkDownAnimation->Update(dt);
}

void Player::Draw(RenderManager* rm)
{
	/*if (_playerSprite != 0)
	{
		rm->drawSprite(_playerSprite,_position.x,_position.y);
	}*/

	rm->drawSpriteAnimation(_walkDownAnimation);
}

void Player::_prepareAnimations()
{
	_walkDownAnimation = new SpriteAnimation(_playerImage);
	_walkDownAnimation->AddFrame(0,0,32,32);
	_walkDownAnimation->AddFrame(32,0,32,32);
	_walkDownAnimation->AddFrame(64,0,32,32);
	_walkDownAnimation->AddFrame(96,0,32,32);
	_walkDownAnimation->AddFrame(128,0,32,32);
	_walkDownAnimation->AddFrame(160,0,32,32);
	_walkDownAnimation->SetAnimationTime(1.0f);
	_walkDownAnimation->SetScale(3.0f,3.0f);
}