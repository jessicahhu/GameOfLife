#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
#include "testing/SimpleTest.h"
#include "vector.h"
#include "random.h"
using namespace std;

/*
 * The populateGrid function reads lines from the
 * already-open ifstream and populates the already-created
 * gameboard.
 */
void populateGrid(Grid<char> &gameboard, ifstream &ifs) {
    string temp;

    for(int i = 0; i < gameboard.numRows();i++){
        getLine(ifs, temp);
        for(int j = 0; j < temp.length(); j++){
            gameboard.set(i, j, temp[j]);
        }
    }
}

/*
 * The loadBoard function takes an already-open
 * ifstream and reads in the width and height of
 * the gameboard. Then it creates and populates
 * the gameboard and closes the ifstream.
 *
 * Return value: A Grid<char> representing the
 * Game of Life starting game board
 */
Grid<char> loadBoard(ifstream &ifs) {
    string rowsStr;
    string colsStr;

    getLine(ifs, rowsStr);
    getLine(ifs, colsStr);

    int rows = stoi(rowsStr);
    int cols = stoi(colsStr);

    Grid<char> gameboard(rows, cols);

    populateGrid(gameboard, ifs);

    return gameboard;
}

/*
 * The printBoard function prints the board to the screen
 */
void printBoard(Grid<char> gameboard) {
    for (int r = 0; r < gameboard.numRows(); r++) {
        for (int c = 0; c < gameboard.numCols(); c++) {
            cout << gameboard[r][c];
        }
        cout << endl;
    }
}

/*
 * isValid checks if the cell is a valid cell of the gameboard if there is no wrapping
*/
bool isValid(int r, int c, Grid<char> gameboard) {
    if(r <= gameboard.numRows() - 1 && c <= gameboard.numCols() - 1) {
        if(r >= 0 && c >= 0)
            return true;
    }
    return false;
}

/*
 * countNeighbors counts the neighbors of a grid squre.
 * '-' is an unpopulated square, and 'X' is a populated square.
 * If wrap is true, then squares that are on edges potentially
 * contribute to the overall count.
 *
 * Returns the neighbor count
 */
int countNeighbors(Grid<char> &gameboard, int row, int col, bool wrap) {
    int myNeighbors = 0;
    for(int r = row-1;r<=row+1;r++) {
        for(int c = col-1; c<=col+1;c++) {
            if(wrap) {
                if (gameboard[(r + gameboard.numRows()) % gameboard.numRows()][ (c + gameboard.numCols()) % gameboard.numCols()] == 'X') {
                    myNeighbors++;
                }
            }
            else {
                if (isValid(r, c, gameboard) && gameboard[r][c] == 'X') {
                    myNeighbors++;
                }
            }
        }
    }
    if(gameboard[row][col] == 'X') {
        myNeighbors--;
    }
    return myNeighbors;
}

/*
 * The tick function runs one iteration of the game of life
 * on gameboard, using wrap to determine if the board should be
 * wrapped or not
 *
 */
void tick(Grid<char> &gameboard, bool wrap) {
    Grid<char> tempboard(gameboard.numRows(), gameboard.numCols());
    int num = 0;

    for (int r = 0; r < gameboard.numRows(); r++) {
        for (int c = 0; c < gameboard.numCols(); c++) {
            num = countNeighbors(gameboard, r, c, wrap);
            if (num <= 1) {
                tempboard[r][c] = '-';
            }
            else if (num == 2) {
                tempboard[r][c] = gameboard [r][c];
            }
            else if (num == 3) {
                tempboard[r][c] = 'X';
            }
            else {
                tempboard[r][c] = '-';
            }
        }
    }
    gameboard = tempboard;
}

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    string myFunc = "";
    char numFrames;
    string wrapString;
    bool wrapBool = false;

    ifstream ifs;
    promptUserForFile(ifs, "Grid input file name?");
    Grid<char> gameboard = loadBoard(ifs);

    cout << "Should the simulation wrap around the grid (y/n)?";
    cin >> wrapString;

    while (wrapString != "y" && wrapString != "n") {
        cout << "Should the simulation wrap around the grid (y/n)?";
        cin >> wrapString;
    }
    if (wrapString == "y") {
        wrapBool = true;
    }

    cout << "a)nimate, t)ick, q)uit?";
    cin >> myFunc;

    while (toLowerCase(myFunc) != "a" && toLowerCase(myFunc) != "t" && toLowerCase(myFunc) != "q") {
        cout << "a)nimate, t)ick, q)uit?";
        cin >> myFunc;
    }
    if (toLowerCase(myFunc) == "a") {
        cout << "How many frames:";
        cin >> numFrames;
        while(!isdigit(numFrames)) {
            cout << "How many frames:";
            cin >> numFrames;
        }
        for (int i = 0; i < int(numFrames); i++) {
            tick(gameboard, wrapBool);
            printBoard(gameboard);
            pause(100);
            clearConsole();
        }
    }
    else if (toLowerCase(myFunc) == "t") {
        printBoard(gameboard);
        tick(gameboard, wrapBool);
        printBoard(gameboard);
    }
    else if (toLowerCase(myFunc) == "q") {
        cout << "Have a nice Life!" << endl;
    }
    return 0;
}

// used for creating tests
void saveGame(Grid<char> gameboard, string filename) {
    ofstream ofs;
    ofs.open(filename, std::fstream::out);
    ofs << gameboard.numRows() << endl;
    ofs << gameboard.numCols() << endl;
    for (int row = 0; row < gameboard.numRows(); row++) {
        for (int col = 0; col < gameboard.numCols(); col++) {
            ofs << gameboard[row][col];
        }
        ofs << endl;
    }
    ofs.close();
}

PROVIDED_TEST("Test loadBoard function on simple.txt") {
    Grid<char> gameboardSoln = {
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', 'X', 'X', 'X', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'}
    };
    ifstream ifs;
    openFile(ifs, "boards/simple.txt");
    Grid<char> gameboard = loadBoard(ifs);
    EXPECT_EQUAL(gameboard, gameboardSoln);
}

PROVIDED_TEST("Test loadBoard function on format.txt") {
    Grid<char> gameboardSoln = {
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', 'X', 'X', 'X', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-'}
    };
    ifstream ifs;
    openFile(ifs, "boards/format.txt");
    Grid<char> gameboard = loadBoard(ifs);
    EXPECT_EQUAL(gameboard, gameboardSoln);
}

PROVIDED_TEST("Test loadBoard function on stableplateau.txt") {
    Grid<char> gameboardSoln = {
        {'-', '-', '-', '-', '-', '-', '-', '-', 'X', 'X'},
        {'-', '-', '-', '-', '-', '-', '-', '-', 'X', 'X'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', 'X', '-', '-', '-', '-'},
        {'-', '-', '-', '-', 'X', '-', 'X', '-', '-', '-'},
        {'-', '-', '-', '-', 'X', '-', 'X', '-', '-', '-'},
        {'-', '-', '-', '-', '-', 'X', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'X', 'X', '-', '-', '-', '-', '-', '-', '-', '-'}
    };
    ifstream ifs;
    openFile(ifs, "boards/stableplateau.txt");
    Grid<char> gameboard = loadBoard(ifs);
    EXPECT_EQUAL(gameboard, gameboardSoln);
}

PROVIDED_TEST("Test simple.txt for one iteration") {
    ifstream ifs;
    openFile(ifs, "boards/simple.txt");
    Grid<char> gameboard = loadBoard(ifs);

    ifstream ifs2;
    openFile(ifs2, "boards-after-one-iteration/simple-after1.txt");
    Grid<char> gameboardAfterOne = loadBoard(ifs2);

    tick(gameboard, false);
    EXPECT_EQUAL(gameboard, gameboardAfterOne);
}

PROVIDED_TEST("Test all boards for one iteration, no wrapping") {
    Vector<string> files = {
        "diehard", "dinner-table", "fish", "flower",
        "format", "glider-explosion", "glider-gun",
        "glider", "quilt-square", "rpent", "seeds",
        "simple", "simple-edge", "snowflake", "spiral",
        "stableplateau", "tictactoe"
    };

    for (string &filename : files) {
        string boardFilename = "boards/" + filename + ".txt";
        string boardAfter1Filename =
                "boards-after-one-iteration/" +  filename + "-after1.txt";
        cout << boardFilename << endl;
        cout << boardAfter1Filename << endl;
        ifstream ifs;
        openFile(ifs, boardFilename);
        Grid<char> gameboard = loadBoard(ifs);
        tick(gameboard, false);

        ifstream ifs2;
        openFile(ifs2, boardAfter1Filename);
        Grid<char> gameboardAfter1 = loadBoard(ifs2);

        EXPECT_EQUAL(gameboard, gameboardAfter1);
//        saveGame(gameboard, boardAfter1Filename);
    }
}

PROVIDED_TEST("Test simple-edge for one iteration, with wrapping") {
    ifstream ifs;
    openFile(ifs, "boards/simple-edge.txt");
    Grid<char> gameboard = loadBoard(ifs);

    ifstream ifs2;
    openFile(ifs2, "boards-after-one-iteration/simple-edge-after1-wrap.txt");
    Grid<char> gameboardAfterOne = loadBoard(ifs2);

    tick(gameboard, true);
    EXPECT_EQUAL(gameboard, gameboardAfterOne);
}


STUDENT_TEST("Test simple.txt for countNeighbors without wrapping") {
    ifstream ifs;
    openFile(ifs, "boards/simple.txt");
    Grid<char> gameboard = loadBoard(ifs);

    EXPECT_EQUAL(countNeighbors(gameboard, 2, 1, false), 0);
}

STUDENT_TEST("Test simple.txt for countNeighbors with wrapping") {
    ifstream ifs;
    openFile(ifs, "boards/simple-edge.txt");
    Grid<char> gameboard = loadBoard(ifs);

    EXPECT_EQUAL(countNeighbors(gameboard, 1, 0, true), 3);
}

STUDENT_TEST("Test countNeighbors function on simple-edge-after1-wrap.txt "){
    ifstream ifs;
    openFile(ifs, "boards-after-one-iteration/simple-edge-after1-wrap.txt");
    Grid<char> gameboard = loadBoard(ifs);
    EXPECT_EQUAL(countNeighbors(gameboard, 2, 8, true), 8);
}
