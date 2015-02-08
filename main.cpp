#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct cell {
  bool north;
  bool south;
  bool west;
  bool east;
  bool visited;
  int acc_dist;
};

int main() {
  
  int height, width;

  cell** maze = new int*[height];
  for (int i=0; i<height; ++i) {
    maze[i] = new cell[width];
  }

  /*
   * Parse
   */
  

}
