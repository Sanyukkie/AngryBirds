#pragma once

#include <iostream>
#include <vector>
#include <Box2D/Box2D.h>

#include "Object.h"
#include "Box.h"
#include "Bird.h"
#include "Pig.h"

const float SCALE = 30.f;
const float DEG = 57.29577f;

class Map : public b2ContactListener, b2ContactFilter
{
private:
	b2World* m_world;					//Map world
	b2Body* catapultBody;
	float catapult_x, catapult_y;		//Catapult coordinates (birds start position) (box2d coordinats)
	
	std::vector<Object*> birds;
	std::vector<Object*> boxes;
	std::vector<Object*> pigs;

	Object* currentBird = nullptr;

	int highScore = 0;
	float totalScore = 0;
	int enemiesLeft = 0;

private:
	void loadMap(std::string level_file);

public:
	Map(std::string file);
	~Map();

	void step();

	void PreSolve(b2Contact* contact, const b2Manifold*);

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void removeNoEnergyObjects(std::vector<Object*> objects);

	void ShootBird(float x, float y);

	void removeBird(Object* bird);
	void removeBox(Object* box);
	void removePig(Object* pig);

	b2Body* getCatapultBody() const { return catapultBody; }
	float getCatapultX() const { return catapult_x; }
	float getCatapultY() const { return catapult_y; }
	
	//Return player score
	int getScore() const { return totalScore; }
	
	//Return the number of enemies in the game
	int getEnemyCount() const { return enemiesLeft; }

	int getHighScore() const { return highScore; }

	bool isEnd() const {
		return ((!getBirdsLeft() || !getEnemyCount()) && currentBird == nullptr);
	}

	bool isWin() const {
		return (!getEnemyCount());
	}

	size_t getBirdsLeft() const { return this->birds.size(); }

	Object* getCurrentBird() const { return this->currentBird; }

	Object::Type getNextBirdType() const;

	//Return list of "Birds", "Boxes" or "Pigs" 
	std::vector<Object*> getBirds() const { return this->birds; }
	std::vector<Object*> getBoxes() const { return this->boxes; }
	std::vector<Object*> getPigs() const { return this->pigs; }

	void killCurrentBird() {
		if (this->getBirdsLeft() >= 2)
			currentBird = this->birds[-2];
		else
			currentBird = nullptr;
		
	}
};

