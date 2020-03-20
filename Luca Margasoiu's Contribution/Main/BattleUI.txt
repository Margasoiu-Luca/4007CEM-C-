#include <iostream>
#include <unistd.h>
#include <iostream>

using namespace std;




//TO DO: A star search algorithm.



int height=9;
int width=9;

char freenode = ' '; // "free" is reserved
char visited = '*';

class Coordinates{
public:
    int x;
    int y;
    
    Coordinates(int x = 0, int y = 0){
        this->x = x;
        this->y = y;
    }
    
    
};

Coordinates startP(1, 0);
Coordinates endP(7, 8);


char Map[9][10] =
{
    "M MMMMMMM",
    "M   M   M",
    "M MMM M M",
    "M M   M M",
    "M M M MMM",
    "M   M M M",
    "M MMM M M",
    "M   M   M",
    "MMMMMMM M",
};


void printAlgorithm(){
    for (int Y = 0; Y < height; Y++)
    {
        cout<<Map[Y]<<endl;
    }
    cout<<endl;
}

bool Solve(int Xcor, int Ycor){
    Map[Ycor][Xcor] = visited;
    
    printAlgorithm();
    
    //The base case. Checks if the end node has been reached. If so, the recursion ends.
    usleep(500000);
    if (Xcor == endP.x && Ycor == endP.y){
        return true;
    }
    
    
    
    // Recursive code starts here. It goes checks for a free node in the following order: Left, Right, Top, Bottom
    if (Xcor > 0 && (Map[Ycor][Xcor - 1] == freenode))
        if( Solve(Xcor - 1, Ycor)){
            return true;
    }
    if (Xcor < width && (Map[Ycor][Xcor + 1] == freenode))
        if(Solve(Xcor + 1, Ycor)){
        return true;
    }
    if (Ycor > 0 && (Map[Ycor - 1][Xcor] == freenode)){
        if(Solve(Xcor, Ycor - 1))
            return true;
    }
    
    if (Ycor < height && (Map[Ycor + 1][Xcor] == freenode)){
        if(Solve(Xcor, Ycor + 1))
            return true;
    }
    
    Map[Ycor][Xcor] = freenode;
    
    printAlgorithm();
    usleep(500000);
    return false;
}



int main(){
    if (Solve(endP.x, startP.y)){
        printAlgorithm();
    }
    else{
        cout<<"Something went terribly wrong.";
    }
    
    return 0;
}


//Code inspired from
//https://stackoverflow.com/questions/9191428/maze-solving-algorithm-in-c/9192067

