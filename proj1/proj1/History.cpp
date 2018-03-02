#include "History.h"
#include "City.h"
#include "globals.h"

#include <iostream>

using namespace std;

History::History(int nRows, int nCols) : m_row(nRows), m_col(nCols){
    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols; j++){
            hGrid[i][j] = '.';
        }
    }
}

bool History::record(int r, int c){
    
    // if its 0 set A otherwise ++ the letter
    if(hGrid[r][c] == '.'){
        hGrid[r][c] = 'A';
    } else {
        hGrid[r][c]++;
    }
    
    return true;
}

void History::display() const {
    clearScreen();
    
    for(int i = 0; i < m_row; i++){
        for(int j = 0; j < m_col; j++){
            cout << hGrid[i][j];
        }
        cout << endl;
    }
    cout << endl;
}