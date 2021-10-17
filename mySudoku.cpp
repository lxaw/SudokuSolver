/*
 * Lexington Whalen
 * 10/06/21
 * Sudoku solver.
 * This program takes in a sudoku file like the ones shown in TESTS/ and 
 * solves it.
 * Input: a text file.
 * Output: void.
 *
 * Lots of shorts!
 */

#ifndef MY_SUDOKU_CPP
#define MY_SUDOKU_CPP

// library includes
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


// my includes
#include "mySudoku.h"

/*
 * *****************************************
 * Constructors, Destructors
 * *****************************************
 */
// default ctor
mySudoku::mySudoku(){
    /*
     * Inputs:
     * void
     * Outputs:
     * void
     * Notes:
     * This is the default constructor for the sudoku.
     */

    /*
     * I don't like to hard code.This is why I made it possible
     * to change the size of squares depending on class variables.
     *
     * I do however use "3" and "9", since I believe that any variant
     * of sudoku uses these numbers. So that even if I had a square size of 14, 
     * there would be three rows of three, so nine total.
     */
    this->square_size = this->DEFAULT_SQUARE_SIZE;

    this->length = 3*this->DEFAULT_SQUARE_SIZE;
    this->width = 3*this->DEFAULT_SQUARE_SIZE;

    this->area = (9*this->square_size*this->square_size);

    // init the matrix
    initMatrix();
    // init the correct summation
    // ie: what the sum of numbers from 1-9 is
    initCorrectSummation(1,this->length);
    
    // save a copy of the original mat
    // for referencing later if needed
    // I generally like to keep the original data somewhere.
    cloneMatrix(this->matrix_ptr,this->original_matrix_ptr,this->area);
}
// param ctor
mySudoku::mySudoku(std::string fileName){
    /*
     * Inputs:
     * string "fileName"
     * Outputs:
     * void
     * Notes:
     * This is the parameterized constructor.
     * Reads in a filename, populates this->matrix_ptr
     *
     */

    this->square_size = this->DEFAULT_SQUARE_SIZE;

    this->length = 3*this->DEFAULT_SQUARE_SIZE;
    this->width = 3*this->DEFAULT_SQUARE_SIZE;

    this->area = (9*this->square_size*this->square_size);

    // init the matrix
    initMatrix();
    // init the correct summation
    initCorrectSummation(1,this->length);
    // read the data
    ReadData(fileName);
    // save a copy of the original mat
    // I generally like to keep the original data somewhere.
    cloneMatrix(this->matrix_ptr,this->original_matrix_ptr,this->area);
}
// init the correct summation 
void mySudoku::initCorrectSummation(const short int &start, const short int &stop){
    /*
     * Inputs:
     * short int "start", short int "stop"
     * Outputs:
     * void
     * Notes:
     * This function sums inclusively from start to stop.
     * It is used for summing the values from 1-9 in sudoku to 
     * check if the puzzle is solved.
     */
    // sum from (inclusive) start to stop
    for(short int i=start;i<=stop;++i){
        this->correctSummation += i;
    }
}

mySudoku::~mySudoku(){
    /*
     * Inputs:
     * void
     * Outputs:
     * void
     * Notes:
     * This is the destructor. It deallocates the mem for
     * the poshort inters used in the class.
     *
     */
    // delete the matrix
    delete [] this->matrix_ptr;
    // delete the original matrix
    delete [] this->original_matrix_ptr;
    
    delete [] this->unknownIndicesArr;
}
/*
 * *****************************************
 * FUNCTIONS
 * *****************************************
 */
void mySudoku::initMatrix(){
    /*
     * Inputs:
     * void
     * Outputs:
     * void
     * Notes:
     * This function inits the matrix_ptr to all zeros.
     *
     */
    // size by size matrix
    this-> matrix_ptr = new short int[this->area] ;
    // a copy of the original sudoku board not to be edited
    this-> original_matrix_ptr = new short int[this->area] ;

    // clear the matrix
    setMatrixZeroes();
}

/*
 * Functions involving indexing
 */
short int mySudoku::getColFromIndex(const short int &index){
    /*
     * Inputs:
     * short int "index"
     * Outputs:
     * short int
     * Notes:
     * This function gets the corresponding column number of an
     * index. This is used to make a 1D matrix appear 2D.
     *
     */
    // gets the column number from a index
    return index % length;
}
short int mySudoku::getRowFromIndex(const short int &index){
    /*
     * Inputs:
     * short int "index"
     * Outputs:
     * short int
     * Notes:
     * This function gets the corresponding row number of an
     * index. This is used to make a 1D matrix appear 2D.
     */
    return index / length;
}
short int mySudoku::getIndexFromRowCol(const short int &row, const short int &col){
    /*
     * Inputs:
     * short int "row", short int "col"
     * Outputs:
     * short int
     * Notes:
     * This function helps translate between row, col notation to just index.
     * This is used to make a 1D matrix appear 2D.
     *
     */
    // returns correct index given a row and col
    return ((row * this->length) + col);
}

void mySudoku::setMatrixZeroes(){
    /*
     * Inputs:
     * void
     * Outputs:
     * void
     * Notes:
     * This function sets the matrix_ptr to all zeros.
     *
     */
    if(this->matrix_ptr != nullptr && this->square_size != 0){
        for(short int i=0;i<this->area;i++){
            this->matrix_ptr[i] = this->UNKNOWN_VAL;
        }
    }
}
/*
 * Summations
 */
short int mySudoku::getSumOfCol(const short int &col){
    /*
     * Inputs:
     * short int "col"
     * Outputs:
     * short int
     * Notes:
     * This function gets the sum of all entries in a column.
     * Used to test agains the total summation from 1-9 inclusive to
     * see if the column is complete.
     */
    // gets the sum of entries in a column
    // get jump by the length
    
    short int sum = 0;

    short int firstIndex = getIndexFromRowCol(0,col);
    // now loop
    for(short int i=firstIndex;i<this->area;i+=this->length){
        sum += this->matrix_ptr[i];
    }

    return sum;
}
short int mySudoku::getSumOfRow(const short int &row){
    /*
     * Inputs:
     * short int "row"
     * Outputs:
     * short int
     * Notes:
     * This function gets the sum of all entries in a row.
     *
     * Used to test agains the total summation from 1-9 inclusive to
     * see if the row is complete.
     */
    // gets the sum of entries in a row
    short int sum = 0;
    short int firstIndex = (row * length);
    short int bound = firstIndex + this->length;

    for(short int i=firstIndex;i<bound;++i){
        sum += this->matrix_ptr[i];
    }
    return sum;
}
short int mySudoku::getSumOfSquare(const short int &square){
    /*
     * Inputs:
     * short int "square"
     * Outputs:
     * short int
     * Notes:
     * This function gets the sum of all entries in a square.
     *
     * Used to test agains the total summation from 1-9 inclusive to
     * see if the square is complete.
     */
    short int summation = 0;

    short int col = (square % this->square_size) * this->square_size;
    short int row = (square/this->square_size) * this->square_size;

    short int start_index = getIndexFromRowCol(row,col);

    for(short int i=0;i<this->square_size;i++){
        short int j_start = start_index + (i*this->length);
        for(short int j = j_start;j < (j_start + this->square_size);j++){
            summation += this->matrix_ptr[j];
        }
    }
    return summation;
}

/*
 * Return the entries in a row or column as an arraylist.
 */
void mySudoku::populateRowArrFromIndex(short int arr[], const short int &index){
    /*
     * Inputs:
     * short int [] "arr", short int "index"
     * Outputs:
     * void
     * Notes:
     * 
     * This function populates "arr" with the values found in the row of 
     * index "index".
     *
     */

    short int row = getRowFromIndex(index);

    short int firstIndex = (row * length);
    short int bound = firstIndex + this->length;

    short int count = 0;
    for(short int i=firstIndex;i<bound;++i){
        arr[count] = this->matrix_ptr[i];
        count ++;
    }
}
void mySudoku::populateColArrFromIndex(short int arr[], const short int &index){
    /*
     * Inputs:
     * short int [] "arr", short int "index"
     * Outputs:
     * void
     * Notes:
     * 
     * This function populates "arr" with the values found in the column of 
     * index "index".
     */

    short int col = getColFromIndex(index);

    short int firstIndex = getIndexFromRowCol(0,col);
    // now loop
    short int count = 0;
    for(short int i=firstIndex;i<this->area;i+=this->length){
        arr[count] = this->matrix_ptr[i];
        count ++;
    }
}
void mySudoku::populateSquareArrFromIndex(short int arr[], const short int &index){
    /*
     * Inputs:
     * short int [] "arr", short int "index"
     * Outputs:
     * void
     * Notes:
     * 
     * This function populates "arr" with the values found in the square of 
     * index "index".
     */

    short int square = getSquareFromIndex(index);

    short int col = (square % this->square_size) * this->square_size;
    short int row = (square/this->square_size) * this->square_size;

    short int start_index = getIndexFromRowCol(row,col);

    short int count = 0;
    for(short int i=0;i<this->square_size;i++){
        short int j_start = start_index + (i*this->length);
        for(short int j = j_start;j < (j_start + this->square_size);j++){
            arr[count] = this->matrix_ptr[j];
            count ++;
        }
    }
}

// check if row or col complete
bool mySudoku::checkRowDone(const short int &row){
    /*
     * Inputs:
     * short int "row"
     * Outputs:
     * bool
     * Notes:
     * This function checks if row number "row" is complete.
     * It checks the sum of the entries of the row against the 
     * proper sum of 1-9 (inclusive)
     *
     */
    
    short int firstIndex = (row * length);
    short int bound = firstIndex + this->length;

    short int summation = getSumOfRow(row);
    if(this->correctSummation != summation){
        return false;
    }
    return true;
}
bool mySudoku::checkColDone(const short int &col){
    /*
     * Inputs:
     * short int "col"
     * Outputs:
     * bool
     * Notes:
     * This function checks if column number "col" is complete.
     * It checks the sum of the entries of the column against the 
     * proper sum of 1-9 (inclusive)
     */
    short int firstIndex = getIndexFromRowCol(0,col);

    short int summation = getSumOfCol(col);

    if(this->correctSummation != summation){
        return false;
    }
    return true;
}
bool mySudoku::checkSquareDone(const short int& square_num){
    /*
     * Inputs:
     * short int "square_num"
     * Outputs:
     * bool
     * Notes:
     *
     * This function checks if square number "square" is complete.
     * It checks the sum of the entries of the square against the 
     * proper sum of 1-9 (inclusive)
     */
    // checks if the square square_num is complete
    short int summation =getSumOfSquare(square_num);
    
    if(this->correctSummation != summation){
        return false;
    }

    return true;
}
bool mySudoku::checkIndexDone(const short int& index){
    /*
     * Inputs:
     * short int "index"
     * Outputs:
     * bool
     * Notes:
     *
     * This function checks if index number "index" is complete.
     * It checks the sum of the entries of the row,column, and square
     * found at the index against the 
     * proper sum of 1-9 (inclusive)
     */
    short int col = getColFromIndex(index);
    if(!checkColDone(col)){
        return false;
    }
    short int row = getRowFromIndex(index);
    if(!checkRowDone(row)){
        return false;
    }
    short int square = getSquareFromIndex(index);
    if(!checkSquareDone(square)){
        return false;
    }
    return true;

}
// check if puzzle complete
bool mySudoku::checkSolved(){
    /*
     * Inputs:
     * void
     * Outputs:
     * bool
     * Notes:
     * Checks if the sudoku is solved by checking if each
     * of the 9 columns, rows, and squares is complete.
     */
    for(short int i=0;i<this->length;i++){
        if(!checkColDone(i)){
            return false;
        }
        if(!checkRowDone(i)){
            return false;
        }
        if(!checkSquareDone(i)){
            return false;
        }
    }
    return true;
    // else we solved it! Woohoo!  return true;
}
/*
 * Setters getters
 */
int mySudoku::getIterations(){
    /*
     * Inputs:
     * void.
     * Outputs:
     * int
     * Notes:
     * This function returns the amount of iterations of the recursive solve.
     * Note we need an int here for larger range!
     */
    return this->iterations;
}
short int mySudoku::getSquareFromIndex(const short int &index){
    /*
     * Inputs:
     * short int "index"
     * Outputs:
     * short int
     * Notes:
     *
     * This function gets the square number that an index is in.
     * Ex: index 0 is in square number 0. Index 4 is in square number 1, et cetera.
     *
     */
    short int col = getColFromIndex(index);
    short int row = getRowFromIndex(index);

    short int square =  (col/this->square_size) + (row/this->square_size) * this->square_size;

    return square;
}

void mySudoku::setAtRowCol(const short int &value, const short int &row, const short int &col){
    /*
     * Inputs:
     * short int "value", short int "row", short int "col"
     * Outputs:
     * void
     * Notes:
     * This function sets a value "value" at row, col of the matrix.
     * Used for treating a 1D array like a 2D one.
     *
     */
    short int conversion = getIndexFromRowCol(row,col);
    this->matrix_ptr[conversion] = value;
}
short int mySudoku::getAtRowCol(const short int &row, const short int &col) {
    /*
     * Inputs:
     * short int "row", short int "col"
     * Outputs:
     * short int
     * Notes:
     * This function gets a value from row "row" and column "column" of the matrix.
     * Used for treating a 1D array like a 2D one.
     *
     */
    return this->matrix_ptr[getIndexFromRowCol(row,col)];
}
void mySudoku::setAtIndex(const short int &val, const short int &index){
    /*
     * Inputs:
     * short int "val", short int "index"
     * Outputs:
     * void
     * Notes:
     * This function sets a value "val" at index "index" of the matrix.
     *
     * NOTE! 
     * We only use this function in the initial part of reading the data.
     * If the data is bad, then we let the user know.
     *
     * Else, we don't use this function so that the program is faster.
     *
     */
    if(index >= this->area){
        printOutOfBoundError(index, this->area);
    }else{
        this->matrix_ptr[index] = val;
    }
}

/*
 * For checking if you can place a value in a column, row, or square
 */
bool mySudoku::CheckColumn(const short int& column, const short int &number){
    /*
     * Inputs:
     * short int "column", short int "number"
     * Outputs:
     * bool
     * Notes:
     * This function checks if you can place a value "number" in column "column"
     *
     */

    short int col_vals [this->length];

    short int colFirstIndex = getIndexFromRowCol(0,column);

    populateColArrFromIndex(col_vals,colFirstIndex);

    for(short int i=0;i<this->length;++i){
        if(number == col_vals[i]){
            return true;
        }
    }

    return false;
}
bool mySudoku::CheckRow(const short int& row, const short int &number){
    /*
     * Inputs:
     * short int "row", short int "number"
     * Outputs:
     * bool
     * Notes:
     * This function checks if you can place a value "number" in row "row"
     *
     */
    short int row_vals [this->length];

    short int rowFirstIndex = getIndexFromRowCol(row,0);

    populateRowArrFromIndex(row_vals,rowFirstIndex);

    for(short int i=0;i<this->length;++i){
        if(number == row_vals[i]){
            return true;
        }
    }

    return false;
}
bool mySudoku::CheckSubSquare(const short int& square, const short int &number){
    /*
     * Inputs:
     * short int "square", short int "number"
     * Outputs:
     * bool
     * Notes:
     * This function checks if you can place a value "number" in subsquare "square"
     *
     */
    short int square_vals [this->length];

    short int squareFirstIndex = getTopLeftIndexFromSquare(square);
    
    populateSquareArrFromIndex(square_vals,squareFirstIndex);

    for(short int i=0;i<this->length;i++){
        if(number == square_vals[i]){
            return true;
        }
    }

    return false;
}

short int mySudoku::getLength() const {
    /*
     * Inputs:
     * void
     * Outputs:
     * short int
     * Notes:
     * Gets the length of the sudoku (9)
     *
     */
    return this->length;
}
short int mySudoku::getTopLeftIndexFromSquare(const short int &square){
    /*
     * Inputs:
     * short int "square"
     * Outputs:
     * short int
     * Notes:
     * This function returns the top left index from a square. For instance, the top left index
     * of square 1 is 3.
     *
     * Used for treating a 1D array like a 2D one.
     *
     */
    short int col = (square % this->square_size) * this->square_size;
    short int row = (square/this->square_size) * this->square_size;

    short int start_index = getIndexFromRowCol(row,col);

    return start_index;
}

// solve the puzzle
bool mySudoku::SolvePuzzle(){
    /*
     * Inputs:
     * void
     * Outputs:
     * void
     * Notes:
     * This function calls the recursive method to solve the sudoku.
     * I split it between "SolvePuzzle" and a different solve method so that
     * I can freely change the method of solution without needing to change
     * "main.cpp" as well.
     *
     */
    if(recursiveSolve()){
        if(checkSolved()){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

bool mySudoku::CheckFits(const short int & val, const short int &index){
    /*
     * Inputs:
     * short int "val", short int "index"
     * Outputs:
     * bool
     * Notes:
     * This function checks if a value can fit in a column, row, or subsquare
     *
     */
    short int col = getColFromIndex(index);
    short int row = getRowFromIndex(index);
    short int square = getSquareFromIndex(index);

    if(CheckColumn(col, val)){
        return false;
    }
    if(CheckRow(row, val)){
        return false;
    }
    if(CheckSubSquare(square, val)){
        return false;
    }
    return true;
}

short int mySudoku::getIndexFromSquareIndex(const short int &square, const short int &index){
    // returns index from square square
    short int col = (square % this->square_size) * this->square_size;
    short int row = (square/this->square_size) * this->square_size;

    short int start_index = getIndexFromRowCol(row,col);

    short int adjustedIndex {start_index};

    for(short int i=0;i<index;i++) {
        if(i!= 0 and i%3 == 0){
            adjustedIndex += 7;
        }
        else{
            adjustedIndex ++;
        }
    }
    return adjustedIndex;
}

bool mySudoku::recursiveSolve(){
    /*
     * Inputs:
     * void.
     * Outputs:
     * void. All changes in place.
     * Notes:
     * This function recursively solves sudoku using backtracking.
     *
     *
     * Here is the idea.
     *
     * You loop over ONLY those squares that are unknown. This saves time!
     * You check if those squares are filled in or not. If not, we loop over the possible values.
     * Plug in the value. Assume it is correct, test by calling the function again and seeing
     * if the value actually could fit. Keep doing this until one doesn't fit. Then you can 
     * pop the method calls back until the one that led to the issue, then go to the next value of
     * i. Continue until all work.
     */
    this->iterations ++;

    for(short int i=0;i<this->unknownIndexCount;i++){
        short int adjustedIndex = this->unknownIndicesArr[i];
        if(this->matrix_ptr[adjustedIndex] == this->UNKNOWN_VAL){
            for(short int testVal = 1;testVal <=9;testVal ++){
                if(CheckFits(testVal,adjustedIndex)){
                    this->matrix_ptr[adjustedIndex] = testVal;
                    if(recursiveSolve()){
                        return true;
                    }else{
                        this->matrix_ptr[adjustedIndex] = UNKNOWN_VAL;
                    }
                }
            }
            return false;
        }
    }
    return true;
    
}
 
// read files
void mySudoku::ReadData(const std::string &fileName){
    /*
     * Inputs:
     * A string "fileName".
     * Outputs:
     * void. All changes made in place.
     * Notes:
     * This function reads in a file "fileName" and populates the 
     * matrix ptr that represents the sudoku matrix.
     */
    // string to store the input
    std::string inputLine;

    // expected size
    short int expectedSize = 9;

    std::ifstream readingFile;        

    readingFile.open(fileName);

    short int counter = 0;

    short int unknownCounter = 0;

    short int temp [this->area];

    while(readingFile.good() && counter < this->area){
        readingFile >> inputLine;
        // populate matrix
        std::stringstream ss(inputLine);
        short int input_val = 0;

        ss >> input_val;

        setAtIndex(input_val,counter);

        if(input_val == this->UNKNOWN_VAL) {
            temp[unknownCounter] = counter;
            unknownCounter ++;
        }
        // increment counter
        counter ++;
    }
    // close the file
    readingFile.close();

    this->unknownIndicesArr = new short int[unknownCounter];

    this->unknownIndexCount = unknownCounter;

    for(short int i=0;i<unknownCounter;i++){
        this->unknownIndicesArr[i] = temp[i];
    }
}
// clone the matrix
void mySudoku::cloneMatrix(short int* &cloner, short int* &clonee, const short int &size){
    /*
     * Inputs:
     * an short integer poshort inter "cloner", an short integer poshort inter "clonee", an short integer "size".
     * Outputs:
     * void.All changes made in place.
     * Notes:
     * This function clones an array "cloner" short into array "clonee".
     */
    for(short int i =0;i<size;++i){
        clonee[i] = cloner[i];
    }
}
// reset matrix to original
void mySudoku::resetMatrix(){
    /*
     * Inputs:
     * void.
     * Outputs:
     * void. All changes made in place.
     * Notes:
     * This function resets the original matrix.
     *
     */
    // just clone short into the matrix_ptr
    cloneMatrix(this->original_matrix_ptr,this->matrix_ptr,this->area);
}

void mySudoku::printOutOfBoundError(const short int &index,const short int &size){
    /*
     * Inputs:
     * an short integer "index", an short integer "size"
     * Outputs:
     * void.
     * Notes:
     * This function prints an out of bound error given an index and an array size.
     */
    std::cout << "************************\n" ;
    std::cout << "Index: " << "\"" << index << "\" out of bound: " << "\n" << size << "\"\n" ;
    std::cout << "************************\n" ;
}
// print
void mySudoku::PrintData(){
    /*
     * Inputs:
     * Void.
     * Outputs:
     * Void.
     * Notes:
     * This function prints the sudoku puzzle onto the screen.
     */
    std::cout << "-------------------\n";
    for(short int i=0;i<this->area;i++){
        // print the horizonal bars
        // only print the line if on the third row and
        // just completed the last column (on the first element)
        if((i != 0 && getColFromIndex(i) == 0) && (getRowFromIndex(i)%3 == 0)){
            std::cout << "\n-------------------";
        }

        // print the vertical bars
        if((i!=0) and (i % this->length == 0)){
            std::cout << "\n";
        }else if((i!=0) and i%this->square_size == 0){
            std::cout << "|";
        }
        std::cout << this->matrix_ptr[i];
        if(i % this->length != this->length - 1){
            std::cout << " ";
        }
    }
    std::cout << "\n-------------------\n";
}
#endif
