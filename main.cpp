#include <string>
#include <fstream>
#include <iostream>

using namespace std;

struct cell{

	bool wall;
	bool visited;
};

struct maze_props{

	int init_row;
	int init_col;
	int goal_row;
	int goal_col;

};


void process_line(cell** maze, string curr_line, int curr_height, maze_props &props)
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
			props.init_row = curr_height;
			props.init_col = curr_col;
		}
		else if(test == '.')
		{
			//goal
			maze[curr_height][curr_col].wall = false;
			props.goal_row = curr_height;
			props.goal_col = curr_col;
		}
		else
		{
			//open space 
			maze[curr_height][curr_col].wall = false;
		}
		maze[curr_height][curr_col].visited = false;
		curr_col++;
	}

}


int main(void)
{

	// allocate on heap
	int small_cols = 0;
	int small_rows = 0;

	ifstream myfile ("mediumMaze.txt");
	//cout << "Unable to open file." << endl;
	//std::ofstream outfile ("output_small.txt");

	cell** maze;
	maze_props props;
	props.init_row = -1;
	props.init_col = -1;
	props.goal_row = -1;
	props.goal_col = -1;

	if(myfile.is_open())
	{
		string curr_line;

		if(getline(myfile, curr_line) == NULL) {
			cout << "Error: no lines read " << endl;
			return 1;
		}
		
		small_cols = curr_line.length();
		maze = new cell*[small_cols];
		process_line(maze, curr_line, small_rows, props);
		small_rows++;

		while(getline(myfile, curr_line))
		{
			process_line(maze, curr_line, small_rows, props);
			small_rows++;
		}
	}

	/*
	for(int i=0; i<small_rows; i++) {
		for (int j=0; j<small_cols; j++) {
			cout << ((maze[i][j].wall)? 'W':' ') ;
		}
		cout << endl;
	}
	*/

	/*
	cout << "The init is at row: " << props.init_row << " and col: " << props.init_col << endl;
	cout << "The goal is at row: " << props.goal_row << " and col: " << props.goal_col << endl;
	*/

	

}
