#include "Board.h"

Board::Board(int screenWidth, int screenHeight) : m_Width(screenWidth), m_Height(screenHeight) {
	if (screenWidth % CELL_SIZE != 0 || screenHeight % CELL_SIZE != 0) {
		error = true;
		std::cout << "Error: screenWidth and screenHeight must be divisible by CELL_SIZE (width % cell_size = 0)!!!";
	}

	srand(time(nullptr)); // seeding rand function
	
	// setting rectangles' sizes
	rect.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	horizontalWall.setSize(sf::Vector2f(CELL_SIZE, WALL_THICKNES));
	verticalWall.setSize(sf::Vector2f(WALL_THICKNES, CELL_SIZE));

	circle.setRadius(CELL_SIZE / 4);

	// pushing cells
	for (int i = 0; i < screenHeight / CELL_SIZE; i++) {
		for (int j = 0; j < screenWidth / CELL_SIZE; j++) {
			m_Cells.push_back(Cell(j, i));
		}
	}

	current = &m_Cells[getIndex(START_X, START_Y)];		// starter cell is visited
	current->visited = true;
	stack.push_back((*current));
}

Board::~Board() {
	// delete current; // doens't work for some reason
	// delete next;
}

int Board::nextNeighbour() {
	Cell c = (*current);
	std::vector<int> indexes;

	if (c.y - 1 >= 0 && m_Cells[getIndex(c.x, c.y - 1)].visited == false) {// top
		indexes.push_back(getIndex(c.x, c.y - 1));
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

	return indexes.size() > 0 ? indexes[rand() % indexes.size()] : -1;
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

void Board::genMaze() {
	int next = nextNeighbour();
	if (next != -1) {
		removeWalls(next);
		current = &m_Cells[next];
		current->visited = true;
		stack.push_back((*current));
	}
	else if (stack.size() > 0) {
		stack.pop_back();
		
		if (stack.size() > 0) {
			Cell *temp = &stack.back();
			current = &m_Cells[getIndex(temp->x, temp->y)];
			// delete temp; // dunno y
		}
		else {
			mazeGenDone = true;
			current = &m_Cells[getIndex(START_X, START_Y)];
			current->correct = true;
			current->marked = true;
			std::cout << "Generated the maze!" << std::endl;
		}
	}
}

inline int Board::getIndex(int x, int y) {
	return x + y * m_Width / CELL_SIZE;;
}

void Board::render(sf::RenderWindow &window) {
	for (int i = 0; i < m_Cells.size(); i++) {
		rect.setPosition(sf::Vector2f(m_Cells[i].x * CELL_SIZE, m_Cells[i].y * CELL_SIZE));

		if (m_Cells[i].visited)
			rect.setFillColor(sf::Color(10, 150, 100));	// visited
		else
			rect.setFillColor(sf::Color(10, 100, 150));	// not yet visited

		window.draw(rect);

		if (m_Cells[i].correct && mazeGenDone) { // draw correct path
			rect.setFillColor(sf::Color::Magenta);
			rect.setPosition(sf::Vector2f(m_Cells[i].x * CELL_SIZE, m_Cells[i].y * CELL_SIZE));
			window.draw(rect);
		}

		if (i == (getIndex(current->x, current->y)) && current != &m_Cells[END_X * END_Y - 1]) { // draw current cell
			rect.setFillColor(sf::Color(214, 113, 66));
			window.draw(rect);
		}

		if (i == getIndex(START_X, START_Y)) { // drawing start and end circle
			circle.setFillColor(sf::Color::Red);
			circle.setPosition(sf::Vector2f((START_X * CELL_SIZE) + circle.getRadius(), (START_Y * CELL_SIZE) + circle.getRadius()));
			window.draw(circle);
		}
		else if (i == getIndex(END_X - 1, END_Y - 1)) {
			circle.setFillColor(sf::Color::Blue);
			circle.setPosition(sf::Vector2f(((END_X - 1) * CELL_SIZE) + circle.getRadius(), ((END_Y - 1) * CELL_SIZE) + circle.getRadius()));
			window.draw(circle);
		}

		// drawing walls
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

int Board::pathFindNextNeighbour() {
	Cell c = (*current);
	std::vector<int> indexes;

	if (c.y - 1 >= 0 && c.walls[0] == false && m_Cells[getIndex(c.x, c.y - 1)].marked == false) {// top
		indexes.push_back(getIndex(c.x, c.y - 1));
	}
	if (c.x + 1 < m_Width / CELL_SIZE && c.walls[1] == false && m_Cells[getIndex(c.x + 1, c.y)].marked == false) { // right
		indexes.push_back(getIndex(c.x + 1, c.y));
	}
	if (c.y + 1 < m_Height / CELL_SIZE && c.walls[2] == false && m_Cells[getIndex(c.x, c.y + 1)].marked == false) { // bottom
		indexes.push_back(getIndex(c.x, c.y + 1));
	}
	if (c.x - 1 >= 0 && c.walls[3] == false && m_Cells[getIndex(c.x - 1, c.y)].marked == false) { // left
		indexes.push_back(getIndex(c.x - 1, c.y));
	}

	return indexes.size() > 0 ? indexes[rand() % indexes.size()] : -1;
}

void Board::findPath() {
	int next = pathFindNextNeighbour();
	// std::cout << next << std::endl;
	if (current->x == (END_X - 1) && current->y == (END_Y - 1)) {
		allDone = true;
		current->correct = true;
		std::cout << "Pathfinder has finished!" << std::endl;
	}
	else if (next != -1) {
		current->correct = true;
		current->marked = true;
		stack.push_back((*current));
		current = &m_Cells[next];
	}
	else if (stack.size() > 0) {
		Cell *temp = &stack.back();
		temp->correct = false;
		current->marked = true;
		current->correct = false;
		current = &m_Cells[getIndex((*temp).x, (*temp).y)];
		stack.pop_back();
	}
}
