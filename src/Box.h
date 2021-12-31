#pragma once

#include <iostream>
#include <Box2d/box2d.h>
#include "Object.h"
#include "Map.h"

class Box : public Object
{

public:
	Box(b2World* world, Object::Type type, float x, float y, float box_width, float box_height, float energy = 0) : Object(world, type, x, y, box_width, box_height, energy) {

		b2PolygonShape boxShape;
		boxShape.SetAsBox(box_width, box_height);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.density = 0.6f;
		fixtureDef.friction = 0.4f;

		this->body->CreateFixture(&fixtureDef);
	}
};