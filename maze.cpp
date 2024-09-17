//
//  maze.cpp
//  maze
//
//  Created by Conor McJannett on 7/26/22.
//

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <time.h>
#include "Player.hpp"
using namespace std;

//randomly generate an x and y coordinate inside of the maze
//but not along the edge
void generateCoords(const vector< vector<char> > &maze,
                    unsigned short &randomStartX, 
                    unsigned short &randomStartY);

// returns true if all nearby coordinates (including diagonal adjacent
// ones), as well as the original coordinates, do not contain the object
// returns false if one does.
bool aroundClearOf(vector< vector<char> > &maze,
                   unsigned short x, unsigned short y, const char object);

// randomly generates inner walls without trapping the player or M, and
// without blocking the key or the door
void generateInnerWalls(vector< vector<char> > &maze,
                        unsigned short &randomX, unsigned short &randomY,
                        unsigned short &randomDirection,
                        const unsigned short wallCount);

//prints the maze by the x and y axis of a regular 2D graph; the origin is
//located in the bottom left and all other coordinates have positive values;
//hence their representation by unsigned shorts.
void printMaze(const vector< vector<char> > &maze);

//Detects what direction the player will move from a wasd entry by the user
//if the user entry is unrecognized, it will prompt them and ask for reentry on
//loop until the user has entered a recognized wasd value, upper or lowercase.
void moveInput(char input, Player &p1, vector< vector<char> > &maze);

//Moves 'M' by randomly picking a direction, checking to see if M can move
//in that direction, moves M if true but will continue to pick random
//directions until it finds one in which M can move.
//M cannot move into '#', '!', or 'E', but can move onto ' ' or '@'.
//If M moves onto '@', game over.
void moveM(unsigned short randomDirection, Player &M,
           vector< vector<char> > &maze);


int main()
{
    //initialize variables and random number generation
    srand(static_cast<unsigned>(time(nullptr)));
    char input;
    unsigned short domain = 40, range = 15, randomStartX, randomStartY;
    
    //create maze vector with all blank spaces
    vector< vector<char> > maze(domain, vector<char>(range, ' '));
    //change edge tiles to wall tiles
    for(vector< vector<char> >::size_type x = 0; x < maze.size(); x++)
    {
        for(vector< vector<char> >::size_type y = 0; y < maze[x].size(); y++)
        {
            if(x == 0 || y == 0
                 || x == maze.size() - 1 || y == maze[x].size() - 1)
            {
                maze[x][y] = '#';
            }
        }
    }
    //generate a random number 0-3 to decide which wall to randomly put the
    //escape door on, without putting the door in the corner
    unsigned short randomDirection = static_cast<unsigned short>(rand() % 4);
    switch(randomDirection)
    {
        case(0):    //left wall
            maze[0][static_cast<unsigned short>(rand())
                                                % (maze[0].size()
                                                   - 2) + 1] = 'E';
            break;
        case(1):    //right wall
            maze[maze.size() - 1][static_cast<unsigned short>
                                  (rand()) % (maze[0].size() - 2) + 1] = 'E';
            break;
        case(2):    //top wall
            maze[static_cast<unsigned short>
                 (rand()) % (maze.size() - 2) + 1][maze[0].size() - 1] = 'E';
            break;
        case(3):    //bottom wall
            maze[static_cast<unsigned short>
                 (rand()) % (maze.size() - 2) + 1][0] = 'E';
            break;
    }
    //setting up the pieces...
    generateCoords(maze, randomStartX, randomStartY);
    Player p1(randomStartX, randomStartY);
    maze[p1.getX()][p1.getY()] = '@';
    generateCoords(maze, randomStartX, randomStartY);
    Player M(randomStartX, randomStartY);
    maze[M.getX()][M.getY()] = 'M';
    generateCoords(maze, randomStartX, randomStartY);
    maze[randomStartX][randomStartY] = '!';
    generateInnerWalls(maze, randomStartX, randomStartY, randomDirection, 20);
    printMaze(maze);
    
    //creating inner maze walls...
    
    //notes and initial prompt
//    cout << "maze.size() is equal to " << maze.size() << endl;
//    cout << "maze[0].size() is equal to " << maze[0].size() << endl;
    cout << "Welcome to the C++ Maze! Use WASD to move (W = up, A = left, S ="
            " down, D = right).\n\nYour goal is to obtain the key represented"
            " by the '!' mark somewhere in the maze, and to reach the Exit"
            " represented by 'E' in the outer wall of the maze."
            "\nYou are playing as '@'."
            "\n\n P.S.\nBeware of M.\n" << endl;
    
    //game loop
    do
    {
        cout << "Enter a direction below.\n" << endl;
        cin >> input;
        moveInput(input, p1, maze);
        moveM(static_cast<unsigned short>(rand()) % 4, M, maze);
        printMaze(maze);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while(!(p1.getGameOver() || M.getGameOver()));
    return 0;
}

void generateCoords(const vector< vector<char> > &maze,
                    unsigned short &randomStartX, unsigned short &randomStartY)
{
    do
    {
        randomStartX = static_cast<unsigned short>(rand()) % (maze.size() - 2)
                                                   + 1;
        randomStartY = static_cast<unsigned short>(rand()) %
                                                   (maze[randomStartX].size()
                                                      - 2) + 1;
    } while(maze[randomStartX][randomStartY] != ' ');
}

//returns true if all nearby coordinates (including diagonal adjacent ones),
//do not contain the object.
//returns false if one does.
bool aroundClearOf(vector< vector<char> > &maze,
                 unsigned short x, unsigned short y, const char object)
{
    if(maze[x][y+1] == object || maze[x][y-1] == object
       || maze[x+1][y] == object || maze[x-1][y] == object
       || maze[x+1][y+1] == object || maze[x+1][y-1] == object
       || maze[x-1][y+1] == object || maze[x-1][y-1] == object)
    {
        return false;
    }
    return true;
}

//randomly generates inner walls without trapping the player or M, and without
//blocking the key or the door
void generateInnerWalls(vector< vector<char> > &maze,
                        unsigned short &randomX, unsigned short &randomY,
                        unsigned short &randomDirection,
                        const unsigned short wallCount)
{
    unsigned short randomWallLength, x, y, failSafe = 1000, easyFailSafe = 20,
                   numWalls = wallCount;
    while(numWalls > 0 && failSafe > 0)
    {
        do
        {
            generateCoords(maze, randomX, randomY);
            failSafe--;
        } while(!(aroundClearOf(maze, randomX, randomY, 'E')
                && aroundClearOf(maze, randomX, randomY, '#')
                && maze[randomX][randomY] == ' '
                && failSafe > 0));
        if(failSafe > 0)
        {
            x = randomX;
            y = randomY;
            maze[x][y] = '#';
            easyFailSafe = 20;
            do
            {
                randomDirection = static_cast<unsigned short>(rand() % 4);
                randomWallLength = static_cast<unsigned short>(rand()
                                                               % (21) + 5);
                switch(randomDirection)
                {
                    case(0):    //build left
                        if(maze[x - 1][y] == ' ' && maze[x - 2][y] == ' '
                           && maze[x - 2][y + 1] == ' '
                           && maze[x - 2][y - 1] == ' '
                           && maze[x - 1][y + 1] == ' '
                           && maze[x - 1][y - 1] == ' ')
                        {
                            maze[x - 1][y] = '#';
                            x--;
                            randomWallLength--;
                        }
                        else
                        {
                            easyFailSafe--;
                            failSafe--;
                        }
                        break;
                    case(1):    //build right
                        if(maze[x + 1][y] == ' ' && maze[x + 2][y] == ' '
                           && maze[x + 2][y + 1] == ' '
                           && maze[x + 2][y - 1] == ' '
                           && maze[x + 1][y + 1] == ' '
                           && maze[x + 1][y - 1] == ' ')
                        {
                            maze[x + 1][y] = '#';
                            x++;
                            randomWallLength--;
                        }
                        else
                        {
                            easyFailSafe--;
                            failSafe--;
                        }
                        break;
                    case(2):    //build up
                        if(maze[x][y + 1] == ' ' && maze[x][y + 2] == ' '
                           && maze[x - 1][y + 2] == ' '
                           && maze[x + 1][y + 2] == ' '
                           && maze[x - 1][y + 1] == ' '
                           && maze[x + 1][y + 1] == ' ')
                        {
                            maze[x][y + 1] = '#';
                            y++;
                            randomWallLength--;
                        }
                        else
                        {
                            easyFailSafe--;
                            failSafe--;
                        }
                        break;
                    case(3):    //build down
                        if(maze[x][y - 1] == ' ' && maze[x][y - 2] == ' '
                           && maze[x - 1][y - 2] == ' '
                           && maze[x + 1][y - 2] == ' '
                           && maze[x - 1][y - 1] == ' '
                           && maze[x + 1][y - 1] == ' ')
                        {
                            maze[x][y - 1] = '#';
                            y--;
                            randomWallLength--;
                        }
                        else
                        {
                            easyFailSafe--;
                            failSafe--;
                        }
                        break;
                }
            } while(randomWallLength > 0 && easyFailSafe > 0);
        }
        numWalls--;
    }
    if(!(failSafe > 0))
    {
        cout << "\nWall Count was too large, could not place all the walls in a"
                " reasonable amount of time." << endl;
    }
    else if(numWalls == 0)
    {
        cout << "\nWalls Filled in successfully." << endl;
    }
}
void printMaze(const vector< vector<char> > &maze)
{
    for(vector< vector<char> >::size_type y = 0; y < maze[0].size(); y++)
    {
        for(vector< vector<char> >::size_type x = 0; x < maze.size(); x++)
        {
            cout << maze[x][maze[x].size() - y - 1];
        }
        cout << endl;
    }
}
void moveInput(char input, Player &p1, vector< vector<char> > &maze)
{
    char charAt;
    bool valid = false;
    do
    {
        switch(input)
        {
            case('W'):
                charAt = maze[p1.getX()][p1.getY() + 1];
                p1.playerTouch(charAt);
                if(charAt == ' ' || charAt == '!')
                {
                    maze[p1.getX()][p1.getY() + 1] = '@';
                    maze[p1.getX()][p1.getY()] = ' ';
                    p1.setY(p1.getY() + 1);
                }
                valid = true;
                break;
            case('w'):
                charAt = maze[p1.getX()][p1.getY() + 1];
                p1.playerTouch(charAt);
                if(charAt == ' ' || charAt == '!')
                {
                    maze[p1.getX()][p1.getY() + 1] = '@';
                    maze[p1.getX()][p1.getY()] = ' ';
                    p1.setY(p1.getY() + 1);
                }
                valid = true;
                break;
            case('A'):
                charAt = maze[p1.getX() - 1][p1.getY()];
                p1.playerTouch(charAt);
                if(charAt == ' ' || charAt == '!')
                {
                    maze[p1.getX() - 1][p1.getY()] = '@';
                    maze[p1.getX()][p1.getY()] = ' ';
                    p1.setX(p1.getX() - 1);
                }
                valid = true;
                break;
            case('a'):
                charAt = maze[p1.getX() - 1][p1.getY()];
                p1.playerTouch(charAt);
                if(charAt == ' ' || charAt == '!')
                {
                    maze[p1.getX() - 1][p1.getY()] = '@';
                    maze[p1.getX()][p1.getY()] = ' ';
                    p1.setX(p1.getX() - 1);
                }
                valid = true;
                break;
            case('S'):
                charAt = maze[p1.getX()][p1.getY() - 1];
                p1.playerTouch(charAt);
                if(charAt == ' ' || charAt == '!')
                {
                    maze[p1.getX()][p1.getY() - 1] = '@';
                    maze[p1.getX()][p1.getY()] = ' ';
                    p1.setY(p1.getY() - 1);
                }
                valid = true;
                break;
            case('s'):
                charAt = maze[p1.getX()][p1.getY() - 1];
                p1.playerTouch(charAt);
                if(charAt == ' ' || charAt == '!')
                {
                    maze[p1.getX()][p1.getY() - 1] = '@';
                    maze[p1.getX()][p1.getY()] = ' ';
                    p1.setY(p1.getY() - 1);
                }
                valid = true;
                break;
            case('D'):
                charAt = maze[p1.getX() + 1][p1.getY()];
                p1.playerTouch(charAt);
                if(charAt == ' ' || charAt == '!')
                {
                    maze[p1.getX() + 1][p1.getY()] = '@';
                    maze[p1.getX()][p1.getY()] = ' ';
                    p1.setX(p1.getX() + 1);
                }
                valid = true;
                break;
            case('d'):
                charAt = maze[p1.getX() + 1][p1.getY()];
                p1.playerTouch(charAt);
                if(charAt == ' ' || charAt == '!')
                {
                    maze[p1.getX() + 1][p1.getY()] = '@';
                    maze[p1.getX()][p1.getY()] = ' ';
                    p1.setX(p1.getX() + 1);
                }
                valid = true;
                break;
            default:
                cout << "Please type a direction (wasd, upper or lowercase)."
                     << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> input;
                
                break;
        }
    } while(!valid);
}

void moveM(unsigned short randomDirection, Player &M, vector< vector<char> > &maze)
{
    char charAt;
    bool valid = false;
    do
    {
        randomDirection = static_cast<unsigned short>(rand() % 4);
        switch(randomDirection)
        {
            case(0):
                charAt = maze[M.getX()][M.getY() + 1];
                if(charAt == '@')
                {
                    M.setGameOver(true);
                    cout << "\nYou were devoured by M. Game Over." << endl;
                }
                if(!(charAt == '#' || charAt == '!' || charAt == 'E'))
                {
                    maze[M.getX()][M.getY() + 1] = 'M';
                    maze[M.getX()][M.getY()] = ' ';
                    M.setY(M.getY() + 1);
                    valid = true;
                }
                break;
            case(1):
                charAt = maze[M.getX() - 1][M.getY()];
                if(charAt == '@')
                {
                    M.setGameOver(true);
                    cout << "\nYou were devoured by M. Game Over." << endl;
                }
                if(!(charAt == '#' || charAt == '!' || charAt == 'E'))
                {
                    maze[M.getX() - 1][M.getY()] = 'M';
                    maze[M.getX()][M.getY()] = ' ';
                    M.setX(M.getX() - 1);
                    valid = true;
                }
                break;
            case(2):
                charAt = maze[M.getX()][M.getY() - 1];
                if(charAt == '@')
                {
                    M.setGameOver(true);
                    cout << "\nYou were devoured by M. Game Over." << endl;
                }
                if(!(charAt == '#' || charAt == '!' || charAt == 'E'))
                {
                    maze[M.getX()][M.getY() - 1] = 'M';
                    maze[M.getX()][M.getY()] = ' ';
                    M.setY(M.getY() - 1);
                    valid = true;
                }
                break;
            case(3):
                charAt = maze[M.getX() + 1][M.getY()];
                if(charAt == '@')
                {
                    M.setGameOver(true);
                    cout << "\nYou were devoured by M. Game Over." << endl;
                }
                   if(!(charAt == '#' || charAt == '!' || charAt == 'E'))
                {
                    maze[M.getX() + 1][M.getY()] = 'M';
                    maze[M.getX()][M.getY()] = ' ';
                    M.setX(M.getX() + 1);
                    valid = true;
                }
                break;
        }
    } while(!valid);
}
