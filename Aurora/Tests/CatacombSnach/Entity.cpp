#include "Entity.h"

Entity::Entity()
{
	_position.set(0,0);
	_size.set(10,10);

	_updateAABB();
}

void Entity::SetPosition(float x,float y)
{
	_position.set(x,y);
	_updateAABB();
}

void Entity::SetSize(float width,float height)
{
	_size.set(width,height);
	_updateAABB();
}

void Entity::Draw(RenderManager* rm)
{

}

void Entity::_updateAABB()
{
	Vector2 min = _position - (_size / 2);
	Vector2 max = _position + (_size / 2);

	_bBox.Reset();
	_bBox.expandToInclude(min);
	_bBox.expandToInclude(max);
}

