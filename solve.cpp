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
#include <set>

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
char reducedBoard[MAX_HEIGHT+2][MAX_WIDTH+2];

bool pointSolveProgress;

char line[2*MAX_WIDTH + 1];
//tiles 1 to tile w

queue<pair<int,int>> tileOfInterest;
queue<pair<int,int>> reviewedTiles;

int main(){
    //open mineField for reading
    ifstream mineField("outputs/minefield.txt");

    //prepping buffer
    string readBuffer;

    //kill program if minefield not found
    if (!mineField.is_open()){
        cout << "couldn't open minefield; exiting\n";
        return 1;
    }

    //read out parameters
    getline(mineField,readBuffer);
    stringstream params(readBuffer);
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
        getline(mineField,readBuffer);
        for(int j = 0; j < width; j++){
            char c = readBuffer[2 * j];
            key[i+1][j+1]  = (c == 'M') ? 0xff : (c-0x30);
        }
        
    }

    //close mineField
    mineField.close();

    //SETUP: is mine marker
    memset(flagged, 0, sizeof(flagged));
    memset(revealed, 0, sizeof(revealed));

    for(int i = 0; i < height + 2; i++){
        revealed[i][0] = 1;
        revealed[i][width + 1] = 1;
    }
    for(int i = 1; i < width+1; i++){
        revealed[0][i] = 1;
        revealed[height+1][i] = 1;
    }

    //SETUP: tile of interest
    //SETUP: line
    for(int i = 0; i < width-1; i++){
        line[2*i + 1] = ' ';
    }
    line[2*width - 1] = '\n';
    line[2*width] = 0;


    //insert spawn tile as tile of interest
    tileOfInterest.push({spawnX,spawnY});
    revealed[spawnY][spawnX] = 1;



    
    int iteration = simple_point_solve();
   
    cout << "Simple point solve steps ended with " << iteration << " iterations.\n";
    cout << "Current board state: \n";
    print_current_config();
            
    

    
    return 0;
}


//read in info
//start from starting tile

//we need to keep track of a set of "boundary" tiles that are of interest
//what tiles are of interest?
//those on the boundary?
//we start out with the spawn point tile as tile of interest

//
bool is_actual_tile(int X, int Y){
    return (X >= 1 && X <= width && Y >= 1  && Y <= height);
}

int uncleared_neighbors_count(int X, int Y){
    int ret = 0;
    for (int i = -1; i <= 1; i++) 
    for(int j = -1; j <= 1; j++)
    if((i || j) && !revealed[Y+i][X+j]) ret ++;
    return ret;
}

int flagged_neighbors_count(int X, int Y){
    int ret = 0;
    for (int i = -1; i <= 1; i++) 
    for(int j = -1; j <= 1; j++)
    if((i || j) && flagged[Y+i][X+j]) ret++;
    return ret;
}

int reduced_tile_num(int X, int Y){
    //only unflagged && 
    int ret = 0;
    for (int i = -1; i <= 1; i++) 
    for(int j = -1; j <= 1; j++)
    if((i || j) && key[Y+i][X+j] == 0xff && !flagged[Y+i][X+j]) ret++;
    return ret;
}

void flag_uncleared_neighbors(int X, int Y){
    for (int i = -1; i <= 1; i++) 
    for(int j = -1; j <= 1; j++)
    if((i || j) && !revealed[Y+i][X+j]) flagged[Y+i][X+j] = true;
}

void clear_unflagged_neighbors(int X, int Y){
    for (int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if((i || j) && !revealed[Y+i][X+j] && !flagged[Y+i][X+j]){     
                revealed[Y+i][X+j] = true;
                pointSolveProgress = true;
                tileOfInterest.push({X+j,Y+i});
                         
                //cout << "cleared tile " << X+j << "," << Y+i << " \n";
            }
        }
    }
    
    
}


void print_current_config(){
    for(int i = 1; i <= height; i++){
        for(int j = 1; j <= width; j++){
            if (!revealed[i][j]){
                line[2*(j-1)] = (flagged[i][j]) ? 'F' : ' ';
            }
            else line[2*(j-1)] = 0x30 + key[i][j];
        }
        if (i == spawnY){
            line[2*(spawnX-1)] = 'X';
        }
        cout << line;
    }
}



void restore_tiles_of_interest(){
    while(!reviewedTiles.empty()){
        tileOfInterest.push(reviewedTiles.front()); reviewedTiles.pop();
    }
}

int simple_point_solve(){
    pointSolveProgress = true;
    int iteration = 1;
    int X;
    int Y;
    pair<int,int> currentTile;

    //we loop the point solve algorithm until no more pointSolveProgress is made, or no pointSolveProgress
    while (pointSolveProgress && tileOfInterest.size()){
        //start out assuming no progress is made this iteration
        pointSolveProgress = false;

        //cout << "iteration " << iteration << ":\n";
    
        
        //the goal of each "section" of the algorithm is to comb over each TOI once during each iteration

        //section 1: flag
        while(!tileOfInterest.empty()){

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
                pointSolveProgress = true;

                //this tile is implicitly removed
            }

            //otherwise return currenttile back to pile
            else reviewedTiles.push(currentTile);
            
        }
        restore_tiles_of_interest();
        int discardedCount = 0;
        //section 2: chord
        while(!tileOfInterest.empty()){
            
            currentTile = tileOfInterest.front();
            tileOfInterest.pop();

            //extract coordinates of tile
            X = currentTile.first;
            Y = currentTile.second;

            //if flagged neighbors = required flag count for this tile
            if (flagged_neighbors_count(X,Y) == key[Y][X] || !key[Y][X]){
                
                clear_unflagged_neighbors(X,Y);
                //cout << "tile " << X << "," << Y  << "(" << (int)key[Y][X]<< ")"<< " is chorded\n";
                

                discardedCount++;
            }

            //otherwise return currenttile back to pile
            else{
                reviewedTiles.push(currentTile);
            } 

          
        }
        restore_tiles_of_interest();
         
        
        
        //print_current_config();
        //cout << "iteration " << iteration <<  " ended. \n";
        iteration++;

    }
    return iteration;
}

