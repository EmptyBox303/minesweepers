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

    pointSolveProgress = true;
    int iteration = 1;
    int X;
    int Y;
    pair<int,int> currentTile;

    //we loop the point solve algorithm until no more pointSolveProgress is made, or no pointSolveProgress
    while (pointSolveProgress && tileOfInterest.size()){
        //start out assuming no progress is made this iteration
        pointSolveProgress = false;

        cout << "iteration " << iteration << ":\n";
    
        
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


        cout << "flagging...\n";
        print_current_config();
        cout << "\n";
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
         
       
        cout << "chording...\n";
        print_current_config();
        cout << "\n";
        
        
        //print_current_config();
        cout << "iteration " << iteration <<  " ended. \n";
        iteration++;

    }
   
    

            
    

    
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
    if((i || j) && !revealed[Y+i][X+j]) ret++;
    return ret;
}

int flagged_neighbors_count(int X, int Y){
    int ret = 0;
    for (int i = -1; i <= 1; i++) 
    for(int j = -1; j <= 1; j++)
    if((i || j) && flagged[Y+i][X+j]) ret++;
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
                add_tile_of_interest(X+j,Y+i);
                //cout << "cleared tile " << X+j << "," << Y+i << " \n";
            }
        }
    }
    
    
}

void add_tile_of_interest(int X, int Y){
    if (X >= 1 && X <= width && Y >= 1  && Y <= height){
        tileOfInterest.push({X,Y});
        pointSolveProgress = true;
        //cout << "tile " << X << "," << Y << "(" << (int)key[Y][X]<< ")" << " is added to list of tiles of interest\n";
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





/*
2 3 2 1 1 2 2 1 0 1 1 1 0 0 0 0 0 1 1 1
M M M 1 1 M M 1 0 1 M 1 0 0 0 0 0 1 M 1
2 3 2 1 1 2 2 1 0 1 1 1 0 0 0 0 0 1 1 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 x 0 0 0 0 0
0 0 0 0 1 2 2 1 0 1 2 2 1 0 0 0 0 0 0 0
0 0 0 0 1 M M 2 0 1 M M 2 0 0 0 0 0 0 0
1 1 0 1 2 4 M 2 0 1 3 M 2 0 0 0 0 1 1 1
M 2 2 3 M 3 2 2 0 1 2 2 1 0 0 0 0 2 M 2
2 3 M M 3 3 M 2 1 2 M 3 2 1 0 0 0 2 M 3
1 M 4 5 M 3 1 2 M 4 4 M M 2 0 1 1 2 2 M
1 1 2 M M 4 1 3 4 M M 5 M 3 0 2 M 2 1 1
0 1 3 6 M 4 M 2 M M 3 3 M 2 0 2 M 2 1 1
0 1 M M M 3 2 4 4 3 1 1 1 1 0 1 1 1 1 M
1 2 2 3 3 2 2 M M 1 0 0 0 0 0 0 1 2 3 2
M 1 0 0 1 M 4 5 5 4 3 2 1 0 0 1 3 M M 3
1 1 0 0 1 2 M M M M M M 1 0 0 1 M M M M

U U U U U U U U U U U 1 0 0 0 0 0 1 U U
U U U U U U U U U U U 1 0 0 0 0 0 1 U U
U U U U U U U U U 1 1 1 0 0 0 0 0 1 1 1
U U U U U U U U U 0 0 0 0 0 X 0 0 0 0 0
U U U U U U U U U 1 2 2 1 0 0 0 0 0 0 0
U U U U U U U U U U U U 2 0 0 0 0 0 0 0
U U U U U U U U U U U U 2 0 0 0 0 1 1 1
U U U U U U U U U U U U 1 0 0 0 0 2 U U
U U U U U U U U U U U U 2 1 0 0 0 2 U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U

U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U C C C U U U U U
U U U U U U U U U U U U C C C C U U U U 
U U U U U U U U U U U U C C X C U U U U 
U U U U U U U U U U U U U C C C U U U U 
U U U U U U U U U U U U U U U U U U U U 
U U U U U U U U U U U U U U U U U U U U 
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
U U U U U U U U U U U U U U U U U U U U
*/
