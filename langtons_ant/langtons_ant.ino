/*
Langton's Ant

This program displays a simulation of the Langton's Ant cellular automata:
http://en.wikipedia.org/wiki/Langton%27s_ant

Author: Duncan Lindbo
History:
  -9/21/2013: Initial version.
  -10/20/2013: Updated to better support multiple ants.

Writen for the LoL Shield, designed by Jimmie Rodgers: 
http://jimmieprodgers.com/kits/lolshield/

This needs the Charliplexing library, which you can get at the 
LoL Shield project page: http://code.google.com/p/lolshield/
 
*/

#include <Charliplexing.h>

#define DELAY 150
#define SIZEX DISPLAY_COLS    //Sets the X axis size
#define SIZEY DISPLAY_ROWS    //Sets the Y axis size
//how many ants: more is more interseting visually, and makes it harder to see the pattern
const byte numberOfAnts=4;
byte world[SIZEX][SIZEY][2];
//array of ants: each ant has an x-position, y-position, and current direction
//ants[ANT][0]: x-position
//ants[ANT][1]: y-position 
//ants[ANT][2]: current direction: 1 = +y, 2 = +x, 3 = -y, 4 = -x
byte ants[numberOfAnts][3];

void setup(){
  //Initilizes the LoL Shield
  LedSign::Init();
  randomSeed(analogRead(5));
  spawnAnts();
}

void loop(){
  for(byte i=0; i < numberOfAnts; i++){
    boolean on = world[ants[i][0]][ants[i][1]][0];
    //flip the state of the led (on/off)
    world[ants[i][0]][ants[i][1]][1] = !on;
    //if the LED was on, turn the ant 90 degrees right
    if(on){
      ants[i][2] = (ants[i][2]%4) + 1;
    }
    //if the LED was off, turn the ant 90 degrees left
    else{
      ants[i][2] = ((ants[i][2]+2)%4)+1;           
    }
    //move the ant forward one unit
    switch(ants[i][2]){
        case 1:
          ants[i][1]=(ants[i][1]+1)%SIZEY;
          break;
        case 2:
          ants[i][0]=(ants[i][0]+1)%SIZEX;
          break;
        case 3:
          ants[i][1]=(ants[i][1]+SIZEY-1)%SIZEY;
          break;
        case 4:
          ants[i][0]=(ants[i][0]+SIZEX-1)%SIZEX;
          break;
        default:
          //should never happen...
          ants[i][1]=(ants[i][1]+1)%SIZEY;
          break;
      }
  }
  for (byte x = 0; x < SIZEX; x++) { 
    for (byte y = 0; y < SIZEY; y++) {
      world[x][y][0] = world[x][y][1];
      LedSign::Set(x, y, world[x][y][1]);
    }
  }
  delay(DELAY);
}

void spawnAnts(){
  for(byte i=0; i < numberOfAnts; i++){
    //pick a random point and direction to start each ant
    //TODO: make sure ants are evenly distributed/can't spawn on top of each other
    ants[i][0] = random(0, SIZEX);
    ants[i][1] = random(0, SIZEY);
    ants[i][2] = random(1, 5);
  }  
}
