#include "GameWindow.h"

GameWindow::GameWindow() : window(sf::VideoMode(WINDOW_W, WINDOW_H), "Angry Birds Group 1", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize) {
	window.setFramerateLimit(60);
	view.reset(sf::FloatRect(0.0f, 0.0f, WINDOW_W, WINDOW_H));
	view.setCenter(sf::Vector2f(WINDOW_W / 2, WINDOW_H / 2));
	window.setView(view);

	game_world = nullptr;

	catapult_x = WINDOW_W * 0.15f;
	catapult_y = WINDOW_H * 0.75f;
}

GameWindow::~GameWindow()
{
	if (game_world != nullptr)
		delete game_world;
}

void GameWindow::run() {
	while (window.isOpen()) {

		window.clear();
		switch (phase)
		{
		case(gamePhase::Menu):
			runMenu();
			break;

		case(gamePhase::Levl_Bar):
			runLevelsMenu();
			break;

		case(gamePhase::Game):
			runGame();
			break;
		}
		window.display();
	}
}

void GameWindow::pollMenuEvents() {
	sf::Event evnt;
	while (window.pollEvent(evnt))
	{
		if (evnt.type == sf::Event::Closed)
			window.close();
		if (evnt.type == sf::Event::MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f mouse = window.mapPixelToCoords(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y));
			switch (phase)
			{
			case(gamePhase::Menu):
				if (playButton.getGlobalBounds().contains(mouse.x, mouse.y))
					phase = gamePhase::Levl_Bar;
				else if (exitButton.getGlobalBounds().contains(mouse.x, mouse.y))
					window.close();
				break;
			case(gamePhase::Levl_Bar):
				std::cout << level_1_SquareShape.getPosition().x << " " << level_1_SquareShape.getPosition().y << std::endl;
				if (backButton.getGlobalBounds().contains(mouse.x, mouse.y))
					phase = gamePhase::Menu;
				if (level_1_SquareShape.getGlobalBounds().contains(mouse.x, mouse.y)) {
					phase = gamePhase::Game;
					game_world = new Map("levels/lvl1.txt");
					currentFile = "levels/lvl1.txt";
				}
				if (level_2_SquareShape.getGlobalBounds().contains(mouse.x, mouse.y)) {
					phase = gamePhase::Game;
					game_world = new Map("levels/lvl2.txt");
					currentFile = "levels/lvl2.txt";
				}
				if (level_3_SquareShape.getGlobalBounds().contains(mouse.x, mouse.y)) {
					phase = gamePhase::Game;
					game_world = new Map("levels/lvl3.txt");
					currentFile = "levels/lvl3.txt";
				}
				break;
			}
		}
	}
}

void GameWindow::creatBackgraund(std::string image) {
	//Creating backgraund
	sf::Texture texture;
	texture.loadFromFile(image);
	texture.setSmooth(1);
	background.setSize((sf::Vector2f)view.getSize());
	background.setTexture(&texture);
	background.setPosition(view.getCenter() - (sf::Vector2f)background.getSize() / 2.0f);
	window.draw(background);
}

sf::RectangleShape& GameWindow::setButton(sf::RectangleShape& buttonShape, sf::Vector2f position, std::string image) {
	buttonTexture.loadFromFile(image);
	buttonTexture.setSmooth(1);
	buttonShape.setSize((sf::Vector2f)buttonTexture.getSize());
	buttonShape.setOrigin((sf::Vector2f)buttonTexture.getSize() / 2.0f);
	buttonShape.setTexture(&buttonTexture);
	buttonShape.setPosition(position);

	return buttonShape;
}

void GameWindow::runMenu() {

	//Drawing the backgraund
	this->creatBackgraund("images/menu.png");

	//Creating "Play" button
	window.draw(this->setButton(playButton, view.getCenter(), "images/menu_playButton.png"));
	//Creating "Exit" button
	window.draw(this->setButton(exitButton, sf::Vector2f(100, WINDOW_H - 100), "images/menu_exitButton.png"));

	pollMenuEvents();
}

void GameWindow::drawBird(Object* bird)
{
	sf::CircleShape shape(bird->getWidth() * SCALE);
	shape.setOrigin(bird->getWidth() * SCALE, bird->getHeight() * SCALE);
	sf::Texture BirdTexture;
	BirdTexture.setSmooth(1);
	switch (bird->getType())
	{
	case Object::Type::BasicBird:
		BirdTexture.loadFromFile("images/basicBird.png");
		shape.setTexture(&BirdTexture);
		break;
	case Object::Type::NoBird:
		shape.setFillColor(sf::Color(255, 255, 255));
	}
	shape.setPosition(bird->getX() * SCALE, bird->getY() * -SCALE);
	shape.setRotation(bird->getAngle() * -DEG);
	window.draw(shape);
}

void GameWindow::drawBox(Object* box)
{

	std::cout << box->getEnergy() << std::endl;
	sf::RectangleShape shape(sf::Vector2f(box->getWidth() * SCALE * 2, box->getHeight() * SCALE * 2));
	shape.setOrigin(box->getWidth() * SCALE, box->getHeight() * SCALE);
	sf::Texture BoxTexture;
	BoxTexture.setSmooth(1);
	BoxTexture.loadFromFile("images/box.png");
	shape.setTexture(&BoxTexture);
	shape.setPosition(box->getX() * SCALE, box->getY() * -SCALE);
	shape.setRotation(box->getAngle() * -DEG);
	window.draw(shape);
}

void GameWindow::drawPig(Object* pig)
{
	sf::CircleShape shape(pig->getWidth() * SCALE);
	shape.setOrigin(pig->getWidth() * SCALE, pig->getHeight() * SCALE);
	sf::Texture PigTexture;
	PigTexture.setSmooth(1);
	PigTexture.loadFromFile("images/pig.png");

	shape.setTexture(&PigTexture);
	shape.setPosition(pig->getX() * SCALE, pig->getY() * -SCALE);
	shape.setRotation(pig->getAngle() * -DEG);
	window.draw(shape);
}

void GameWindow::runLevelsMenu()
{
	//Drawing the backgraund
	this->creatBackgraund("images/menu.png");

	//Creating Levels menu
	float x = view.getCenter().x - view.getSize().x / 8.0f; // -2.0f * levelSquareButton.getSize().x;
	float y = view.getCenter().y - view.getSize().y / 8.0f; //2.0f * levelSquareButton.getSize().y;

	level_1_SquareShape.setSize(sf::Vector2f(100, 100));
	level_2_SquareShape.setSize(sf::Vector2f(100, 100));
	level_3_SquareShape.setSize(sf::Vector2f(100, 100));

	window.draw(this->setButton(level_1_SquareShape, sf::Vector2f(x + 0.0f * 60, y), "images/menu_levlButton.png"));
	window.draw(this->setButton(level_2_SquareShape, sf::Vector2f(x + 2.0f * 60, y), "images/menu_levlButton.png"));
	window.draw(this->setButton(level_3_SquareShape, sf::Vector2f(x + 4.0f * 60, y), "images/menu_levlButton.png"));
	window.draw(this->setButton(backButton, sf::Vector2f(100, WINDOW_H - 100), "images/menu_backButton.png"));

	pollMenuEvents();
}

void GameWindow::drawGameStatic()
{
	//Drawing the backgraund
	this->creatBackgraund("images/game_backgraund.png");


	// Drawing the catapult
	sf::RectangleShape catapult_bg(sf::Vector2f(50, 90));

	sf::Texture CatapultTexture;

	//catapultBody.setSize(sf::Vector2f(WINDOW_W * 0.2f, WINDOW_H * 1.0f));

	CatapultTexture.loadFromFile("images/catapult.png");
	CatapultTexture.setSmooth(1);


	catapult_bg.setTexture(&CatapultTexture);
	catapult_bg.setOrigin(16, 16);
	catapult_bg.setPosition(catapult_x - 10, catapult_y + 5);

	//Set texture according to upcoming bird
	catapult.setRadius(16);

	Object::Type nextType = game_world->getNextBirdType();
	sf::Texture BirdTexture;
	switch (nextType)
	{
	case Object::Type::BasicBird:
		BirdTexture.loadFromFile("images/basicBird.png");
		catapult.setTexture(&BirdTexture);
		break;
	case Object::Type::NoBird:
		catapult.setFillColor(sf::Color(255, 255, 255));
	}
	catapult.setOrigin(16, 16);

	if (shoot_aiming) {
		sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
		sf::Vector2f location = sf::Vector2f(catapult_x, catapult_y);
		float d = (float)std::sqrt(std::pow(mouse.x - location.x, 2) + std::pow(mouse.y - location.y, 2));
		if (d > 150)
			catapult.setPosition(location.x + 150 * (mouse.x - location.x) / d, location.y + 150 * (mouse.y - location.y) / d);
		else
			catapult.setPosition(mouse.x, mouse.y);
	}
	else
		catapult.setPosition(catapult_x, catapult_y);

	window.draw(catapult_bg);
	window.draw(catapult);
}

void GameWindow::drawGameDynamicObjects()
{
	for (auto& bird : game_world->getBirds()) { this->drawBird(bird); }
	for (auto& box : game_world->getBoxes()) { this->drawBox(box); }
	for (auto& pig : game_world->getPigs()) { this->drawPig(pig); }
}

void GameWindow::runGame()
{
	game_world->step();

	this->drawGameStatic();
	this->drawGameDynamicObjects();

	window.draw(this->setButton(backButton, sf::Vector2f(WINDOW_W * 0.05f, WINDOW_H * 0.1f), "images/game_backButton.png"));
	window.draw(this->setButton(replayButton, sf::Vector2f(WINDOW_W * 0.13f, WINDOW_H * 0.1f), "images/game_replayButton.png"));

	pollGameEvents();
}

void GameWindow::pollGameEvents()
{
	sf::Event evnt;
	while (window.pollEvent(evnt))
	{
		if (evnt.type == sf::Event::Closed)
			window.close();
		if (evnt.type == sf::Event::MouseButtonPressed && evnt.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f mouse = window.mapPixelToCoords(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y), view);
			std::cout << "Pressed " << mouse.x << " " << mouse.y << std::endl;
			if (mouse.x < catapult_x + 15 && mouse.x > catapult_x - 15 && mouse.y < catapult_y + 15 && mouse.y > catapult_y - 15)
				shoot_aiming = 1;
		}

		if (evnt.type == sf::Event::MouseButtonReleased && evnt.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f mouse = window.mapPixelToCoords(sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y), view);
			if (replayButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
				delete game_world;
				game_world = new Map(currentFile);
			}

			if (backButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
				delete game_world;
				phase = gamePhase::Levl_Bar;
			}

			if ((shoot_aiming) && (game_world->getCurrentBird() != nullptr)) {
				std::cout << "action event" << std::endl;
				game_world->getCurrentBird()->action(game_world);
			}

			if (shoot_aiming) {
				std::cout << "Shot Bird" << std::endl;
				game_world->getCatapultBody()->SetActive(false);
				game_world->ShootBird((32 * (catapult_x - catapult.getPosition().x)), -(32 * (catapult_y - catapult.getPosition().y)));

				shoot_aiming = 0;
			}

		}

	}
}
