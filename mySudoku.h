/*
 * Lexington Whalen
 * 10/06/21
 * Sudoku solver.
 * This program takes in a sudoku file like the ones shown in TESTS/ and 
 * solves it.
 * Input: a text file.
 * Output: void.
 */
#ifndef MY_SUDOKU_H
#define MY_SUDOKU_H

/*
 * This class solves sudoku.
 *
 * Note that all of the meat of the class is private. Users can only use the 
 * "solve" function and print.
 * They don't need to know the magic behind it!
 */

class mySudoku{
    public:
        /*
         * **********************************************
         * Ctor Dtor
         * **********************************************
         */
        // default ctor
        mySudoku();

        // param ctor
        mySudoku(std::string inputFile);
        // dtor
        ~mySudoku();


        /*
         * **********************************************
         * SolvePuzzle() solves the sudoku!
         * **********************************************
         */
        bool SolvePuzzle();

        /*
         * ****************************************
         * Prshort ints
         * ****************************************
         */
        void PrintData();
        // get the iterations, see how good we did!
        int getIterations();
    private:
        /*
         * *****************************************
         * Constants
         * *****************************************
         */
        static const short int DEFAULT_SQUARE_SIZE = 3;
        static const short int UNKNOWN_VAL = 0;
        /*
         * *****************************************
         * Attributes
         * *****************************************
         */
        // 3x3 of squares, each square of size "square_size";
        // size = length and width of square
        short int square_size {0};
        short int length {0};
        short int width {0};
        // stores the summation of the values of the sudoku (for checking)
        short int correctSummation{0};
        // the total area (size^2)
        short int area {0};
        short int  * matrix_ptr = nullptr;
        short int * original_matrix_ptr = nullptr;

        /*
         * *****************************************
         * Keeping track of the unknown indices in the puzzle
         * *****************************************
         */
        // count the number of unknown index
        short int unknownIndexCount {0};
        short int * unknownIndicesArr = nullptr;
        // keep track of the iterations
        int iterations = 0;

        /*
         * *****************************************
         * Functions
         * *****************************************
         */
        // alloc the mem
        void initMatrix();
        // set 0's
        void setMatrixZeroes();
        // read file
        void ReadData(const std::string &fileName);

        // out of bounds error
        void printOutOfBoundError(const short int& index,const short int &size);
        // clone matrix
        void cloneMatrix(short int* &cloner, short int* &clonee, const short int &size);
        // reset matrix to original
        void resetMatrix();

        /*
         * *********************************************
         */
        // init the summation
        void initCorrectSummation(const short int &start, const short int &stop);

        /*
         * **********************************************
         * Indexing functions
         * **********************************************
         */
        short int getColFromIndex(const short int &index);
        short int getRowFromIndex(const short int &index);
        short int getIndexFromRowCol(const short int &row, const short int &col);
        short int getIndexFromSquareIndex(const short int &square, const short int &index);
        short int getSquareFromIndex(const short int &index);
        short int getTopLeftIndexFromSquare(const short int &square);

        /*
         * **********************************************
         * Check if rows, columns, squares, index complete
         * Finally, check if the puzzle is solved
         * **********************************************
         */
        bool checkRowDone(const short int &row);
        bool checkColDone(const short int &col);
        bool checkSquareDone(const short int &square_num);
        bool checkIndexDone(const short int &index);
        bool checkSolved();
        /*
         * **********************************************
         * Check a value fits in some section of the puzzle
         * Finally, check if the value fits in all sections
         * **********************************************
         */
        bool CheckColumn(const short int& column, const short int& number);
        bool CheckRow(const short int& row, const short int& number);
        bool CheckSubSquare(const short int& square, const short int& number);
        bool CheckFits(const short int & val, const short int &index);

        /*
         * **********************************************
         * Set a value at some section of the puzzle
         * Get a value at some section of the puzzle
         * **********************************************
         */
        void setAtRowCol(const short int &value, const short int &row, const short int &col);
        void setAtIndex(const short int &val, const short int &index);
        short int getAtRowCol(const short int &row, const short int &col);

        /*
         * **********************************************
         * Populate an input array from some section of the puzzle
         * **********************************************
         */
        void populateRowArrFromIndex(short int arr[],const short int &index);
        void populateColArrFromIndex(short int arr[], const short int &index);
        void populateSquareArrFromIndex(short int arr[], const short int &index);
        /*
         * **********************************************
         * Get summations of some section of the puzzle
         * Used in checking if the section is complete
         * **********************************************
         */
        short int getSumOfCol(const short int &col);
        short int getSumOfRow(const short int &row);
        short int getSumOfSquare(const short int &square);
        
        /*
         * **********************************************
         * Getters and Setters for class
         * **********************************************
         */
        short int getLength() const;
        
        // recursive solve puzzle
        bool recursiveSolve();
};

#endif
