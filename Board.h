#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <ctime>

#define CELL_SIZE 20	// inverted difficulty the higher it is the easier the level
#define WALL_THICKNES 1

struct Cell {
	bool walls[4];// top right bottom left
	bool visited = false;
	int x, y;

	Cell() {
		x = 0;
		y = 0;
		for (int i = 0; i < 4; i++)
			walls[i] = true;
	}

	Cell(int cell_x, int cell_y) : x(cell_x), y(cell_y) {
		for (int i = 0; i < 4; i++)
			walls[i] = true;
	}
};

class Board {
private:
	int m_Width, m_Height;

	std::vector<Cell> m_Cells;
	std::vector<Cell> stack;

	sf::RectangleShape rect, horizontalWall, verticalWall;

	Cell* current;
	Cell* nextC;
public:
	bool done = false;
	bool error = false;

	Board(int screenWidth, int screenHeight);
	~Board();

	void render(sf::RenderWindow &window);
	void update();
	inline int getIndex(int x, int y);
	int nextNeighbour(); // returns index of nextneighbour (randomly)
	void removeWalls(int nextIndex);
};