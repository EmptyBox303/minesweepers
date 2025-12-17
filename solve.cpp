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

int width;
int height;
int mineCount;
int spawnX;
int spawnY;

char key[MAX_WIDTH][MAX_HEIGHT];

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

    //for now we will assume these parameters are reasonable and valid
    for(int i = 0; i < height; i++){
        //read in line from file
        getline(mineField,line);
        
        for(int j = 0; j < width; j++){
            char state = line[2 * j];
        }
    }

    //close mineField
    mineField.close();

    
    return 0;
}


//read in info
//start from starting tile

//we need to keep track of a set of "boundary" tiles that are of interest
//what tiles are of interest?
//those on the boundary?
//we start out with the spawn point tile as tile of interest

//