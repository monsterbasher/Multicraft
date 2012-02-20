#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/SpriteAnimation.h>
#include <string>

class Player : public Entity
{
private:

	std::string _playerName;
	std::string _playerImage;

	//main sprite
	Sprite* _playerSprite;

	//animations
	SpriteAnimation* _walkDownAnimation;

private:

	void _prepareAnimations();

public:

	Player();
	Player(std::string playerName,std::string playerImage);

	void SetPosition(float x,float y);
	void SetSize(float width,float height);

	void Upadte(float dt);
	void Draw(RenderManager* rm);

};

#endif
