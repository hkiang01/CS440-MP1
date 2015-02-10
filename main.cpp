#include <string>
#include <fstream>
#include <iostream>
#include <queue>

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
	int num_rows;
	int num_cols;

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

void bfs(cell** maze, maze_props &props)
{

  	queue< pair<int, int> > frontier;

  	pair<int, int> start_point (props.init_row, props.init_col);
	maze[start_point.first][start_point.second]).visited=true;
  	frontier.push(start_point);


  	pair<int, int> curr_loc = frontier.front();
  	frontier.pop();
  	while(!frontier.empty())
  	{
	  	if(curr_loc.first == props.goal_row && curr_loc.second == props.goal_col)
	  	{
	  		// return the solution
	  	}
	  	else
	  	{
		  	if( curr_loc.first > 0 && (maze[curr_loc.first-1][curr_loc.second]).wall==false && (maze[curr+loc.first-1][curr_loc.second]).visited==false)
		  	{
		  		//can move up
		  		pair<int, int> another_loc (curr_loc.first-1, curr_loc.second);
		  		maze[curr+loc.first-1][curr_loc.second]).visited=true;
		  		frontier.push(another_loc);
		  	}
		  	if( curr_loc.second + 1 < props.num_cols && (maze[curr_loc.first][curr_loc.second+1]).wall==false && (maze[curr+loc.first][curr_loc.second+1]).visited==false)
		  	{
		  		//can move right
		  		pair<int, int> another_loc (curr_loc.first, curr_loc.second+1);
		  		maze[curr+loc.first][curr_loc.second+1]).visited=true;
		  		frontier.push(another_loc);
		  	}
		  	if( curr_loc.first+1 < props.num_rows && (maze[curr_loc.first+1][curr_loc.second]).wall==false && (maze[curr+loc.first+1][curr_loc.second]).visited==false)
		  	{
		  		//can move down
		  		pair<int, int> another_loc (curr_loc.first+1, curr_loc.second);
		  		maze[curr+loc.first+1][curr_loc.second]).visited=true;
		  		frontier.push(another_loc);
		  	}
		  	if( curr_loc.second - 1 >= 0 && (maze[curr_loc.first][curr_loc.second - 1]).wall==false && (maze[curr+loc.first][curr_loc.second]-1).visited==false)
		  	{
		  		// can move left
		  		pair<int, int> another_loc (curr_loc.first, curr_loc.second-1);
		  		maze[curr+loc.first][curr_loc.second-1]).visited=true;
		  		frontier.push(another_loc);
		  	}
		}
  	}

}


int main(void)
{

	int num_cols = 0;
	int num_rows = 0;

	ifstream myfile ("smallMaze.txt");
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
		
		num_cols = curr_line.length();
		maze = new cell*[num_cols];
		process_line(maze, curr_line, num_rows, props);
		num_rows++;

		while(getline(myfile, curr_line))
		{
			process_line(maze, curr_line, num_rows, props);
			num_rows++;
		}
	}

	props.num_rows = num_rows;
	props.num_cols = num_cols;

	for(int i=0; i<num_rows; i++) {
		for (int j=0; j<num_cols; j++) {
			cout << ((maze[i][j].wall)? 'W':' ') ;
		}
		cout << endl;
	}
	
	bfs(maze, props);
	
	cout << "The init is at row: " << props.init_row << " and col: " << props.init_col << endl;
	cout << "The goal is at row: " << props.goal_row << " and col: " << props.goal_col << endl;
	

	

}
