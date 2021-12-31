#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>

#include "Map.h"

Map::Map(std::string file)
{
	//Set catapult coordinates
	catapult_x = 6.4f; catapult_y = -18.0f;

	//Creating a world
	m_world = new b2World({ 0.0f * SCALE, -9.81f * SCALE });
	m_world->SetContactListener(this);
	m_world->SetContactFilter(this);

	//Creating statick line (graund) so the blocks dont fall
	b2BodyDef bodyDef;
	bodyDef.type = b2BodyType::b2_staticBody;
	bodyDef.position.Set(0.0f, -22.5f);
	b2Body* body = m_world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(50.0f, 2.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	body->CreateFixture(&shape, 0.0f);


	//Creating the holder for the catapult


	b2BodyDef catapultDef;
	catapultDef.type = b2BodyType::b2_staticBody;
	catapultDef.position.Set(6.5f, -19.5f);
	catapultBody = m_world->CreateBody(&catapultDef);
	b2PolygonShape catapultShape;
	catapultShape.SetAsBox(0.2f, 1.0f);
	fixtureDef.shape = &catapultShape;
	catapultBody->CreateFixture(&catapultShape, 0.0f);
	
	//##### Load file #####
	this->loadMap(file); // SET LEVEL PATH AS PARAMETER

}

Map::~Map()
{
	for (auto i : birds) {
		delete i;
	}
	for (auto i : boxes) {
		delete i;
	}
	for (auto i : pigs) {
		delete i;
	}
	delete m_world;
	m_world = nullptr;
}

void Map::loadMap(std::string textfilename)
{
	using namespace std;
	ifstream file(textfilename);
	string line;
	b2PolygonShape shape;
	int id;
	float width, height, x, y;
	while (getline(file, line)) {
		std::istringstream some_stream(line);
		some_stream >> id >> width >> height >> x >> y;
		if (id == 10) { // BIRD
			birds.push_back(new Bird(m_world, Object::Type::BasicBird, catapult_x, catapult_y));
		}
		else if (id == 20) { //BOX
			boxes.push_back(new Box(m_world, Object::Type::WoodBox, x, y, width, height));
		}
		else if (id == 30) { // PIG
			pigs.push_back(new Pig(m_world, Object::Type::Pig, x, y, 0.5f * width));
			this->enemiesLeft++;
		}
	}
}

void Map::step()
{
	m_world->Step(1.0f / 60.0f, 10, 10);
	m_world->ClearForces();
	
	this->removeNoEnergyObjects(this->birds);
	this->removeNoEnergyObjects(this->boxes);
	this->removeNoEnergyObjects(this->pigs);
	
}

void Map::PreSolve(b2Contact* contact, const b2Manifold*)
{
	objectData* objectDataA = reinterpret_cast<objectData*>(contact->GetFixtureA()->GetBody()->GetUserData());
	objectData* objectDataB = reinterpret_cast<objectData*>(contact->GetFixtureB()->GetBody()->GetUserData());
	if (objectDataA)
		objectDataA->object->velocity = contact->GetFixtureA()->GetBody()->GetLinearVelocity();
	if (objectDataB)
		objectDataB->object->velocity = contact->GetFixtureB()->GetBody()->GetLinearVelocity();
}

void Map::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	if (impulse->normalImpulses[0] > 1)
	{
		float maxImpulse = 0.0f;
		for (int i = 0; i < contact->GetManifold()->pointCount; ++i)
		{
			maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
		}
		//If FixtureA has energy, calculate lost energy according to impulse strength and add that to totalscore
		objectData* objectDataA = reinterpret_cast<objectData*>(contact->GetFixtureA()->GetBody()->GetUserData());
		objectData* objectDataB = reinterpret_cast<objectData*>(contact->GetFixtureB()->GetBody()->GetUserData());
		float deltaEnergy = 0;
		if (objectDataA && objectDataA->hasEnergy)
		{
			deltaEnergy = maxImpulse * 0.1;
			objectDataA->energy = objectDataA->energy > deltaEnergy ? objectDataA->energy - deltaEnergy : 0;
			if (objectDataB && !objectDataA->energy)
				contact->GetFixtureB()->GetBody()->SetLinearVelocity(objectDataB->object->velocity);
		}
		else if (objectDataA)
		{
			objectDataA->object->timer = 200 + std::rand() % 10 * 30;
		}

		totalScore += deltaEnergy;

		deltaEnergy = 0;
		//If FixtureB has energy, calculate lost energy according to impulse strength and add that to totalscore
		if (objectDataB && objectDataB->hasEnergy)
		{
			deltaEnergy = maxImpulse * 0.1;
			objectDataB->energy = objectDataB->energy > deltaEnergy ? objectDataB->energy - deltaEnergy : 0;
			if (objectDataA && !objectDataB->energy)
				contact->GetFixtureA()->GetBody()->SetLinearVelocity(objectDataA->object->velocity);
		}
		else if (objectDataB)
		{
			objectDataB->object->timer = 200 + std::rand() % 10 * 30;
		}
		totalScore += deltaEnergy;
	}
}

void Map::removeNoEnergyObjects(std::vector<Object*> objects)
{
	for (auto i = objects.begin(); i != objects.end();)
	{
		if ((*i)->getType() == Object::Type::Pig)
			this->enemiesLeft++;
		
		objectData* data = reinterpret_cast<objectData*>((*i)->body->GetUserData());

		if (((data->hasEnergy && data->energy <= 0) || (data->timerEnabled && (*i)->timer == 0))) {
			// (*i)->getBody()->SetActive(false);
			(*i)->destroy(this);
			if (*i == getCurrentBird())
				killCurrentBird();
			
			switch ((*i)->getType())
			{
			case(Object::Type::BasicBird):
				delete* i;
				this->removeBird(*i);
				break;
			case(Object::Type::Pig):
				delete* i;
				this->removePig(*i);
				break;
			case(Object::Type::WoodBox):
				delete* i;
				this->removeBox(*i);
				break;
			}
			
		}
		else if (data && !data->hasEnergy)
			--(*i)->timer;
		++i;
	}
}

void Map::ShootBird(float x, float y)
{
	if (birds.empty())
	{
		return;
	}
	currentBird = birds.back();
	currentBird->getBody()->SetActive(true);
	currentBird->setImpulse(x, y);
	birds.push_back(currentBird);
	birds.pop_back();
}

void Map::removeBird(Object* bird)
{
	auto it = std::remove(birds.begin(), birds.end(), bird);
	birds.erase(it);
}

void Map::removeBox(Object* box)
{
	auto it = std::remove(boxes.begin(), boxes.end(), box);
	boxes.erase(it);
}

void Map::removePig(Object* pig)
{
	auto it = std::remove(pigs.begin(), pigs.end(), pig);
	pigs.erase(it);
}

Object::Type Map::getNextBirdType() const
{
	if (this->birds.empty())
		return Object::Type::NoBird;
	return birds.back()->getType();
}
