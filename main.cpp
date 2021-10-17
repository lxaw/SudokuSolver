#include <iostream>

#include "./mySudoku.h"

int main(int argc, char** argv){

    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <Input File>\n";
        exit(1);
    }

    std::string fileName {""};
    mySudoku s{argv[1]};

    s.PrintData();
    std::cout << "Solving the puzzle...\n";
    if(s.SolvePuzzle()){
        std::cout << "Puzzle solved in " << s.getIterations() << " iterations.\n";
        s.PrintData();
    }else{
        std::cout << "Puzzle cannot be solved. Please check if the puzzle is properly initialized.\n";
    }
    return (0);
}
