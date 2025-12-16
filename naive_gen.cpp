using namespace std;
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
//function of the program:
//naive-gen width height mcount
#define MAX_WIDTH 50
#define MIN_WIDTH 9
#define MAX_HEIGHT 50
#define MIN_HEIGHT 9
#define MAX_DENSITY 0.25


int width;
int height;
int mineCount;
int area;

bool isTileMined[MAX_WIDTH+2][MAX_HEIGHT+2];
char lineBuffer[2 * MAX_WIDTH + 1];

int main(int argc, char* argv[]){

    //invalidate improper input count
    if (argc != 4){
        cout << "usage: naive-gen width height mcount\n";
        return 1; 
    }

    //collect input parameters
    width = stoi(argv[1]);
    height = stoi(argv[2]);
    mineCount = stoi(argv[3]);
    

    //invalidate params outside of defined dimensions
    if (width > MAX_WIDTH || width < MIN_WIDTH){
        cout << "Input width should be an integer between 9 and 50; exiting\n";
        return 1;
    }

    if (height > MAX_HEIGHT || height < MIN_HEIGHT){
        cout << "Input length should be an integer between 9 and 50; exiting\n";
        return 1;
    }

    if (mineCount < 0){
        cout << "Input mine count should be non-negative; exiting\n";
        return 1;
    }
    area = width * height;
    if (mineCount > (int)(MAX_DENSITY * area)){
        cout << "Input mine exceeded maximum mine density of 25%; exiting\n";
        return 1;
    }
    

    //properly initialize booleans;
    memset(isTileMined, 0, sizeof(isTileMined));
    
    //seed random number generator
    srand(time({}));


    
    //pick spawn coordinates
    int spawnX = (int)ceil(((double)rand()* width) / RAND_MAX);
    int spawnY = (int)ceil(((double)rand()* height) / RAND_MAX);
    if (!spawnX || !spawnY){
        cout << "Error: generated spawning coords are invalid; exiting\n";
        return 1;
    }
    cout << "spawning in at: x: " << spawnX << ", y: " << spawnY << "\n"; 
    
    for(int i = 0; i <= width; i++){
        lineBuffer[2 * i + 1] = ' ';
    }
    lineBuffer[2 * width - 1] = '\n';
    lineBuffer[2 * width] = '\0';
    
    ofstream rawMineMap("outputs/rawmines.txt");

    //generate rawMineMap
    for(int i = 1; i <= height; i++){
        for(int j = 1; j <= width; j++){
            if (((double)rand() / RAND_MAX) < ((double)mineCount / area) && 
                abs(j - spawnX) > 1 && abs(i - spawnY) > 1){

                //set mine status
                isTileMined[i][j] = 1; 
                mineCount--;

            }
            //push corresponding mine status
            lineBuffer[2*(j-1)] = (isTileMined[i][j]) ? '1' : '0';
            area--;
        }
        //mark spawn point
        if (i == spawnY) 
            lineBuffer[2 * spawnX - 2] = 'X';

        //push line into file
        rawMineMap << lineBuffer;

    }

    //close file
    rawMineMap.close();

    //generate minefield
    ofstream mineField("outputs/minefield.txt");
    int currentTileMineCount;

    //generate mineField
     for(int i = 1; i <= height; i++){
        for(int j = 1; j <= width; j++){
            currentTileMineCount = 
                isTileMined[i+1][j+1] + 
                isTileMined[i][j+1] + 
                isTileMined[i-1][j+1] + 
                isTileMined[i+1][j] + 
                isTileMined[i-1][j] + 
                isTileMined[i+1][j-1] + 
                isTileMined[i][j-1] + 
                isTileMined[i-1][j-1];
            lineBuffer[2*(j-1)] = (isTileMined[i][j])? 'M' : '0' + currentTileMineCount;
        }
        if (i == spawnY) 
            lineBuffer[2 * spawnX - 2] = 'X';
        mineField << lineBuffer;
    }

    mineField.close();
    return 0;
}
