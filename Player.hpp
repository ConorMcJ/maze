//
//  Player.hpp
//  maze
//
//  Created by Conor McJannett on 7/26/22.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>

class Player
{
private:
    unsigned short x, y;
    bool gameOver;
    bool hasKey;
    
public:
    //constructor
    Player(unsigned short xStart, unsigned short yStart);
    
    //getters
    unsigned short getX() {return x;}
    unsigned short getY() {return y;}
    bool getGameOver() {return gameOver;}
    bool getHasKey() {return hasKey;}
    
    //setters
    void setX(const unsigned short newX) {x = newX;}
    void setY(const unsigned short newY) {y = newY;}
    void setGameOver(const bool TorF) {gameOver = TorF;}
    void setHasKey(const bool TorF) {hasKey = TorF;}
    
    //interaction function
    void playerTouch(char object);
};


#endif /* Player_hpp */

