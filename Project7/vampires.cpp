// vampires.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;                // max number of rows in the arena
const int MAXCOLS = 20;                // max number of columns in the arena
const int MAXVAMPIRES = 100;           // max number of vampires allowed

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
			  // type name, since it's mentioned in the Vampire declaration.

class Vampire
{
public:
	// Constructor
	Vampire(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	void move();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	int    m_poisonedCount;
	bool    m_poisonedMove;
};

class Player
{
public:
	// Constructor
	Player(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	string dropPoisonVial();
	string move(int dir);
	void   setDead();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	bool   m_dead;
};

class Arena
{
public:
	// Constructor/destructor
	Arena(int nRows, int nCols);
	~Arena();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     vampireCount() const;
	int     getCellStatus(int r, int c) const;
	int     numberOfVampiresAt(int r, int c) const;
	void    display(string msg) const;

	// Mutators
	void setCellStatus(int r, int c, int status);
	bool addVampire(int r, int c);
	bool addPlayer(int r, int c);
	void moveVampires();

private:
	int      m_grid[MAXROWS][MAXCOLS];
	int      m_rows;
	int      m_cols;
	Player* m_player;
	Vampire* m_vampires[MAXVAMPIRES];
	int      m_nVampires;
	int      m_turns;

	// Helper functions
	void checkPos(int r, int c, string functionName) const;
	bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nVampires);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Vampire implementation
///////////////////////////////////////////////////////////////////////////

Vampire::Vampire(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** A vampire must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "***** Vampire created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_poisonedCount = 0;
	m_poisonedMove = true;
}

int Vampire::row() const
{
	return m_row;
}

int Vampire::col() const
{
	return m_col;
}

bool Vampire::isDead() const
{
	if (m_poisonedCount == 2)
		return true;
	return false;
}

void Vampire::move()
{
	if (m_poisonedMove || m_poisonedCount == 0) {
		if (attemptMove(*m_arena, randInt(NORTH, WEST), m_row, m_col)) {
			if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON) {
				m_poisonedCount++;
				m_arena->setCellStatus(m_row, m_col, EMPTY);
			}
		}
		if (m_poisonedCount == 1) {
			m_poisonedMove = false;
		}
	}
	else
		m_poisonedMove = true;
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** The player must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_dead = false;
}

int Player::row() const
{
	return m_row;
}

int Player::col() const
{
	return m_col;
}

string Player::dropPoisonVial()
{
	if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
		return "There's already a poisoned blood vial at this spot.";
	m_arena->setCellStatus(m_row, m_col, HAS_POISON);
	return "A poisoned blood vial has been dropped.";
}

string Player::move(int dir)
{
	if (attemptMove(*m_arena, dir, m_row, m_col)) {
		if (m_arena->numberOfVampiresAt(m_row, m_col) != 0) {
			m_dead = true;
			return "Player walked into a vampire and died.";
		}
		else if (dir == NORTH) {
			return "Player moved north.";
		}
		else if (dir == EAST) {
			return "Player moved east.";
		}
		else if (dir == SOUTH) {
			return "Player moved south.";
		}
		else if (dir == WEST) {
			return "Player moved west.";
		}
	}
	return "Player couldn't move; player stands."; 
}

bool Player::isDead() const
{
	return m_dead;
}

void Player::setDead()
{
	m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	for (int i = 0; i < MAXVAMPIRES; i++)
		m_vampires[i] = nullptr;
	m_nVampires = 0;
	m_turns = 0;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
	delete m_player;
	m_player = nullptr;
	for (int i = 0; i < MAXVAMPIRES; i++) {
		delete m_vampires[i];
		m_vampires[i] = nullptr;
	}
}

int Arena::rows() const
{
	return m_rows;
}

int Arena::cols() const
{
	return m_cols;
}

Player* Arena::player() const
{
	return m_player;
}

int Arena::vampireCount() const
{
	return m_nVampires;
}

int Arena::getCellStatus(int r, int c) const
{
	checkPos(r, c, "Arena::getCellStatus");
	return m_grid[r - 1][c - 1];
}

int Arena::numberOfVampiresAt(int r, int c) const
{
	int counter = 0;
	for (int i = 0; i < MAXVAMPIRES; i++) {
		if (m_vampires[i] != nullptr && m_vampires[i]->row() == r && m_vampires[i]->col() == c)
			counter++;
	}
	return counter;
}

void Arena::display(string msg) const
{
	char displayGrid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill displayGrid with dots (empty) and stars (poisoned blood vials)
	for (r = 1; r <= rows(); r++)
		for (c = 1; c <= cols(); c++)
			displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

	// Indicate each vampire's position
	for (r = 1; r <= rows(); r++) {
		for (int c = 1; c <= cols(); c++) {
			switch (numberOfVampiresAt(r, c)) {
				case 1:
					displayGrid[r - 1][c - 1] = 'V';
					break;
				case 2:
					displayGrid[r - 1][c - 1] = '2';
					break;
				case 3:
					displayGrid[r - 1][c - 1] = '3';
					break;
				case 4:
					displayGrid[r - 1][c - 1] = '4';
					break;
				case 5:
					displayGrid[r - 1][c - 1] = '5';
					break;
				case 6:
					displayGrid[r - 1][c - 1] = '6';
					break;
				case 7:
					displayGrid[r - 1][c - 1] = '7';
					break;
				case 8:
					displayGrid[r - 1][c - 1] = '8';
					break;
				default:
					if (numberOfVampiresAt(r, c) >= 9) {
						displayGrid[r - 1][c - 1] = '9';
						break;
					}
			}
		}
	}

	// Indicate player's position
	if (m_player != nullptr)
		displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? 'X' : '@');

	// Draw the grid
	clearScreen();
	for (r = 1; r <= rows(); r++)
	{
		for (c = 1; c <= cols(); c++)
			cout << displayGrid[r - 1][c - 1];
		cout << endl;
	}
	cout << endl;

	// Write message, vampire, and player info
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << vampireCount() << " vampires remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player!" << endl;
	else if (m_player->isDead())
		cout << "The player is dead." << endl;
	cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
	checkPos(r, c, "Arena::setCellStatus");
	m_grid[r - 1][c - 1] = status;
}

bool Arena::addVampire(int r, int c)
{
	if (!isPosInBounds(r, c))
		return false;

	// Don't add a vampire on a spot with a poisoned blood vial
	if (getCellStatus(r, c) != EMPTY)
		return false;

	// Don't add a vampire on a spot with a player
	if (m_player != nullptr && m_player->row() == r && m_player->col() == c)
		return false;

	//Don't add a vampire if there are already the max number of vampires in the arena
	if (m_nVampires == MAXVAMPIRES)
		return false;

	//Add a vampire
	m_vampires[m_nVampires] = new Vampire(this, r, c);
	m_nVampires++;
	return true;
}

bool Arena::addPlayer(int r, int c)
{
	if (!isPosInBounds(r, c))
		return false;

	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Don't add a player on a spot with a vampire
	if (numberOfVampiresAt(r, c) > 0)
		return false;

	m_player = new Player(this, r, c);
	return true;
}

void Arena::moveVampires()
{
	for (int i = 0; i < MAXVAMPIRES; i++) {
		if (m_vampires[i] != nullptr) {
			m_vampires[i]->move();
			if (this->player()->row() == m_vampires[i]->row() && this->player()->col() == m_vampires[i]->col()) {
				this->player()->setDead();
				break;
			}
			if (m_vampires[i]->isDead()) {
				delete m_vampires[i];
				m_vampires[i] = nullptr;
				m_nVampires--;
			}
		}
	}
	m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
	return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
	if (r < 1 || r > m_rows || c < 1 || c > m_cols)
	{
		cout << "***** " << "Invalid arena position (" << r << ","
			<< c << ") in call to " << functionName << endl;
		exit(1);
	}
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nVampires)
{
	if (nVampires < 0)
	{
		cout << "***** Cannot create Game with negative number of vampires!" << endl;
		exit(1);
	}
	if (nVampires > MAXVAMPIRES)
	{
		cout << "***** Trying to create Game with " << nVampires
			<< " vampires; only " << MAXVAMPIRES << " are allowed!" << endl;
		exit(1);
	}
	int nEmpty = rows * cols - nVampires - 1;  // 1 for Player
	if (nEmpty < 0)
	{
		cout << "***** Game created with a " << rows << " by "
			<< cols << " arena, which is too small too hold a player and "
			<< nVampires << " vampires!" << endl;
		exit(1);
	}

	// Create arena
	m_arena = new Arena(rows, cols);

	// Add player
	int rPlayer;
	int cPlayer;
	do
	{
		rPlayer = randInt(1, rows);
		cPlayer = randInt(1, cols);
	} while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
	m_arena->addPlayer(rPlayer, cPlayer);

	// Populate with vampires
	while (nVampires > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		if (r == rPlayer && c == cPlayer)
			continue;
		m_arena->addVampire(r, c);
		nVampires--;
	}
}

Game::~Game()
{
	delete m_arena;
}

string Game::takePlayerTurn()
{
	for (;;)
	{
		cout << "Your move (n/e/s/w/x or nothing): ";
		string playerMove;
		getline(cin, playerMove);

		Player* player = m_arena->player();
		int dir;

		if (playerMove.size() == 0)
		{
			if (recommendMove(*m_arena, player->row(), player->col(), dir))
				return player->move(dir);
			else
				return player->dropPoisonVial();
		}
		else if (playerMove.size() == 1)
		{
			if (tolower(playerMove[0]) == 'x')
				return player->dropPoisonVial();
			else if (decodeDirection(playerMove[0], dir))
				return player->move(dir);
		}
		cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
	}
}

void Game::play()
{
	m_arena->display("");
	Player* player = m_arena->player();
	if (player == nullptr)
		return;
	while (!player->isDead() && m_arena->vampireCount() > 0)
	{
		string msg = takePlayerTurn();
		m_arena->display(msg);
		if (player->isDead())
			break;
		m_arena->moveVampires();
		m_arena->display(msg);
	}
	if (player->isDead())
		cout << "You lose." << endl;
	else
		cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
	if (highest < lowest)
		swap(highest, lowest);
	static random_device rd;
	static default_random_engine generator(rd());
	uniform_int_distribution<> distro(lowest, highest);
	return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
	switch (tolower(ch))
	{
	default:  return false;
	case 'n': dir = NORTH; break;
	case 'e': dir = EAST;  break;
	case 's': dir = SOUTH; break;
	case 'w': dir = WEST;  break;
	}
	return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would run off the edge of the arena.
// Otherwise, update r and c to the position resulting from the move and
// return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	if (dir == NORTH) {
		if (r > 1) {
			r -= 1;
			return true;
		}
	}
	else if (dir == EAST) {
		if (c < a.cols()) {
			c += 1;
			return true;
		}
	}
	else if (dir == SOUTH) {
		if (r < a.rows()) {
			r += 1;
			return true;
		}
	}
	else if (dir == WEST) {
		if (c > 1) {
			c -= 1;
			return true;
		}
	}
	return false;
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poisoned blood vial and
// not move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
	int directN, directE, directS, directW;
	bool zeroN, zeroE, zeroS, zeroW;
	int zeroCount = 0, minDir;
	int totalN = 1000, totalE = 1000, totalS = 1000, totalW = 1000;

	r > 1 ? directN = a.numberOfVampiresAt(r - 1, c) : directN = 1000;
	c < a.cols() ? directE = a.numberOfVampiresAt(r, c + 1) : directE = 1000;
	r < a.rows() ? directS = a.numberOfVampiresAt(r + 1, c) : directS = 1000;
	c > 1 ? directW = a.numberOfVampiresAt(r, c - 1) : directW = 1000;

	if (directN < 1000) totalN = directN + a.numberOfVampiresAt(r - 1, c - 1) + a.numberOfVampiresAt(r - 1, c + 1);
	if (directE < 1000) totalE = directE + a.numberOfVampiresAt(r - 1, c + 1) + a.numberOfVampiresAt(r + 1, c + 1);
	if (directS < 1000) totalS = directS + a.numberOfVampiresAt(r + 1, c - 1) + a.numberOfVampiresAt(r + 1, c + 1);
	if (directW < 1000) totalW = directW + a.numberOfVampiresAt(r - 1, c - 1) + a.numberOfVampiresAt(r + 1, c - 1);

	if (r > 2) totalN += a.numberOfVampiresAt(r - 2, c);
	if (c < a.cols() - 1) totalE += a.numberOfVampiresAt(r, c + 2);
	if (r < a.rows() - 1) totalS += a.numberOfVampiresAt(r + 2, c);
	if (c > 2) totalW += a.numberOfVampiresAt(r, c - 2);

	zeroN = !directN;
	zeroE = !directE;
	zeroS = !directS;
	zeroW = !directW;

	if (zeroN)
		zeroCount++;
	if (zeroE)
		zeroCount++;
	if (zeroS)
		zeroCount++;
	if (zeroW)
		zeroCount++;

	if (zeroCount == 1) {
		if (zeroN)
			bestDir = NORTH;
		else if (zeroE)
			bestDir = EAST;
		else if (zeroS)
			bestDir = SOUTH;
		else 
			bestDir = WEST;
		return true;
	}

	else if (zeroCount == 2) {
		if (zeroN) {
			if(zeroE)
				bestDir = (min(totalN, totalE) == totalN ? NORTH : EAST);
			else if (zeroE)
				bestDir = (min(totalN, totalS) == totalN ? NORTH : SOUTH);
			else
				bestDir = (min(totalN, totalW) == totalN ? NORTH : WEST);
		}
		else if (zeroE) {
			if(zeroS)
				bestDir = (min(totalE, totalS) == totalE ? EAST : SOUTH);
			else
				bestDir = (min(totalE, totalW) == totalE ? EAST : WEST);
		}
		else
			bestDir = (min(totalS, totalW) == totalS ? SOUTH : WEST);
		return true;
	}

	else if (zeroCount == 3) {
		if (directN >= 1000 || directE >= 1000 || directS >= 1000 || directW >= 1000)
			return false;

		if (!zeroN) {
			minDir = min(min(totalE, totalS), totalW);
			bestDir = (minDir == totalE ? EAST : minDir == totalS ? SOUTH : WEST);
		}
		else if (!zeroE) {
			minDir = min(min(totalN, totalS), totalW);
			bestDir = (minDir == totalN ? NORTH : minDir == totalS ? SOUTH : WEST);
		}
		else if (!zeroS) {
			minDir = min(min(totalN, totalE), totalW);
			bestDir = (minDir == totalN ? NORTH : minDir == totalE ? EAST : WEST);
		}
		else {
			minDir = min(min(totalN, totalE), totalS);
			bestDir = (minDir == totalN ? NORTH : minDir == totalE ? EAST : SOUTH);
		}
		return true;
	}
	if(a.getCellStatus(r, c) == EMPTY)
		return false;
	
	minDir = min(min(min(totalN, totalE), totalS), totalW);
	bestDir = (minDir == totalN ? NORTH : minDir == totalE ? EAST : minDir == totalS ? SOUTH : WEST);
	return true;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 5, 2);
	Game g(10, 12, 40);

	// Play the game
	g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif