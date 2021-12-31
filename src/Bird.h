#pragma once

#include <iostream>

#include <Box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "Object.h"


class Bird;

class Bird : public Object
{
protected:
    int abilities_amount;

public:
    Bird(b2World* world, Object::Type type, float x, float y, int actions = 0, float radius = 0.5f) : Object(world, type, x, y, radius, radius, 0), abilities_amount(actions)
    {
        b2CircleShape shape;
        shape.m_radius = radius;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.restitution = 0.4f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);

    }
    virtual ~Bird() {}

    virtual bool action(Map*)
    {
        return(true);
    }

    virtual bool ability_is_avaible(Map*)
    {
        if (abilities_amount > 0)
        {
            abilities_amount--;
            return (true);
        }
        return (false);
    }
};
