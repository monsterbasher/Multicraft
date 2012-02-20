#ifndef ENTITY_H
#define ENTITY_H

#include <Aurora/Math/Vector2.h>
#include <Aurora/Math/BoundingBox2D.h>
#include <Aurora/Graphics/RenderManager.h>

using namespace Aurora::Math;
using namespace Aurora::Graphics;

class Entity
{
protected:

	Vector2 _position;
	Vector2 _size;

	BoundingBox2D _bBox;

private:

	void _updateAABB();

public:

	Entity();

	virtual void SetPosition(float x,float y);
	virtual void SetSize(float width,float height);

	Vector2 GetSize() { return _size; }
	Vector2 GetPosition() { return _position; }

	virtual void Draw(RenderManager *rm);
	
};

#endif
