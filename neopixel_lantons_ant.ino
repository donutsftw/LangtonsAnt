/*
Langton's Ant

This program displays a simulation of the Langton's Ant cellular automata:
http://en.wikipedia.org/wiki/Langton%27s_ant

Author: Duncan Lindbo
History:
  -2/15/2014: Initial version.
  -4/30/2015: Expanded to 16x16
 
*/

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 6
#define DELAY 150
#define SIZEX 16    //Sets the X axis size
#define SIZEY 16    //Sets the Y axis size
//how many ants: more is more interesting visually, and makes it harder to see the pattern
const byte numberOfAnts=1;
byte world[SIZEX][SIZEY][2];
//array of ants: each ant has an x-position, y-position, and current direction
//ants[ANT][0]: x-position
//ants[ANT][1]: y-position 
//ants[ANT][2]: current direction: 1 = +y, 2 = +x, 3 = -y, 4 = -x
byte ants[numberOfAnts][3];

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, PIN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_COLUMNS   + NEO_TILE_ZIGZAG +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB + NEO_KHZ800);

uint16_t color = matrix.Color(0, 0, 255);

const uint16_t colors[] = {matrix.Color(0, 0, 0), matrix.Color(0, 0, 255), matrix.Color(0, 255, 0), matrix.Color(255, 0, 0) };
const char directions[] = {'L', 'R', 'L', 'R'};
const int numStates = 4;

void setup(){
  matrix.begin();
  matrix.setBrightness(40);
  randomSeed(analogRead(5));
  spawnAnts();
}

void loop(){
  for(byte i=0; i < numberOfAnts; i++){
    byte state = world[ants[i][0]][ants[i][1]][0];
    char instruction = directions[state];
    world[ants[i][0]][ants[i][1]][1] = (++state)%numStates;
    //turn the ant 90 degrees right
    if(instruction == 'R'){
      ants[i][2] = (ants[i][2]%4) + 1;
    }
    //turn the ant 90 degrees left
    else if(instruction == 'L'){
      ants[i][2] = ((ants[i][2]+2)%4)+1;           
    }
    //no turn
    else if(instruction == 'N'){
    }
    //
    else if(instruction == 'U'){
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
      //if(world[x][y][1]){
        //matrix.drawPixel(x, y, color);
      //}
      //else{
        //matrix.drawPixel(x, y, 0);
      //}
      //LedSign::Set(x, y, world[x][y][1]);
      matrix.drawPixel(x, y, colors[world[x][y][1]]);
    }
  }
  matrix.show();
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
