#include "grid.h"
#include <cctype>
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;

bool plotLine(int row, int col, int distance, int dir, char plotChar, int fgbg);
void plotHorizontalLine(int row, int col, int distance, char ch, int fgbg);
void plotVerticalLine(int row, int col, int distance, char ch, int fgbg);
bool checkLineInputs(int row, int col, int distance, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);
int parsingCommands(string commandString, char& plotChar, int& mode, int& currentR, int& currentC, int& badPos);
int isValidCommand(string commandString, int& badPos);
int stringToInt(string commandString, int x);
void updatePos(int& row, int& col, int distance, int dir);

int main(){
	setSize(20, 30);
	char currentChar = '*';
	int currentMode = FG;
	for (;;){
		cout << "Enter a command string: ";
		string cmd;
		getline(cin, cmd);
		if (cmd == "")
			break;
		int position;
		int status = performCommands(cmd, currentChar, currentMode, position);
		switch (status) {
			case 0:
				draw();
				break;
			case 1:
				cout << "Syntax error at position " << position + 1 << endl;
				break;
			case 2:
				cout << "Cannot perform command at position " << position + 1 << endl;
				break;
			default:
				// It should be impossible to get here.
				cerr << "performCommands returned " << status << "!" << endl;
		}
	}
}

int performCommands(string commandString, char& plotChar, int& mode, int& badPos) {
	int currentR = 1; //set the current row to 1
	int currentC = 1; //set the current column to 1
	return parsingCommands(commandString, plotChar, mode, currentR, currentC, badPos); //return the value from parsingCommands (either 0, 1, 2) when parsingCommands() is ran
}

//the parsingCommands() function does the blunt of the work, taking in all the values from the other funcitons and using these values to determine what should be done
int parsingCommands(string commandString, char& plotChar, int& mode, int& currentR, int& currentC ,int& badPos) {
	if (commandString == "") //run nothing with an empty string
		return 0;
	if (isValidCommand(commandString, badPos) == 0) { //if there is no syntax error
		for (int i = 0; i < commandString.size(); i++) {
			switch (tolower(commandString[i])) {
				case 'v':
					if (currentR + stringToInt(commandString, i) > 20 || currentR + stringToInt(commandString, i) < 1) {
						badPos = i;
						return 2; //out of bounds error
					}
					plotLine(currentR, currentC, stringToInt(commandString, i), VERT, plotChar, mode); //run this if there are no out of bounds errors
					updatePos(currentR, currentC, stringToInt(commandString, i), VERT);
					break;
				case 'h':
					if (currentC + stringToInt(commandString, i) > 30 || currentC + stringToInt(commandString, i) < 1) {
						badPos = i;
						return 2; //out of bounds error
					}
					plotLine(currentR, currentC, stringToInt(commandString, i), HORIZ, plotChar, mode); //run this if there are no out of bounds errors
					updatePos(currentR, currentC, stringToInt(commandString, i), HORIZ);
					break;
				case 'f':
					mode = FG;
					plotChar = commandString[i + 1]; //set the plot character to the character after the f
					i++;
					break;
				case 'b':
					mode = BG;
					plotChar = commandString[i + 1]; //set the plot character to the character after the b
					i++;
					break;
				case 'c':
					clearGrid();
					currentR = 1;
					currentC = 1;
					plotLine(currentR, currentC, 0, HORIZ, ' ', FG); //set the current position to (1,1) and make sure the whole plot is empty
					mode = FG;
					plotChar = '*';
					break;
				default:
					break;
			}
		}
		return 0;
	}
	else
		return 1;
}

//this isValidCommand() function checks for syntax errors
int isValidCommand(string commandString, int& badPos) {
	for (int i = 0; i < commandString.size(); i++) {
		if (!isprint(commandString[i])) { //is the character not printable?
			badPos = i;
			return 1;
		}
		if (isalpha(commandString[i])) { //check if the character is a letter
			switch (tolower(commandString[i])) {
				case 'v':
				case 'h':
					if (isdigit(commandString[i + 1])) { //is the next character a digit?
						if (isdigit(commandString[i + 2])) //is the digit after that a digit?
							i += 2; //if so, then skip over the next two characters and continue
						else
							i++; //if not, then skip over the next character and continue
						break;
					}
					else if ((commandString[i + 1] == '-')) { //if the character after is a minus sign
						if (isdigit(commandString[i + 2])) {  //is the character after the minus a digit?
							if (isdigit(commandString[i + 3]))  //is the second character after the minus a digit?
								i += 3; //if so, then skip over the next three characters and continue
							else
								i += 2; //if so, then skip over the next two characters and continue
						}
						break;
					}
					else {
						badPos = i + 1; //if the next digit after the v or b is not a digit or minus sign, then it's a syntax error
						return 1;
					}
				case 'f':
				case 'b':
					if (isprint(commandString[i + 1])) { //is the character after the f or b a valid printable character?
						i++; //if so, skip over the next character and continue
						break;
					}
					else {
						badPos = i + 1; //if not, this position causes a syntax error
						return 1;
					}
				case 'c':
					break; //nothing needs to be done with c, we just want that character
				default:
					badPos = i; //anything that is not the 5 letters that run a command is a syntax error
					return 1;
			}
		}
		else {
			badPos = i; //a safety net just in case, but theoretically does the same as the default above
			return 1;
		}
	}
	return 0; //if nothing goes wrong, then there are no syntax errors
}

//the stringToInt() function converts strings with numbers into ints
int stringToInt(string commandString, int x) {
	for (int a = 0; a < commandString.size(); a++) {
		if (a == x) { //find what position in the string we want to check the digits after for
			if (isdigit(commandString[a + 2])) {
				if (commandString[a + 1] == '-') 
					if(isdigit(commandString[a + 3]))
						return -1 * (10 * (commandString[a + 2] - 48)) + commandString[a + 3] - 48; //if there is a minus sign and two digits after
					else
						return -1 * (commandString[a + 2] - 48); //if there is a minus sign and only one digit after
				else 
					return (10 * (commandString[a + 1] - 48)) + commandString[a + 2] - 48; //if there is two digits after
				}
			else 
				return commandString[a + 1] - 48; //if there is only one digit after
		}

	}
	return 0; //if nothing goes right, return 0 SHOULD NEVER HAPPEN
}

//this updatePos() function update the current position of the command so that the next command in the command string runs from where the last one ended off at
void updatePos(int& row, int& col, int distance, int dir) {
	if (dir == VERT) 
		row += distance;
	if(dir == HORIZ)
		col += distance;
	
}


//the plotLine() function utilizes the plotVerticalLine() and plotHorizontalLine() to plot a line using the parameters from parsingCommands()
bool plotLine(int row, int col, int distance, int dir, char plotChar, int fgbg) {
	if (checkLineInputs(row, col, distance, dir, plotChar, fgbg)) { //if the inputs don't cause problems with the plotting lines
		if (dir) { //if true (value of 1 for VERT)
			plotVerticalLine(row, col, distance, plotChar, fgbg);
		}
		else {
			plotHorizontalLine(row, col, distance, plotChar, fgbg);
		}
		return true; //you did it
	}
	else
		return false; //the inputs are not good
}

//the checkLineInputs() function checks the inputs for plotLine and makes sure that the parameters are good (e.g FG or BG for the mode)
bool checkLineInputs(int row, int col, int distance, int dir, char plotChar, int fgbg) {
	if (row >= 1 && col >= 1 && (dir == HORIZ || dir == VERT) && isprint(plotChar) && (fgbg == FG || fgbg == BG)) {
		if (dir == VERT) {
			if (row + distance <= 20 && row + distance >= 1) //make sure that the line being plotted doesn't go outside the grid
				return true;
		}
		if (dir == HORIZ) {
			if (col + distance >= 1 && col + distance <= 30) //make sure that the line being plotted doesn't go outside the grid
				return true;
		}
	}
	return false;
}

//the plotVerticalLine() function plots a vertical line with certain parameters
void plotVerticalLine(int row, int col, int distance, char ch, int fgbg) {
	if ((row + distance <= 20) && (row + distance >= 1)) { //if the end point is within the row limits
		if(!fgbg){ //if it is in the foreground
			if (col > 0 && col <= 30) { //if the col value is within the column limits
				if (distance >= 0) {
					for (int y = row; y <= row + distance; y++) { //set characters to the ch character, downwards
						if (y > 0 && y <= 20)
							setChar(y, col, ch);
					}
				}
				if (distance < 0) {
					for (int y = row; y >= row + distance; y--) { //set characters to the ch character, upwards
						if (y > 0 && y <= 20)
							setChar(y, col, ch);
					}
				}
			}
		}
		else { //background
			if (col > 0 && col <= 30) { //if the col value is within the column limits
				if (distance >= 0) {
					for (int y = row; y <= row + distance; y++) { 
						if (y > 0 && y <= 20 && getChar(y, col) == ' ') //set space characters to the ch character, downwards
							setChar(y, col, ch);
					}
				}
				if (distance < 0) {
					for (int y = row; y >= row + distance; y--) {
						if (y > 0 && y <= 20 && getChar(y, col) == ' ') //set space characters to the ch character, upwards
							setChar(y, col, ch);
					}
				}
			}
		}
	}
}

//the plotHorizontalLine() function plots a horizontal line with certain parameters
void plotHorizontalLine(int row, int col, int distance, char ch, int fgbg) {
	if ((col + distance >= 1) && (col + distance <= 30)) { //if the end point is within column limits
		if (!fgbg) { //if it is in the foreground
			if (row > 0 && row <= 20) { //if the row value is within the row limits
				if (distance >= 0) {
					for (int x = col; x <= col + distance; x++) { //set characters to the ch character, rightwards
						if (x > 0 && x <= 30) 
							setChar(row, x, ch);
					}
				}
				if (distance < 0) { //set characters to the ch character, leftwards
					for (int x = col; x >= col + distance; x--) { //set characters to the ch character, leftwards
						if (x > 0 && x <= 30) 
							setChar(row, x, ch);
					}
				}
			}
		}
		else { //background
			if (row > 0 && row <= 20) { //if the row value is within the row limits
				if (distance >= 0) {
					for (int x = col; x <= col + distance; x++) {
						if (x > 0 && x <= 30 && getChar(row, x) == ' ') //set space characters to the ch character, rightwards 
							setChar(row, x, ch);
					}
				}
				if (distance < 0) {
					for (int x = col; x >= col + distance; x--) {
						if (x > 0 && x <= 30 && getChar(row, x) == ' ') //set space characters to the ch character, leftwards
							setChar(row, x, ch);
					}
				}
			}
		}
	}
}