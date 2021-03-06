#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <ctime>

const int CELL_SIZE = 20;	// inverted difficulty the higher it is the easier the level
const int WALL_THICKNES = 1;

const int START_X = 0;
const int START_Y = 0;
#define END_X ((m_Width / CELL_SIZE) - 1)
#define END_Y ((m_Height / CELL_SIZE) - 1)

struct Cell {
	bool walls[4];			// top right bottom left
	bool visited = false;   // for the mazeGen
	bool marked = false;	// if patfinder was on the cell
	bool correct = false;	// if the cell is correct
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
	sf::CircleShape circle;

	Cell* current;
	Cell* nextC;
public:
	bool mazeGenDone = false;
	bool allDone = false;
	bool error = false;

	Board(int screenWidth, int screenHeight);

	// uses maze generator
	int nextNeighbour();				// returns index of the next neighbour (randomly)
	void removeWalls(int nextIndex);
	void genMaze();

	void render(sf::RenderWindow &window);

	inline int getIndex(int x, int y) {
		return x + y * m_Width / CELL_SIZE;;
	}

	// pathfinder functions
	int pathFindNextNeighbour();
	void findPath();				
};
