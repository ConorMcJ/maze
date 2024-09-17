//
//  Player.cpp
//  maze
//
//  Created by Conor McJannett on 7/26/22.
//

#include "Player.hpp"
#include <iostream>
using namespace std;


//constructor function
Player::Player(unsigned short startX, unsigned short startY):
               x(startX), y(startY), gameOver(false), hasKey(false) { }

/* Function that lets the player touch objects and figures out what to do
 based on what the player touches*/
void Player::playerTouch(char object)
{
    switch(object)
    {
        case('E'):
            if(Player::getHasKey())
            {
                Player::setGameOver(true);
                cout << "\nYou escaped! Congratulations!" << endl;
            }
            else
            {
                cout << "\nYou cannot exit yet. You do not have the key."
                     << endl;
                
            }
            break;
        case('!'):
            Player::setHasKey(true);
            cout << "\nYou found the key! Now get to the exit!"
                      << endl;
            break;
        case('M'):
            Player::setGameOver(true);
            cout << "\nYou were devoured by M. Game Over." << endl;
            break;
        case('#'):
            cout << "\nYou cannot move there." << endl;
            break;
    }
}


