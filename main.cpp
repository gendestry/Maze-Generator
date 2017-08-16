#include <SFML\Graphics.hpp>
#include "Board.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "Maze Generator");
	sf::Event event;

	Board board(SCR_WIDTH, SCR_HEIGHT);
	//window.setFramerateLimit(10);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		window.clear();

		// drawing
		if (!board.error) {
			if (!board.mazeGenDone)
				board.genMaze();
			else if (!board.allDone)
				board.findPath();
			board.render(window);
		}

		window.display();
	}
	return 0;
}
