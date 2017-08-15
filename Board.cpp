#include "Board.h"

Board::Board(int screenWidth, int screenHeight) : m_Width(screenWidth), m_Height(screenHeight) {
	if (screenWidth % CELL_SIZE != 0 || screenHeight % CELL_SIZE != 0) {
		error = true;
		std::cout << "Error: screenWidth and screenHeight must be divisible by CELL_SIZE (width % cell_size = 0)!!!";
	}

	srand(time(nullptr)); // seeding rand function
	
	rect.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	horizontalWall.setSize(sf::Vector2f(CELL_SIZE, WALL_THICKNES));
	verticalWall.setSize(sf::Vector2f(WALL_THICKNES, CELL_SIZE));

	// horizontalWall.setFillColor(sf::Color::Red);
	// verticalWall.setFillColor(sf::Color::Red);

	for (int i = 0; i < screenHeight / CELL_SIZE; i++) {
		for (int j = 0; j < screenWidth / CELL_SIZE; j++) {
			m_Cells.push_back(Cell(j, i));

			if (i == 0 && j == 0)	// manual wall removing for start and finish
				m_Cells[0].walls[0] = false;

			if (i == (screenHeight / CELL_SIZE) - 1 && j == (screenWidth / CELL_SIZE) - 1)
				m_Cells[(screenWidth / CELL_SIZE) * (screenHeight / CELL_SIZE) - 1].walls[1] = false;
			
		}
	}

	current = &m_Cells[0];		// starter cell
	(*current).visited = true;	// is visited
	stack.push_back((*current));
}

void Board::update() {
	int next = nextNeighbour();
	if (next != 0) {
		removeWalls(next);
		current = &m_Cells[next];
		(*current).visited = true;
		stack.push_back((*current));
	}
	else if (stack.size() > 0) {
		stack.pop_back();
		
		if (stack.size() > 0) {
			Cell *temp;
			temp = &stack.back();
			current = &m_Cells[getIndex((*temp).x, (*temp).y)];
			// delete temp; // dunno y
		}
		else {
			done = true;
			std::cout << "Done";
		}
	}
}

void Board::render(sf::RenderWindow &window) {
	for (int i = 0; i < m_Cells.size(); i++) {
		if(m_Cells[i].visited)
			rect.setFillColor(sf::Color(10, 150, 100));	// visited
		else
			rect.setFillColor(sf::Color(10, 100, 150));	// not yet visited

		if(i == ((*current).x + (*current).y * (m_Width / CELL_SIZE)) && current != &m_Cells[0]) // current
			rect.setFillColor(sf::Color(214, 113, 66));


		rect.setPosition(sf::Vector2f(m_Cells[i].x * CELL_SIZE, m_Cells[i].y * CELL_SIZE));

		window.draw(rect);
		
		if (m_Cells[i].walls[0]) { // top
			horizontalWall.setPosition(sf::Vector2f(m_Cells[i].x * CELL_SIZE, m_Cells[i].y * CELL_SIZE));
			window.draw(horizontalWall);
		}
		if (m_Cells[i].walls[1]) { // right
			verticalWall.setPosition(sf::Vector2f(m_Cells[i].x * CELL_SIZE + CELL_SIZE - WALL_THICKNES, m_Cells[i].y * CELL_SIZE));
			window.draw(verticalWall);
		}
		if (m_Cells[i].walls[2]) { // bottom
			horizontalWall.setPosition(sf::Vector2f(m_Cells[i].x * CELL_SIZE, m_Cells[i].y * CELL_SIZE + CELL_SIZE - WALL_THICKNES));
			window.draw(horizontalWall);
		}
		if (m_Cells[i].walls[3]) { // left
			verticalWall.setPosition(sf::Vector2f(m_Cells[i].x * CELL_SIZE, m_Cells[i].y * CELL_SIZE));
			window.draw(verticalWall);
		}
	}
}

Board::~Board() {
	// delete current; // doens't work for some reason
	// delete next;
}

inline int Board::getIndex(int x, int y) {
	return x + y * m_Width / CELL_SIZE;;
}

int Board::nextNeighbour() {
	Cell c = (*current);
	std::vector<int> indexes;
	
	if (c.y - 1 >= 0 && m_Cells[getIndex(c.x, c.y - 1)].visited == false) {// top
		indexes.push_back(getIndex(c.x	, c.y - 1));
	}
	if (c.x + 1 < m_Width / CELL_SIZE && m_Cells[getIndex(c.x + 1, c.y)].visited == false) { // right
		indexes.push_back(getIndex(c.x + 1, c.y));
	}
	if (c.y + 1 < m_Height / CELL_SIZE && m_Cells[getIndex(c.x, c.y + 1)].visited == false) { // bottom
		indexes.push_back(getIndex(c.x, c.y + 1));
	}
	if (c.x - 1 >= 0 && m_Cells[getIndex(c.x - 1, c.y)].visited == false) { // left
		indexes.push_back(getIndex(c.x - 1, c.y));
	}
	
	return indexes.size() > 0 ? indexes[rand() % indexes.size()] : 0;
}

void Board::removeWalls(int nextIndex) {
	int diff = getIndex((*current).x, (*current).y) - nextIndex;
	nextC = &m_Cells[nextIndex];

	if (diff == 1) { // the current is to the RIGHT of the next
		(*current).walls[3] = false;
		(*nextC).walls[1] = false;
	}
	else if (diff == -1) { // the current is to the LEFT of the next
		(*current).walls[1] = false;
		(*nextC).walls[3] = false;
	}
	else if (diff < -1) {// the current is higher of the next
		(*current).walls[2] = false;
		(*nextC).walls[0] = false;
	}
	else if (diff > 1) {
		(*current).walls[0] = false;
		(*nextC).walls[2] = false;
	}

}