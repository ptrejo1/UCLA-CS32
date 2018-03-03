#include "mazequeue.h"

#include <queue>
#include <string>

bool pathExists(std::string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    
    std::queue<Coord> coordStack;
    Coord endPoint(er, ec);
    
    // Push start point on stack
    coordStack.push(Coord(sr, sc));
    maze[sr][sc] = 'o';
    
    while (!coordStack.empty()) {
        Coord point = coordStack.front();
        coordStack.pop();
        
        // Check if current point is end point
        if(point.r() == endPoint.r() && point.c() == endPoint.c()){
            return true;
        }
        
        // Check each place you can move from the current cell
        if (maze[point.r()][point.c()+1] == '.') {
            maze[point.r()][point.c()+1] = 'o';
            coordStack.push(Coord(point.r(), point.c()+1));
        }
        if (maze[point.r()+1][point.c()] == '.') {
            maze[point.r()+1][point.c()] = 'o';
            coordStack.push(Coord(point.r()+1, point.c()));
        }
        if (maze[point.r()][point.c()-1] == '.') {
            maze[point.r()][point.c()-1] = 'o';
            coordStack.push(Coord(point.r(), point.c()-1));
        }
        if (maze[point.r()-1][point.c()] == '.') {
            maze[point.r()-1][point.c()] = 'o';
            coordStack.push(Coord(point.r()-1, point.c()));
        }
    }
    
    return false;
}