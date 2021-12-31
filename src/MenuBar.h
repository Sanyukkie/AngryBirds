
#include "GameWindow.h"

void runLevelsMenu()
{
	window.setView(view);

	//Creating menu texture for backgraund
	menuTexture.loadFromFile("images/menu.png");
	menuTexture.setSmooth(1);
	sf::RectangleShape background((sf::Vector2f)menuTexture.getSize());
	background.setPosition(view.getCenter() - (sf::Vector2f)menuTexture.getSize() / 2.0f);
	background.setTexture(&menuTexture);

	window.draw(background);

	//Creating Levels menu
	levelSquareButton.loadFromFile("images/LevlsMenu_square.png");

	float x = view.getCenter().x - view.getSize().x / 8.0f; // -2.0f * levelSquareButton.getSize().x;
	float y = view.getCenter().y - view.getSize().y / 8.0f; //2.0f * levelSquareButton.getSize().y;

	sf::RectangleShape level_1_SquareShape((sf::Vector2f)levelSquareButton.getSize());
	level_1_SquareShape.setOrigin((sf::Vector2f)levelSquareButton.getSize() / 2.0f);
	level_1_SquareShape.setPosition(sf::Vector2f(x, y));
	level_1_SquareShape.setTexture(&levelSquareButton);

	sf::RectangleShape level_2_SquareShape((sf::Vector2f)levelSquareButton.getSize());
	level_2_SquareShape.setOrigin((sf::Vector2f)levelSquareButton.getSize() / 2.0f);
	level_2_SquareShape.setPosition(sf::Vector2f(x + 2.0f * levelSquareButton.getSize().x, y));
	level_2_SquareShape.setTexture(&levelSquareButton);

	window.draw(level_1_SquareShape);
	window.draw(level_2_SquareShape);

	pollMenuEvents();
}
