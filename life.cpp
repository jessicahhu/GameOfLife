// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

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

/**
 * The populateGrid function reads lines from the
 * already-open ifstream and populates the already-created
 * gameboard.
 */
void populateGrid(Grid<char> &gameboard, ifstream &ifs) {
    // TODO: Your code here
}

/**
 * The loadBoard function takes an already-open
 * ifstream and reads in the width and height of
 * the gameboard. Then it creates and populates
 * the gameboard and closes the ifsteram.
 *
 * Return value: A Grid<char> representing the
 * Game of Life starting game board
 */
Grid<char> loadBoard(ifstream &ifs) {
    // TODO: Your code here (you will change the return value)
    return {{(char)randomInteger(65, 90), (char)randomInteger(65, 90)}};
}

/**
 * The printBoard function prints the board to the screen
 */
void printBoard(Grid<char> gameboard) {
    // TODO: Your code here
}

/**
 * countNeighbors counts the neighbors of a grid squre.
 * '-' is an unpopulated square, and 'X' is a populated square.
 * If wrap is true, then squares that are on edges potentially
 * contribute to the overall count.
 *
 * Returns the neighbor count
 */
int countNeighbors(Grid<char> &gameboard, int row, int col, bool wrap) {
    int myNeighbors = 0;
    // TODO: Your code here (change the return value)
    for(int r = row-1;r<=row+1;r++) {
        for(int c = col-1; c<=col+1;c++)
                    
    }
    return myNeighbors;
}

/**
 * The tick function runs one iteration of the game of life
 * on gameboard, using wrap to determine if the board should be
 * wrapped or not
 *
 */
void tick(Grid<char> gameboard, bool wrap) {

}

// You should have more functions here

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    // TODO: Finish the game of life!

    cout << "Have a nice Life!" << endl;
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