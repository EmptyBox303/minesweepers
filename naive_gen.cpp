using namespace std;
#include <iostream>
#include <string>
//function of the program:
//naive-gen width height mcount
#define MAX_WIDTH 50
#define MIN_WIDTH 9
#define MAX_HEIGHT 50
#define MAX_DENSITY 0.25


int width;
int height;
int mineCount;


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

    if (mineCount > (int)(MAX_DENSITY * width * height)){
        cout << "Input mine exceeded maximum mine density of 25%; exiting\n";
        return 1;
    }
    

    
    return 0;
}
