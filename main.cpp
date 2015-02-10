#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <unistd.h>

using namespace std;

#define DEBUG true

struct cell{
	
	bool wall;
	bool visited;
	
	int x;
	int y;
	
	cell() : visited(false) {}
};

struct maze_props{

	int init_row;
	int init_col;
	int goal_row;
	int goal_col;
	int num_rows;
	int num_cols;
	//
	cell* start;
	cell* goal;
};

struct node{
	node* parent;

	int row;
	int col;

	node* childA;
	node* childB;
	node* childC;
	node* childD;
};

/*
int calc_path_cost(node* curr_node, maze_props &props, int curr_cost)
{
	if(curr_node->row == props.goal_row && curr_node->col == props.goal_col)
	{
		return curr_cost;
	}


}
*/


bool frontierCheckPush(queue<cell*>& frontier, cell** maze, maze_props props, int x, int y, int &num_expansions/*, node* curr_node*/) {
	if (x<0 || y<0 || x>props.num_rows-1 || y>props.num_cols-1) {
		return false;
	}
	
	cell* candidate_cell = &(maze[x][y]);
	
	if (candidate_cell->visited)
		return false;
		
	if (!candidate_cell->wall) {
		frontier.push(candidate_cell);
		/*
		if(curr_node->childA==NULL)
		{
			node* new_node = new node;
			new_node->row = x;
			new_node->col = y;
			new_node->parent = curr_node;
			curr_node->childA=new_node;
		}
		else if(curr_node->childB==NULL)
		{
			node* new_node = new node;
			new_node->row = x;
			new_node->col = y;
			new_node->parent = curr_node;
			curr_node->childB=new_node;
		}
		else if(curr_node->childC==NULL)
		{
			node* new_node = new node;
			new_node->row = x;
			new_node->col = y;
			new_node->parent = curr_node;
			curr_node->childC=new_node;
		}
		else if(curr_node->childD==NULL)
		{
			node* new_node = new node;
			new_node->row = x;
			new_node->col = y;
			new_node->parent = curr_node;
			curr_node->childD=new_node;
		}
		*/
		num_expansions++;
		return true;
	}
	
	return false;
}

void process_line(cell** maze, string curr_line, int curr_height, maze_props &props)
{

	maze[curr_height] = new cell[curr_line.length()];
	int curr_col = 0;
	for(string::iterator it = curr_line.begin(); it != curr_line.end(); it++)
	{
		char test = *it;
		maze[curr_height][curr_col].x = curr_height;
		maze[curr_height][curr_col].y = curr_col;
		
		if(test == '%')
		{
			//wall
			maze[curr_height][curr_col].wall = true;

		}
		else if(test == 'P')
		{
			//starting point
			maze[curr_height][curr_col].wall = false;
			
			props.start = &(maze[curr_height][curr_col]);
			//
			props.init_row = curr_height;
			props.init_col = curr_col;
		}
		else if(test == '.')
		{
			//goal
			maze[curr_height][curr_col].wall = false;
			
			props.goal = &(maze[curr_height][curr_col]);
			//
			props.goal_row = curr_height;
			props.goal_col = curr_col;
		}
		else
		{
			//open space 
			maze[curr_height][curr_col].wall = false;
		}
		
		//maze[curr_height][curr_col].visited = false;
		curr_col++;
	}

}

/* Doesn't return a solution yet, since it doesn't keep track of the nodes taken */
void bfs(cell** maze, maze_props props, node* root) {

	cell* current_cell;
	
	queue< cell* > frontier;
	frontier.push( props.start );

	int num_expansions = 0;
	
	while(!frontier.empty()) {
		current_cell = frontier.front();
		current_cell->visited = true;
		frontier.pop();
		
		if (current_cell == props.goal) {
			//We done did eet
			//cout << "Path cost: " << calc_path_cost(root, props, 0) << endl;
			continue;
		}
		
		//Add all adjacent cells to frontier
		int cx = current_cell->x, cy = current_cell->y;
		frontierCheckPush(frontier, maze, props, cx+1,	cy	, num_expansions/*, root*/);
		frontierCheckPush(frontier, maze, props, cx,	cy+1, num_expansions/*, root*/);
		frontierCheckPush(frontier, maze, props, cx-1,	cy	, num_expansions/*, root*/);
		frontierCheckPush(frontier, maze, props, cx,	cy-1, num_expansions/*, root*/);
		
		//Progress In Text
		if (DEBUG) {
			for(int i=0; i<props.num_rows; i++) {
				for (int j=0; j<props.num_cols; j++) {
					if(i==props.init_row && j==props.init_col)
					{
						cout << 'P';
						continue;
					}
					if(i==props.goal_row && j==props.goal_col)
					{
						cout << 'G';
						continue;
					}

					cout << ((maze[i][j].wall)? '%':(maze[i][j].visited? '.':' ' )) ;
				}
				cout << endl;
			}
			cout << "Number of expansions: " << num_expansions << endl;

			for(int i=0; i<14; i++)
			{
				cout << endl;
			} 
			usleep(150000); //sleep 0.25 sec
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
	node* root = NULL;
	
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



	if(DEBUG) {
		cout << "The init is at row: " << props.init_row << " and col: " << props.init_col << endl;
		cout << "The goal is at row: " << props.goal_row << " and col: " << props.goal_col << endl;
	}
	
	
	for(int i=0; i<num_rows; i++) {
		for (int j=0; j<num_cols; j++) {
			cout << ((maze[i][j].wall)? 'W':' ') ;
		}
		cout << endl;
	}
	
	bfs(maze, props, root);
	
}
