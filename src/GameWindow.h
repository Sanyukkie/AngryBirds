#pragma once

const int WINDOW_W = 1280;
const int WINDOW_H = 720;

#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include "Map.h"

class GameWindow {
private:
	sf::RenderWindow window;
	sf::View view;

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	sf::CircleShape catapult;
	Map* game_world;
	float catapult_x;
	float catapult_y;
	int shoot_aiming;							// Mouse pos. needed for cannon
	//###############################

	//General window parameters
	sf::RectangleShape background;
	sf::RectangleShape backButton;
	std::string currentFile;

	//Start Menu
	sf::Texture buttonTexture;
	sf::RectangleShape playButton;
	sf::RectangleShape exitButton;

	//Level menu
	sf::RectangleShape level_1_SquareShape;
	sf::RectangleShape level_2_SquareShape;
	sf::RectangleShape level_3_SquareShape;

	//Game buttons
	sf::RectangleShape replayButton;

	enum class gamePhase
	{
		Menu,
		Levl_Bar,
		Game
	};
	gamePhase phase = gamePhase::Menu;


public:

	~GameWindow();
	GameWindow();

	
	void run();

	void creatBackgraund(std::string image);
	sf::RectangleShape& setButton(sf::RectangleShape& buttonShape, sf::Vector2f position, std::string image);
	
	void pollMenuEvents();

	void runMenu();

	void runLevelsMenu();
	void drawBird(Object* bird);
	void drawBox(Object* box);
	void drawPig(Object* pig);
	void drawGameStatic();
	void drawGameDynamicObjects();
	void runGame();

	void pollGameEvents();
};
