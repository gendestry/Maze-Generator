#include <SFML/Graphics.hpp>
#include "Board.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

bool takenMazePic = false;
bool takenSolvedMazePic = false;
bool waitedOne = false;

int main() {
	try {
		sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "Maze Generator");
		sf::Event event;

		Board board(SCR_WIDTH, SCR_HEIGHT);
		// window.setFramerateLimit(10);

		while (window.isOpen()) {
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
					window.close();
			}

			window.clear();

			// drawing
			if (!board.mazeGenDone)
				board.genMaze();
			else if (!board.allDone)
				board.findPath();


			board.render(window);

			window.display();
			if (board.mazeGenDone && !takenMazePic) {
				sf::Image screenshot;
				screenshot = window.capture(); // empty because window is closed
				screenshot.saveToFile("maze.png"); // write empty image
				takenMazePic = true;
			}

			if (board.allDone && !takenSolvedMazePic) {
				if (waitedOne) { // must wait one more loop before saving because of the current cell still being drawn (for some unknown reason)
					sf::Image screenshot;
					screenshot = window.capture(); // empty because window is closed
					screenshot.saveToFile("solvedmaze.png"); // write empty image
					takenSolvedMazePic = true;
				}
				else waitedOne = true;
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << "!!!\n";
		// std::cin.get();
		return -1;
	}
	catch (...) { // this actually says ..., I'm not abbreviating :P
		std::cerr << "Unknown error.\n";
		return -1;
	}
	return 0;
}
