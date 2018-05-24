#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

// this program uses the backtracking method to solve the sudoku puzzle
/* used https://www.geeksforgeeks.org/backtracking-set-7-suduku/ for help but most of the programming
was done by myself despite the structure being similar, also added file reading/interpreting feature */

bool isInRow(int grid[9][9], int row, int number);
bool isInCol(int grid[9][9], int col, int number);
bool isInSubGrid(int grid[9][9], int row, int col, int number);
bool isOkay(int grid[9][9], int row, int col, int number);
bool solvePuzzle(int grid[9][9]);
bool FindUnknownLocation(int grid[9][9], int &row, int &col)
{
    for (row = 0; row < 9; row++)
        for (col = 0; col < 9; col++)
            if (grid[row][col] == 0)
                return true;
    return false;
}

int main(int argc, char const *argv[]) {
	// argc should be 2, only supply one parameter which is sudoku file name
	if ( argc != 2 ) {
        cout << "Must supply the input file name as the only parameter" << endl;
        exit(1);
    }

    // this is the sudoku grid, will hold correct amount of rows and columns of integers
    int grid[9][9];

    // the input file is typically 9 rows that each hold 9 characters (10 if including enter at end), spaces represent blanks in the input grid
    // read in the file and convert to a grid
    string line;
	ifstream infile;
	infile.open(argv[1]);
	int lineCount = 0;
    while(getline(infile, line)) { // To get all the lines.
    	if (line.length() > 9 + 1) { // one more than dimension because the enter at the end of the line counts in this length
    		cout << "Too many columns entered" << endl;
    		cout << line.length() << endl;
    		exit(1);
    	}
    	if (line.length() < 9) { // can't use dimension + 1 because the last line doesn't have an enter
    		cout << "Too few columns entered" << endl;
    		cout << line.length() << endl;
    		exit(1);
    	}

    	for (int i = 0; i < 9; i++) {
    		// representing unknown values as 0 since 0 isn't used in sudoku puzzles
    		if (line[i] == ' ') {
    			grid[lineCount][i] = 0;
    		} else {
    			grid[lineCount][i] = line[i] - '0';
    		}
    	}
    	lineCount++;
    }
	infile.close();

	if (lineCount != 9) {
		cout << "Number of rows is not correct" << endl;
		exit(1);
	}

	solvePuzzle(grid);

	// print the grid
	for (int i = 0; i < 9; i++) {
		cout << endl;
		for (int j = 0; j < 9; j++) {
			cout << grid[i][j] << " ";
		}
	}

	return 0;
}

// checks whether the number is in that row in grid
bool isInRow(int grid[9][9], int row, int number) {
	for (int c = 0; c < 9; c++) {
		if (grid[row][c] == number) {
			return true;
		}
	}
	return false;
}

// checks whether the number is in the column of the grid
bool isInCol(int grid[9][9], int col, int number) {
	for (int r = 0; r < 9; r++) {
		if (grid[r][col] == number) {
			return true;
		}
	}
	return false;
}

// checks if number is in 3x3 sub-square of grid
bool isInSubGrid(int grid[9][9], int row, int col, int number) {
	int firstRow;
	int firstCol;
	// determine the top left box in the 3x3 sub-square
	if (row < 3) {
		firstRow = 0;
	} else if (row < 6) {
		firstRow = 3;
	} else {
		firstRow = 6;
	}

	if (col < 3) {
		firstCol = 0;
	} else if (col < 6) {
		firstCol = 3;
	} else {
		firstCol = 6;
	}

	// check the 3x3 grid for the number
	for (int r = firstRow; r < firstRow + 3; r++) {
		for (int c = firstCol; c < firstCol + 3; c++) {
			if (grid[r][c] == number) {
				return true;
			}
		}
	}

	return false;
}

/* combines all of the requirements to figure out if the number can be placed there with no errors */
bool isOkay(int grid[9][9], int row, int col, int number) {
	if (!isInRow(grid, row, number) && !isInCol(grid, col, number) && !isInSubGrid(grid, row, col, number)) {
		return true;
	} else {
		return false;
	}
}

bool solvePuzzle(int grid[9][9]) {
	int row = 0;
	int col = 0;

	/* if all spots are known, then the puzzle is solved, but if not then the row and column
	are set to the spot of the next unknown in the grid */
	if (!FindUnknownLocation(grid, row, col)) {
       return true;
	}

	// try the possible numbers for the unknown spaces until they work
	for (int possibleNum = 1; possibleNum < 10; possibleNum++) {
		if (isOkay(grid, row, col, possibleNum)) {
			grid[row][col] = possibleNum;
			/* call function recursively to keep working off of guess for that space in grid,
			if it doesn't work then reassign that number to be zero as it was before when it was unknown*/
			if (solvePuzzle(grid)) {
				return true;
			}
			grid[row][col] = 0;
		}
	}
	return false;
}