using namespace std;
#include <stdlib>

//function of the program:
//naive-gen width height mcount
#define MAX_WIDTH 50
#define MAX_HEIGHT 50
#define MAX_DENSITY 0.25

int main(int argc, char* argv[]){

    //invalidate improper input
    if (argc != 4){
        cout << "usage: naive-gen width height mcount\n";
        return 1; 
    }
    
    int width = stoi(argv[2]);
    int height = stoi(argv[3]);
    int mineCount = stoi(argv[4]);
    
    
    return 0;
}
