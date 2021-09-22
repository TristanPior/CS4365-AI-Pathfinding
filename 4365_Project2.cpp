/*


*/

#include "pch.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define PATH_COST 1

class Maze;

std::string input(std::string fileName);
void output(std::vector<std::string> directions, int numMoves, std::string fileName);
std::vector<std::string> pathfind(Maze m);

int numberMoves;

class State {
private:
	int atCol = -1;
	int atRow = -1;
	std::vector<int> blocksCol;
	std::vector<int> blocksRow;

public:
	State() {
		atCol = -1;
		atRow = -1;
		blocksCol;
		blocksRow;
	}
	State(int startRow, int startCol, std::vector<int> blRowpos, std::vector<int> blColpos) {
		atCol = startCol;
		atRow = startRow;
		blocksCol = blColpos;
		blocksRow = blRowpos;
	}

	bool operator == (State compare) {
		if (this->atCol != compare.atCol)
			return false;
		if (this->atRow != compare.atRow)
			return false;
		if (this->blocksCol != compare.blocksCol)
			return false;
		if (this->blocksRow != compare.blocksRow)
			return false;
		return true;
	}
};


class Maze {
private:
	//2D array representing the states graph where each state is placed on atx, aty
	State MazeArray[10][10];
	std::vector<State> goals;
	int startRow = -1;
	int startCol = -1;
	State isBlockState = State();

public:
	//Constructs a 2D array for the maze from the string input
	Maze(std::string input) {
		std::vector<int> goalsCol;
		std::vector<int> goalsRow;
		std::vector<int> blColpos;
		std::vector<int> blRowpos;
		//Stringstream for tokenizing the input
		std::stringstream input_ss(input);
		int temp = -1;
		//Populates the 2D state array
		for (int i = 0; i < 10; i++) {
			for (int k = 0; k < 10; k++) {
				input_ss >> temp;
				if (temp == 1) {
					startRow = i;
					startCol = k;
				}
				if (temp == 2) {
					goalsRow.push_back(i);
					goalsCol.push_back(k);
				}
				if (temp == 3) {
					blRowpos.push_back(i);
					blColpos.push_back(k);
				}
			}
		}
		for (int i = 0; i < 10; i++) {
			for (int k = 0; k < 10; k++) {
				bool isBlock = 0;
				for (int j = 0; j < blRowpos.size(); j++) {
					if (i == blRowpos[j] && k == blColpos[j])
						isBlock = 1;
				}
				if (!isBlock)
					MazeArray[i][k] = State(i, k, blRowpos, blColpos);
				else
					MazeArray[i][k] = State();
			}
		}
		for (int i = 0; i < goalsRow.size(); i++) {
			goals.push_back(State(goalsRow[i], goalsCol[i], blRowpos, blColpos));
		}
	}

	//Returns startX
	int getStartRow() {
		return startRow;
	}
	//Returns startY
	int getStartCol() {
		return startCol;
	}
	//Checks if the specified cell is a goal cell
	bool isGoal(int currRow, int currCol) {
		if (currRow < 0 || currRow > 9 || currCol < 0 || currCol > 9)
			return false;
		for (State s : goals) {
			if (s == MazeArray[currRow][currCol])
				return true;
		}
		return false;
	}

	std::vector<std::string> applicable(int currRow, int currCol) {
		std::vector<std::string> applicableActions;
		bool isBlock = 0;
		if (MazeArray[currRow][currCol] == isBlockState)
			return applicableActions;
		if (currRow > 0 && !isBlock)
			applicableActions.push_back("UP");
		if (currCol < 9 && !isBlock)
			applicableActions.push_back("RIGHT");
		if (currRow < 9 && !isBlock)
			applicableActions.push_back("DOWN");
		if (currCol > 0 && !isBlock)
			applicableActions.push_back("LEFT");
		return applicableActions;
	}
};


int main()
{
	std::string fileName = "";
	std::cout << "Insert file name: ";
	std::cin >> fileName;
	std::string inputString = input(fileName);
	Maze pathfindingMaze(inputString);
	std::vector<std::string> path = pathfind(pathfindingMaze);
	output(path, numberMoves, fileName);
}


//Function that reads the input file into a string.
std::string input(std::string fileName) {
	std::ifstream infile(fileName);
	std::string input_String = "";
	for (int i = 0; i < 100; i++) {
		std::string temp;
		infile >> temp;
		input_String += temp + " ";
	}
	return input_String;
}

//Function that pathfinds the way from the start point to the end point.
std::vector<std::string> pathfind(Maze m) {
	//Tracks the direction to get to any given node
	std::string path[10][10];
	//Tracks what cells are visited with a 1, all others with 0.
	bool visited[10][10];
	//Tracks the path cost to get to any given cell from the starting cell.
	int cost[10][10];
	int startRow = m.getStartRow();
	int startCol = m.getStartCol();

	//Initialize the cost array to be extremely high and the visited array to be 0
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			cost[i][k] = 999;
			visited[i][k] = false;
		}
	}

	//Set the starting position cost to 0 and path to SRC
	cost[startRow][startCol] = 0;
	//Initialize variables
	bool goalFound = false;
	int goalRow;
	int goalCol;

	//Loop to pathfind
	while (!goalFound) {
		//Set an initial value for currRow and currCol
		int currCost = 1000;
		int currRow = startRow;
		int currCol = startCol;
		//Find the lowest cost next cell
		for (int i = 0; i < 10; i++) {
			for (int k = 0; k < 10; k++) {
				if (!visited[i][k] && cost[i][k] < currCost) {
					currRow = i;
					currCol = k;
					currCost = cost[i][k];
				}
			}
		}
		visited[currRow][currCol] = true;
		//If the lowest cost cell is the goal then set the exit condition to true
		if (m.isGoal(currRow, currCol)) {
			goalFound = true;
			goalRow = currRow;
			goalCol = currCol;
		}
		else {

			//Check Northern cell
			for (std::string s : m.applicable(currRow, currCol)) {
				if (!visited[currRow - 1][currCol] && s == "UP") {
					cost[currRow - 1][currCol] = cost[currRow][currCol] + PATH_COST;
					path[currRow - 1][currCol] = "UP";
				}
				//Check Eastern cell
				if (!visited[currRow][currCol + 1] && s == "RIGHT") {
					cost[currRow][currCol + 1] = cost[currRow][currCol] + PATH_COST;
					path[currRow][currCol + 1] = "RIGHT";
				}
				//Check Southern cell
				if (!visited[currRow + 1][currCol] && s == "DOWN") {
					cost[currRow + 1][currCol] = cost[currRow][currCol] + PATH_COST;
					path[currRow + 1][currCol] = "DOWN";
				}
				//Check Western cell
				if (!visited[currRow][currCol - 1] && s == "LEFT") {
					cost[currRow][currCol - 1] = cost[currRow][currCol] + PATH_COST;
					path[currRow][currCol - 1] = "LEFT";
				}
			}
		}
	}

	//Goal has been found by now, so trace back from the goal to the source
	std::string current = path[goalRow][goalCol];
	std::vector<std::string> output;
	int currRow = goalRow;
	int currCol = goalCol;

	while (currRow != startRow || currCol != startCol) {
		output.push_back(current);
		numberMoves++;
		if (current == "UP") {
			currRow++;
			current = path[currRow][currCol];
		}
		else if (current == "RIGHT") {
			currCol--;
			current = path[currRow][currCol];
		}
		else if (current == "DOWN") {
			currRow--;
			current = path[currRow][currCol];
		}
		else if (current == "LEFT") {
			currCol++;
			current = path[currRow][currCol];
		}
	}

	return output;
}

//Function that Outputs the List of directions with a new direction on each line.
void output(std::vector<std::string> directions, int numMoves, std::string fileName) {
	std::ofstream outfile("output" + fileName);
	for (int i = 0; i < numMoves; i++) {
		outfile << directions.back() << std::endl;
		directions.pop_back();
	}
}
