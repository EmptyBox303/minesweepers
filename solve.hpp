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

int uncleared_neighbors_count(int X, int Y);

int flagged_neighbors_count(int X, int Y);

void flag_uncleared_neighbors(int X, int Y);

void clear_unflagged_neighbors(int X, int Y);

void print_current_config();

void restore_tiles_of_interest();