#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <unistd.h> //for sleep
#include <stack>
#include <vector>
#include <math.h>
#include <cmath>
//referenced: cplusplus.com for use of libraries for...
//fstream, iostream, sleep in unistd.h, stack, queue, priority queue, math

using namespace std;

#define DEBUG true
#define DEBUG_INIT true

struct cell{
	
	bool wall;
	bool visited;
	cell* previous;
	
	int x;
	int y;

	double manhattan_dist;
	int step_cost;

	cell() : visited(false), previous(NULL), manhattan_dist(0), step_cost(0) {}
};

//referenced https://stackoverflow.com/questions/9178083/priority-queue-for-user-defined-types
//for the priority queue in greedy best first search
struct Comp {
	bool operator()(cell const& cell_1, cell const& cell_2)
	{
		return (cell_1.manhattan_dist + cell_1.step_cost) < (cell_2.manhattan_dist + cell_2.step_cost); 
	}
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

void memory_cleanup(cell** maze, maze_props props)
{
	for(int i = props.num_rows-1; i>=0; i--)
	{
		delete[] maze[i];
	}
	delete[] maze;
	return;
}

void print_solution_bfs(cell** maze, maze_props props, int offset) {
	cout << "Solution: " << endl;
	
	if(!(props.goal)->visited) {
		cout << "Could not find path from starting point to finish point!" << endl;
		return;
	}
	
	cell* current_cell = props.goal;
	
	vector<cell*> path;
	int path_length = 0;
	
	while(current_cell != NULL) {
		if (DEBUG)
			cout << current_cell->x << "," << current_cell->y << "    ";
		usleep(1500);
		path.push_back(current_cell);
		++path_length;
		current_cell = current_cell->previous;
	}
	
	cout << endl;
	cout << "Found path of length " << path_length + offset << endl;
	for(int i = path_length-1; i>=0; i--) {
		cout << "(" << path[i]->x << "," << path[i]->y << ") -> " ;
	}
	cout << endl;
}

void print_solution_dfs(cell** maze, maze_props props) {
	props.goal->visited = true;
	print_solution_bfs(maze, props, -1);

}

void print_progress(maze_props props, cell** maze, int expansions)
{
	//Progress In Text
	for(int i=0; i<props.num_rows; i++) {
		for (int j=0; j<props.num_cols; j++) {
			if(i==props.start->y && j==props.start->x)
			{
				cout << 'P';
				continue;
			}
			if(i==props.goal->y && j==props.goal->x)
			{
				cout << 'G';
				continue;
			}

			cout << ((maze[i][j].wall)? '%':(maze[i][j].visited? '.':' ' )) ;
		}
		cout << endl;
	}
	//cout << "Current depth: " << depth << endl;
	cout << "Nodes expanded: " << expansions << endl;
	//cout << "Cost: " << cost << endl;

	for(int i=0; i<12; i++)
	{
		cout << endl;
	} 
	usleep(55000); //sleep 0.25 sec
}

bool frontierCheckPush_bfs(queue<cell*>& frontier, cell** maze, maze_props props, cell* previous_cell, int y, int x) {
	if (x<0 || y<0 || x>props.num_cols-1 || y>props.num_rows-1) {
		return false;
	}
	
	cell* candidate_cell = &(maze[y][x]);
	
	if (candidate_cell->visited)
		return false;
		
	if (!candidate_cell->wall) {
		frontier.push(candidate_cell);
		candidate_cell->previous = previous_cell;
		return true;
	}
	
	return false;
}

//the only difference with the above is that the first parameter is a stack instead of a queue
bool frontierCheckPush_dfs(stack<cell*>& frontier, cell** maze, maze_props props, cell* previous_cell, int y, int x) {

	if (x<0 || y<0 || y>props.num_rows-1 || x>props.num_cols-1) {
		return false;
	}

	
	cell* candidate_cell = &(maze[y][x]);
	
	if (candidate_cell->visited)
		return false;
		
	if (!candidate_cell->wall) {
		frontier.push(candidate_cell);
		candidate_cell->previous = previous_cell;
		return true;
	}
	
	return false;
}

//the only difference with the above is that the first parameter is a priority queue instead of a queue
bool frontierCheckPush_greedy(priority_queue<cell*>& frontier, cell** maze, maze_props props, cell* previous_cell, int y, int x) {

	if (x<0 || y<0 || y>props.num_rows-1 || x>props.num_cols-1) {
		return false;
	}
	
	cell* candidate_cell = &(maze[y][x]);
	
	if (candidate_cell->visited)
		return false;
		
	if (!candidate_cell->wall) {
		candidate_cell->previous = previous_cell;
		frontier.push(candidate_cell);
		return true;
	}
	
	return false;
}

//the only difference with the above is that before pushing onto the stack the step cost is calculated
bool frontierCheckPush_astar(priority_queue<cell*>& frontier, cell** maze, maze_props props, cell* previous_cell, int y, int x) {
		if (x<0 || y<0 || y>props.num_rows-1 || x>props.num_cols-1) {
		return false;
	}
	
	cell* candidate_cell = &(maze[y][x]);
	
	if (candidate_cell->visited)
		return false;
		
	if (!candidate_cell->wall) {
		candidate_cell->previous = previous_cell;
		candidate_cell->step_cost = candidate_cell->previous->step_cost+1;
		frontier.push(candidate_cell);
		return true;
	}
	
	return false;
}

//referenced: https://math.stackexchange.com/questions/139600/euclidean-manhattan-distance
double calc__manhattan_dist(cell* curr, cell* dest)
{
	//int arg_1 = dest->x - curr->x;
	//int arg_2 = dest->y - curr->y;
	//arg_1 = pow(arg_1, 2);
	//arg_2 = pow(arg_2, 2);
	//return sqrt(arg_1 + arg_2);
	int arg_1 = (dest->x - curr->x);
	int arg_2 = (dest->y - curr->y);
	if(arg_1<0)
		arg_1=-arg_1;
	if(arg_2<0)
		arg_2=-arg_2;
	return arg_1 + arg_2;
}


void process_line(cell** maze, string curr_line, int curr_height, maze_props &props)
{

	maze[curr_height] = new cell[curr_line.length()];
	int curr_col = 0;
	for(string::iterator it = curr_line.begin(); it != curr_line.end(); it++)
	{
		char test = *it;
		maze[curr_height][curr_col].x = curr_col;
		maze[curr_height][curr_col].y = curr_height;
		
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
			//props.goal_row = curr_height;
			//props.goal_col = curr_col;
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

//reference: https://en.wikipedia.org/wiki/Breadth-first_search
void bfs(cell** maze, maze_props props) {

	cell* current_cell;
	
	queue< cell* > frontier;
	frontier.push( props.start );

	//int depth = 0; //not keeping track
	int expansions = 0;
	//int cost = 0;
	
	while(!frontier.empty()) {
		current_cell = frontier.front();
		current_cell->visited = true;
		frontier.pop();
		
		expansions++; 
		
		if (current_cell == props.goal) {
			//We done did eet
			//continue;
			break;
		}
		
		//Add all adjacent cells to frontier
		int cx = current_cell->x, cy = current_cell->y;
		
		frontierCheckPush_bfs(frontier, maze, props, current_cell, cy,	cx+1) ;
		frontierCheckPush_bfs(frontier, maze, props, current_cell, cy+1,cx	) ;
		frontierCheckPush_bfs(frontier, maze, props, current_cell, cy,	cx-1) ;
		frontierCheckPush_bfs(frontier, maze, props, current_cell, cy-1,cx	) ;
		
		//Progress In Text
		if(DEBUG)
			print_progress(props, maze, expansions);//, found);
	}
	
	print_solution_bfs(maze, props, 0);

	//memory cleanup
	memory_cleanup(maze, props);

}

//reference: https://en.wikipedia.org/wiki/Depth-first_search
void dfs(cell** maze, maze_props props)
{
	cell* current_cell;
	
	stack< cell* > frontier;
	frontier.push( props.start );
	int expansions = 0;

	while(!frontier.empty())
	{
		//cout << "while" << endl;
		current_cell = frontier.top();
		frontier.pop();
		expansions++;

		if(current_cell == props.goal)
		{
			break;
		}

		if(current_cell->visited==false)
		{
			//cout << "iteration" << endl;
			current_cell->visited=true;
			int cx = current_cell->x, cy = current_cell->y;
			frontierCheckPush_dfs(frontier, maze, props, current_cell, cy,	cx+1) ;
			frontierCheckPush_dfs(frontier, maze, props, current_cell, cy+1,cx	) ;
			frontierCheckPush_dfs(frontier, maze, props, current_cell, cy,	cx-1) ;
			frontierCheckPush_dfs(frontier, maze, props, current_cell, cy-1,cx	) ;
		}

		if(DEBUG)
			print_progress(props, maze, expansions);
	}
	//offset -1 in print_solution_dfs definition
	print_solution_dfs(maze, props);
	memory_cleanup(maze, props);

}

void greedy(cell** maze, maze_props props)
{
	cell* current_cell;
	priority_queue <cell*> frontier;
	frontier.push(props.start);

	int expansions = 0;

	while(!frontier.empty())
	{
		//current_cell is the cell with the lowest heuristic
		//see overloaded operator in struct Comp (below struct cell)
		current_cell = frontier.top();
		current_cell->visited = true;
		frontier.pop();
		expansions++;

		int cx = current_cell->x, cy = current_cell->y;
		//a root node, push all children onto priority queue
		frontierCheckPush_greedy(frontier, maze, props, current_cell, cy,	cx+1) ;
		frontierCheckPush_greedy(frontier, maze, props, current_cell, cy+1,cx	) ;
		frontierCheckPush_greedy(frontier, maze, props, current_cell, cy,	cx-1) ;
		frontierCheckPush_greedy(frontier, maze, props, current_cell, cy-1,cx	) ;

		if(current_cell == props.goal)
		{
			break;
		}

		//only expand node with lowest heuristic
		if(DEBUG)
			print_progress(props, maze, expansions);

	}
	print_solution_bfs(maze, props, 0);
	memory_cleanup(maze, props);

}

void astar(cell** maze, maze_props props)
{
	cell* current_cell;
	priority_queue <cell*> frontier;
	frontier.push(props.start);

	int expansions = 0;

	while(!frontier.empty())
	{
		//current_cell is the cell with the lowest heuristic
		//see overloaded operator in struct Comp (below struct cell)
		current_cell = frontier.top();
		current_cell->visited = true;
		frontier.pop();
		expansions++;

		int cx = current_cell->x, cy = current_cell->y;
		//a root node, push all children onto priority queue
		frontierCheckPush_astar(frontier, maze, props, current_cell, cy,	cx+1) ;
		frontierCheckPush_astar(frontier, maze, props, current_cell, cy+1,cx	) ;
		frontierCheckPush_astar(frontier, maze, props, current_cell, cy,	cx-1) ;
		frontierCheckPush_astar(frontier, maze, props, current_cell, cy-1,cx	) ;

		if(current_cell == props.goal)
		{
			break;
		}

		//only expand node with lowest heuristic
		if(DEBUG) {
			cout << "Current step cost: " << current_cell->step_cost << endl;
			print_progress(props, maze, expansions);
		}
	}
		print_solution_bfs(maze, props, 0);
		memory_cleanup(maze, props);
}		


int main(int argc, char* argv[])
{

	if (argc!=3) {
		cout<< "usuck" <<endl;
		return 1;
	}
	
	char mode = argv[1][0];
	string m_size = argv[2];
	
	int num_cols = 0;
	int num_rows = 0;

	
	ifstream myfile (m_size.c_str());
	//cout << "Unable to open file." << endl;
	//std::ofstream outfile ("output_small.txt");

	cell** maze;
	maze_props props;
	//props.init_row = -1;
	//props.init_col = -1;
	//props.goal_row = -1;
	//props.goal_col = -1;
	//node* root = NULL;
	
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

	//initialize manhattan_dist heuristic
	for(int i=0; i<num_rows; i++)
	{
		for(int j=0; j<num_cols; j++)
		{
			maze[i][j].manhattan_dist = calc__manhattan_dist(&maze[i][j], props.goal);
			if(DEBUG_INIT)
			{
				cout << "Manhattan distance from [" << i << "][" << j << "] to [" << props.goal->y << "][" << props.goal->x << "]: " << maze[i][j].manhattan_dist << endl;
			}
		}
	}

	if(DEBUG_INIT) {
		cout << "The init is at row: " << props.start->x << " and col: " << props.start->y << endl;
		cout << "The goal is at row: " << props.goal->x << " and col: " << props.goal->y << endl;
	}
	
	
	for(int i=0; i<num_rows; i++) {
		for (int j=0; j<num_cols; j++) {
			cout << ((maze[i][j].wall)? 'W':' ') ;
		}
		cout << endl;
	}
	
	
	switch (mode) {
	
		case 'b' :
			bfs(maze, props);
			break;

		case 'd' :
			dfs(maze, props);
			break;
			
		case 'g' :
			greedy(maze, props);
			break;
			
		case 'a' :
			astar(maze, props);
			break;

		default:
			cout<<"usuck"<<endl;
	}
	//double test_dist = calc__manhattan_dist(props.start, props.goal);
	//cout << "Manhattan distance from start to goal: " << test_dist << endl;
}
