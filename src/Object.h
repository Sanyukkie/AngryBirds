#pragma once

#include <iostream>
#include <Box2D/Box2D.h>
#include "Map.h"

class Map;
class Object;

class objectData {
public:
	float energy;
	float fullEnergy;
	bool hasEnergy;
	bool timerEnabled;
	Object* object;
};

class Object
{
public:
	enum class Type
	{
		BasicBird	= 10,
		NoBird	= 19,

		WoodBox = 20,

		Pig		= 30

	};

	Object(b2World* world, Object::Type type, float x, float y, float w, float h, float energy = 100.f, float time = 180 + std::rand() % 20 * 15, bool dynamic = true) : type(type), width(w), height(h), timer(time) {
		b2BodyDef bodyDef;
		objectData* data = new objectData;
		data->energy = energy;
		data->fullEnergy = energy;
		data->hasEnergy = energy ? 1 : 0;
		data->timerEnabled = time ? 1 : 0;
		data->object = this;
		
		if (dynamic)
			bodyDef.type = b2_dynamicBody;
		else
			bodyDef.type = b2_staticBody;
		bodyDef.bullet = true;
		bodyDef.position.Set(x, y);
		bodyDef.angle = 0.0f;
		body = world->CreateBody(&bodyDef);

		body->SetUserData(data);
	}

	virtual ~Object()
	{
		delete reinterpret_cast<objectData*>(body->GetUserData());
		body->GetWorld()->DestroyBody(body);
	}

	virtual bool action(Map*) { return true; }
	
	Object& operator=(const Object&) = delete;
	Object(const Object&) = delete;

	void setImpulse(float x, float y) {
		body->ApplyLinearImpulse(b2Vec2(x, y), body->GetWorldCenter(), 1);
	}

	void destroy(Map* map) {
		b2World* world = body->GetWorld();
		float x = getX();
		float y = getY();
		b2Vec2 v = body->GetLinearVelocity();
		body->SetAwake(false);
	}

	b2Body* getBody() const {
		return body;
	}

	Object::Type getType() const {
		return this->type;
	}

	float getMass() const {
		return body->GetMass();
	}

	float getX() const {
		return body->GetPosition().x;
	}

	float getY() const {
		return body->GetPosition().y;
	}

	float getAngle() const {
		return body->GetAngle();
	}

	virtual float getWidth() const {
		return width;
	}

	virtual float getHeight() const {
		return height;
	}

	void setRadius(float r) {
		width = r;
		height = r;
	}

	float getEnergy() const {
		objectData* data = reinterpret_cast<objectData*>(body->GetUserData());
		return (data->hasEnergy && data->fullEnergy != 0) ? data->energy / data->fullEnergy : 1;
	}

	//Public variable to store path to the image of an object
	const std::string imagePath;
	friend class Map;

protected:
	b2Body* body = nullptr;
	float width, height, timer;
	Object::Type type;
	b2Vec2 velocity;

};

