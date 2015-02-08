#include <fstream>
#include <iostream>
using namespace std;

struct cell{

	bool wall;
};


void process_line(cell** maze, string curr_line, int curr_height)
{

	maze[curr_height] = new cell[curr_line.length()];
	int curr_col = 0;
	for(string::iterator it = curr_line.begin(); it != curr_line.end(); it++)
	{
		char test = *it;
		if(test == '%')
		{
			//wall
			maze[curr_height][curr_col].wall = true;

		}
		else if(test == 'P')
		{
			//starting point
			maze[curr_height][curr_col].wall = false;
		}
		else if(test == '.')
		{
			//goal
			maze[curr_height][curr_col].wall = false;
		}
		else
		{
			//open space 
			maze[curr_height][curr_col].wall = false;
		}
		curr_col++;
	}

}


int main(void)
{

	// allocate on heap
	int small_cols = 0;
	int small_rows = 0;

	ifstream myfile ("smallMaze.txt");
	//std::ofstream outfile ("output_small.txt");

	if(myfile.is_open())
	{
		string curr_line;


		if(getline(myfile, curr_line) == null) {
			cout << "Error: no lines read " << endl;
			return;
		}
		
		small_cols = curr_line.length();
		cell** maze = new cell*[small_cols];
		process_line(maze, curr_line, small_rows);
		small_rows++;

		while(getline(myfile, curr_line))
		{
			process_line(maze, curr_line, small_rows);
			small_rows;
		}
	}
	else
	{
		cout << "Unable to open file." << std::endl;
	}
}