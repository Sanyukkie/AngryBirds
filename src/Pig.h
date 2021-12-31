#pragma once

#include <iostream>
#include <Box2d/box2d.h>
#include "Object.h"

class Pig : public Object
{
public:
    Pig(b2World* world, Object::Type type, float x, float y, float radius, float density = 1.0f, float energy = 40) : Object(world, type, x, y, radius, radius, energy)
    {
        b2CircleShape shape;
        shape.m_radius = radius;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = density;
        fixtureDef.restitution = 0.4f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);
    }
    virtual ~Pig() {}

};

