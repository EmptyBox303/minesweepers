using namespace std;

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <utility>
#include <queue>
#include <vector>
#include "head.hpp"
#include "solve.hpp"

int width;
int height;
int mineCount;
int spawnX;
int spawnY;

char key[MAX_HEIGHT+2][MAX_WIDTH+2];
bool flagged[MAX_HEIGHT+2][MAX_WIDTH+2];
bool revealed[MAX_HEIGHT+2][MAX_WIDTH+2];
bool progress;
int combCount;

queue<pair<int,int>> tileOfInterest;

int main(){
    //open mineField for reading
    ifstream mineField("outputs/minefield.txt");

    //prepping buffer
    string line;

    //kill program if minefield not found
    if (!mineField.is_open()){
        cout << "couldn't open minefield; exiting\n";
        return 1;
    }

    //read out parameters
    getline(mineField,line);
    stringstream params(line);
    string token;
    vector<string> tokens;
    while(getline(params,token,' ')){
        tokens.push_back(token);
    }

    //convert parameters to numerics
    width = stoi(tokens[0]);
    height = stoi(tokens[1]);
    mineCount = stoi(tokens[2]);
    spawnX = stoi(tokens[3]);
    spawnY = stoi(tokens[4]);

    //nullproof keys
    for(int i = 0; i < height + 2; i++){
        key[i][0] = 0;
        key[i][width + 1] = 0;
    }
    for(int i = 1; i < width+1; i++){
        key[0][i] = 0;
        key[height+1][i] = 0;
    }


    //for now we will assume these parameters are reasonable and valid
    for(int i = 0; i < height; i++){
        //read in line from file
        getline(mineField,line);
        for(int j = 0; j < width; j++){
            char c = line[2 * j];
            key[i+1][j+1]  = (c == 'M') ? 0xff : (c-0x30);
        }
        
    }

    //close mineField
    mineField.close();

    //SETUP: is mine marker
    memset(flagged, 0, sizeof(flagged));
    memset(revealed, 0, sizeof(revealed));

    //SETUP: tile of interest
    

    //insert spawn tile as tile of interest
    tileOfInterest.push({spawnX,spawnY});
    revealed[spawnY][spawnX] = 1;

    progress = true;
    int iteration = 0;
    int X;
    int Y;
    pair<int,int> currentTile;

    //we loop the point solve algorithm until no more progress is made, or no progress
    while (progress && tileOfInterest.size()){
        //start out assuming no progress is made this iteration
        progress = false;
        
        //the goal of each "section" of the algorithm is to comb over each TOI once during each iteration

        //section 1: flag
        combCount = tileOfInterest.size();
        for(int i = 0; i < combCount; i++){

            //extract current tile of interest
            currentTile = tileOfInterest.front();
            tileOfInterest.pop();
            
            //extract coordinates of tile
            X = currentTile.first;
            Y = currentTile.second;

            //if uncleared neighbors = required flag count for this tile
            if ((uncleared_neighbors_count(X,Y) == key[Y][X])){

                //flag all uncleared tiles, this is progress
                flag_uncleared_neighbors(X,Y);
                progress = true;

                //this tile is implicitly removed
            }

            //otherwise return currenttile back to pile
            else tileOfInterest.push(currentTile);
            
        }
        
        //section 2: chord
        combCount = tileOfInterest.size();
        for(int i = 0; i < combCount; i++){
            //extract current tile of interest
            currentTile = tileOfInterest.front();
            tileOfInterest.pop();

            //extract coordinates of tile
            X = currentTile.first;
            Y = currentTile.second;

            //if flagged neighbors = required flag count for this tile
            if (flagged_neighbors_count(X,Y) == key[Y][X]){
                //clear all unflagged neighbors
                clear_unflagged_neighbors(X,Y);
                
                //this tile is implicitly removed
            }

            //otherwise return currenttile back to pile
            else tileOfInterest.push(currentTile);

        }
         
       
        cout << "iteration " << iteration << "\n";
        iteration++;
    }
   
    cout << "Point solve algorithm complete; printing results:\n";
    
            
    

    
    return 0;
}


//read in info
//start from starting tile

//we need to keep track of a set of "boundary" tiles that are of interest
//what tiles are of interest?
//those on the boundary?
//we start out with the spawn point tile as tile of interest

//


int uncleared_neighbors_count(int X, int Y){
    int ret = 0;
    for (int i = -1; i <= 1; i++) 
    for(int j = -1; j <= 1; j++)
    if(i && j && !revealed[Y+i][X+j]) ret++;
    return ret;
}

int flagged_neighbors_count(int X, int Y){
    int ret = 0;
    for (int i = -1; i <= 1; i++) 
    for(int j = -1; j <= 1; j++)
    if(i && j && flagged[Y+i][X+j]) ret++;
    return ret;
}

void flag_uncleared_neighbors(int X, int Y){
    for (int i = -1; i <= 1; i++) 
    for(int j = -1; j <= 1; j++)
    if(i && j && !revealed[Y+i][X+j]) flagged[Y+i][X+j] = true;
}

void clear_unflagged_neighbors(int X, int Y){
    for (int i = -1; i <= 1; i++) 
    for(int j = -1; j <= 1; j++)
    if(i && j && !revealed[Y+i][X+j] && !flagged[Y+i][X+j]){
        revealed[Y+i][X+j] = true;
        add_tile_of_interest(X+j,Y+i);
    }
    
}

void add_tile_of_interest(int X, int Y){
    if (X >= 1 && X <= width && Y >= 1  && Y <= height){
        tileOfInterest.push({X,Y});
        progress = true;
        combCount++;
    }
}
