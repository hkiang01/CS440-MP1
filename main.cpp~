#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <unistd.h> //for sleep
#include <stack>
#include <vector>
#include <math.h>
#include <cmath>
#include <climits>
#include <algorithm>
//referenced: cplusplus.com for use of libraries for...
//fstream, iostream, sleep in unistd.h, stack, queue, priority queue, math

using namespace std;

#define DEBUG false
#define DEBUG_INIT false

struct cell{
	
	public:
		bool wall;
		bool visited;
		cell* previous;
		
		int x;
		int y;
	
		int manhattan_dist;
		cell* nearest_goal;
		int step_cost;
		int total_cost;
		int goal_order;

		cell() : visited(false), previous(NULL), manhattan_dist(0), step_cost(0), goal_order(-1) {}
};

struct NodeGreater
{
    bool operator() ( const cell* lhs, const cell* rhs ) const
    {
    	if (lhs->total_cost == rhs->total_cost) 
    		return lhs->manhattan_dist > rhs->manhattan_dist;
        return lhs->total_cost > rhs->total_cost;
    }
};
//referenced https://stackoverflow.com/questions/10223845

struct GoalGreater
{
    bool operator() ( const cell* lhs, const cell* rhs ) const
    {
        return lhs->goal_order < rhs->goal_order;
    }
} goalgreater;


struct maze_props{

	int num_rows;
	int num_cols;
	//
	cell* start;
	cell* goal;

	vector<cell*> goals;
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

void memory_cleanup(cell** maze, maze_props props)
{
	for(int i = props.num_rows-1; i>=0; i--)
	{
		delete[] maze[i];
	}
	delete[] maze;
	return;
}

bool isSolPath(cell* curCell, maze_props props) {
	cell* c = props.goal;
	while(c->previous != NULL) {
		if (curCell == c)
			return true;
		c = c->previous;
	}
	return false;
}

void print_solution_bfs(cell** maze, maze_props props, int offset) {
	cout << "Solution: " << endl;
	
	if(!(props.goal)->visited) {
		cout << "Could not find path from starting point to finish point!" << endl;
		return;
	}
	
	/*
	//SNIPPET FROM PRINT_PROG
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
	cout << "Nodes expanded: " << expansions << endl;*/
	
	cell* current_cell = props.goal;
	
	vector<cell*> path;
	int path_length = 0;
	
	while(current_cell != NULL && current_cell!=props.start) {
		if (DEBUG)
			cout << current_cell->x << "," << current_cell->y << "    ";
		path.push_back(current_cell);
		++path_length;
		current_cell = current_cell->previous;
	}
	
	cout << "Found path of length " << path_length + offset << endl;
	for(int i = path_length-1; i>=0; i--) {
		cout << "(" << path[i]->x << "," << path[i]->y << ") -> " ;
	}
	cout << endl;
	
	//PRINT SOLUTION IN MAZEEEEEEEE
	//Progress In Text
	for(int i=0; i<props.num_rows; i++) {
		for (int j=0; j<props.num_cols; j++) {
			if(i==props.start->y && j==props.start->x)
			{
				cout << 'P';
				continue;
			}
			
			bool continue_flag = false;
			for(unsigned int k=0; k<props.goals.size(); k++) {
				if(i==props.goals.at(k)->y && j==props.goals.at(k)->x)
				{
					if (maze[i][j].visited)
					{
						cout << '*';
					}
					else
						cout << 'G';
					continue_flag=true;
					break;
				}
			}
			if (continue_flag)
				continue;
				
			cout << ((maze[i][j].wall)? '%':(maze[i][j].visited? (isSolPath(&(maze[i][j]),props)?'+':'.'):' ' )) ;
		}
		cout << endl;
	}
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
			
			bool continue_flag = false;
			for(unsigned int k=0; k<props.goals.size(); k++) {
				if(i==props.goals.at(k)->y && j==props.goals.at(k)->x)
				{
					if (maze[i][j].visited)
					{
						cout << '*';
					}
					else
						cout << 'G';
					continue_flag=true;
					break;
				}
			}
			if (continue_flag)
				continue;
				
			cout << ((maze[i][j].wall)? '%':(maze[i][j].visited? '.':' ' )) ;
		}
		cout << endl;
	}
	//cout << "Current depth: " << depth << endl;
	cout << "Nodes expanded: " << expansions << endl;
	//cout << "Cost: " << cost << endl;
	
	for(int i=0; i<1; i++)
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
bool frontierCheckPush_greedy(priority_queue<cell*, vector<cell*>, NodeGreater >& frontier, cell** maze, maze_props props, cell* previous_cell, int y, int x) {

	if (x<0 || y<0 || y>props.num_rows-1 || x>props.num_cols-1) {
		return false;
	}
	
	cell* candidate_cell = &(maze[y][x]);
	
	if (candidate_cell->visited)
		return false;
		
	if (!candidate_cell->wall) {
		candidate_cell->total_cost = candidate_cell->manhattan_dist+candidate_cell->step_cost;
		candidate_cell->previous = previous_cell;
		frontier.push(candidate_cell);
		return true;
	}
	
	return false;
}

//the only difference with the above is that before pushing onto the stack the step cost is calculated
bool frontierCheckPush_astar(priority_queue<cell*, vector<cell*>, NodeGreater >& frontier, cell** maze, maze_props props, cell* previous_cell, int y, int x) {
		if (x<0 || y<0 || y>props.num_rows-1 || x>props.num_cols-1) {
		return false;
	}
	
	cell* candidate_cell = &(maze[y][x]);
	
	if (candidate_cell->visited)
		return false;
		
	if (!candidate_cell->wall) {
		candidate_cell->previous = previous_cell;
		candidate_cell->step_cost = candidate_cell->previous->step_cost+1;
		candidate_cell->total_cost = candidate_cell->manhattan_dist+candidate_cell->step_cost;
		candidate_cell->visited = true;
		frontier.push(candidate_cell);
		return true;
	}
	
	return false;
}



//referenced: https://math.stackexchange.com/questions/139600/euclidean-manhattan-distance
int calc__manhattan_dist(cell* curr, cell* dest)
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

		}
		else if(test == '.')
		{
			//goal
			maze[curr_height][curr_col].wall = false;
			props.goal = &(maze[curr_height][curr_col]);
			props.goals.push_back(&maze[curr_height][curr_col]);
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


void set_manhattan_distances(cell** maze, maze_props props)
{
		//initialize manhattan_dist heuristic
	for(int i=0; i<props.num_rows; i++)
	{
		for(int j=0; j<props.num_cols; j++)
		{
			maze[i][j].manhattan_dist = calc__manhattan_dist(&maze[i][j], props.goal);
			if(DEBUG_INIT)
			{
				cout << "Manhattan distance from [" << i << "][" << j << "] to [" << props.goal->y << "][" << props.goal->x << "]: " << maze[i][j].manhattan_dist << endl;
			}
		}
	}
}

void set_manhattan_distances_3(cell** maze, maze_props props, vector<cell*> goals_list)
{
	//initialize manhattan_dist heuristic
	for(int i=0; i<props.num_rows; i++)
	{
		for(int j=0; j<props.num_cols; j++)
		{
			int min_dist = INT_MAX;
			cell* min_dist_cell;
			//for every goal in props.goals
			for(unsigned int k = 0; k < goals_list.size(); k++)
			{
				int test_dist = calc__manhattan_dist(&maze[i][j], goals_list.at(k));
				if(test_dist<min_dist)
				{
					min_dist = test_dist;
					min_dist_cell = &(maze[goals_list.at(k)->y][goals_list.at(k)->x]);
				}
			}
			maze[i][j].manhattan_dist = min_dist;
			maze[i][j].nearest_goal = min_dist_cell;
			if(DEBUG_INIT)
			{
				//cout << "Manhattan distance from [" << i << "][" << j << "] to [" << min_dist_cell->y << "][" << min_dist_cell->x << "]: " << min_dist/*maze[i][j].manhattan_dist*/ << endl;
			}
		}
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
	priority_queue <cell*, vector<cell*>, NodeGreater > frontier;
	frontier.push(props.start);

	int expansions = 0;

	while(!frontier.empty())
	{
		//current_cell is the cell with the lowest heuristic
		//see overloaded operator in struct Comp (below struct cell)
		current_cell = frontier.top();
		current_cell->visited = true;
		//frontier = priority_queue <cell*, vector<cell*>, less<cell*> >();
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
		//if(DEBUG)
		//	print_progress(props, maze, expansions);
		//only expand node with lowest heuristic
		if(DEBUG) {
			cout << "Current total cost: " << current_cell->total_cost << endl;
			
			//
			queue<cell*> f;
			unsigned int s = frontier.size();
			for(unsigned int i=0; i<s; i++) {
				cell* c = frontier.top();
				frontier.pop();
				cout << c->total_cost << "(" << c->x << "," << c->y << ");;; ";
				f.push(c);
			}
			for(unsigned int i=0; i<s; i++) {
				frontier.push(f.front());
				f.pop();
			}
			cout << endl;
			//
			
			print_progress(props, maze, expansions);
		}

	}
	print_solution_bfs(maze, props, 0);
	memory_cleanup(maze, props);

}

void astar(cell** maze, maze_props props)
{
	cell* current_cell;
	priority_queue <cell*, vector<cell*>, NodeGreater > frontier;
	(props.start)->visited=true;
	frontier.push(props.start);

	int expansions = 0;

	while(!frontier.empty())
	{
		//current_cell is the cell with the lowest heuristic
		//see overloaded operator in struct Comp (below struct cell)
		current_cell = frontier.top();
		//current_cell->visited = true;
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
			cout << "Current total cost: " << current_cell->total_cost << endl;
			
			//
			queue<cell*> f;
			unsigned int s = frontier.size();
			for(unsigned int i=0; i<s; i++) {
				cell* c = frontier.top();
				frontier.pop();
				cout << c->total_cost << "(" << c->x << "," << c->y << ");;; ";
				f.push(c);
			}
			for(unsigned int i=0; i<s; i++) {
				frontier.push(f.front());
				f.pop();
			}
			cout << endl;
			//
			
			print_progress(props, maze, expansions);
		}
	}
		print_solution_bfs(maze, props, 0);
		memory_cleanup(maze, props);
}		


////MP13 A STAR 3 SEARCH//////

void print_solution_astar_3(cell** maze, maze_props props)
{
	sort(props.goals.begin(), props.goals.end(), goalgreater);
	cout << "Goal | coordinates" << endl;
	for(unsigned int i = 0; i < props.goals.size(); i++)
	{
		cout << i+1 << "\t(" << props.goals.at(i)->x << ", " << props.goals.at(i)->y << ")" << endl;
	}
}


//Logic is very different from the previous frontier check push's, primarily, ndoes can be revisited now
bool frontierCheckPush_astar_3(priority_queue<cell*, vector<cell*>, NodeGreater >& frontier, cell** maze, maze_props props, cell* previous_cell, int y, int x) {
		if (x<0 || y<0 || y>props.num_rows-1 || x>props.num_cols-1) {
		return false;
	}
	
	cell* candidate_cell = &(maze[y][x]);
	
	if (!candidate_cell->wall) {
		candidate_cell->previous = previous_cell;
		candidate_cell->step_cost = candidate_cell->previous->step_cost+1;
		//candidate_cell->total_cost = candidate_cell->manhattan_dist+candidate_cell->step_cost;// + calc__manhattan_dist(candidate_cell, previous_cell);
		candidate_cell->visited = true;

		
		//replace duplicate in frontier with new distance
		stack<cell*> temp_stack;
		cell* temp_cell;
		temp_stack.push(candidate_cell);
		
		//Preemptively oblierate all duplicates
		for(unsigned int i = 0; i < frontier.size(); ++i)
		{
			temp_cell = frontier.top();
			frontier.pop();
			
			if(temp_cell!=candidate_cell)
				temp_stack.push(temp_cell);	
		}
		
		//push stack back onto frontier
		while(temp_stack.size()>0) {
			temp_cell = temp_stack.top();
			frontier.push(temp_cell);
			temp_stack.pop();
		}

		return true;
	}
	


	return false;
}


//referenced: http://www.laurentluce.com/posts/solving-mazes-using-python-simple-recursivity-and-a-search/
//referenced: https://en.wikipedia.org/wiki/A*_search_algorithm
bool astar_3(cell** maze, maze_props props)
{
	vector<cell*> goals_list = props.goals;
	priority_queue<cell*, vector<cell*>, NodeGreater> open_list;
	//priority_queue<cell*, vector<cell*>, NodeGreater> closed_list;
	vector<cell*> closed_list;
	vector<cell*> adjacent_cells;

	int num_expansions = 0;
	int goal_counter = 0;

	//push start node on open list
	open_list.push(props.start);
	cell* curr_cell;
	while(!open_list.empty() && !goals_list.empty())
	{
		//pop cell from open list
		curr_cell = open_list.top();
		open_list.pop();
		num_expansions++;
		//add cell to closed list to prevent from processing it twice
		closed_list.push_back(curr_cell);
		curr_cell->visited=true;

		//check for a goal, erase it from goals_list
		for(unsigned int i=0; i<goals_list.size(); i++) {
			if(curr_cell == goals_list.at(i))
			{
				goals_list.erase(goals_list.begin()+i);
				curr_cell->goal_order=goal_counter;
				goal_counter++;
				i--;
				if (DEBUG)
					cout << "Goal found at: (" << curr_cell->x << ", " << curr_cell->y << ")" << endl;

				//recalculate heuristic for each cell
				set_manhattan_distances_3(maze, props, goals_list);
				break;
			}
		}

		//get adjacent cells and push them on to adjacent_cells
		int cx = curr_cell->x;
		int cy = curr_cell->y;
		if(cx < props.num_cols-1)
			adjacent_cells.push_back(&maze[cy][cx+1]);
		if(cy > 0)
			adjacent_cells.push_back(&maze[cy-1][cx]);
		if(cx > 0)
			adjacent_cells.push_back(&maze[cy][cx-1]);
		if(cy < props.num_rows-1)
			adjacent_cells.push_back(&maze[cy+1][cx]);

		//for every adjacent cell,
		for(unsigned int i = 0; i < adjacent_cells.size(); i++)
		{
			cell* adjacent_candidate= adjacent_cells.at(i);
			bool in_closed = false;
			bool in_open = false;
			
			//check to see if it's in the closed list
			for( unsigned int j = 0; j < closed_list.size(); j++)
			{
				if(adjacent_candidate->x==closed_list.at(j)->x && adjacent_candidate->y==closed_list.at(j)->y)
				{
					in_closed = true;
					break;
				}
			}

			//check to see if it's in the open list
			cell* temp_cell;
			stack<cell*> temp_stack;
			for(unsigned int j = 0; j < open_list.size(); j++)
			{
				temp_cell = open_list.top();
				open_list.pop();
				
				if(temp_cell!=adjacent_candidate)
					temp_stack.push(temp_cell);	
				else
				{
					in_open=true;
					break;
				}
			}
			//push stack back onto frontier
			while(!temp_stack.empty()) {
				temp_cell = temp_stack.top();
				open_list.push(temp_cell);
				temp_stack.pop();
			}
			//check to see if it's in the open list

			//DOUBLE CHECK THIS
			//if the adjacent cell is reachable and not in the closed list
			if(adjacent_candidate->wall==false && !in_closed)
			{
				if(in_open)
				{		
					// Regarding “if c.g > cell.g + 10″,
					// we are checking if the path going through the current cell is better than what was previously calculated
					// for the adjacent cell. +10 means current path beats adjacent cell path + one move.
					//check to see if the current path is better than the one previously found for adjacent_candidate
					if(adjacent_candidate->step_cost > curr_cell->step_cost+1)
					{
						//update the cell: 1) increment step cost of adjacent_candidate
						//				   2) assign heuristic to adjacent_candidate (not needed, see first for loop in method)
						//				   3) assign adjacent_candidate's parent to curr_cell
						//				   4) update total cost of adjacent_candidate

						//calculate G and H in flow chart
						adjacent_candidate->step_cost = adjacent_candidate->step_cost+1;
						adjacent_candidate->total_cost = adjacent_candidate->manhattan_dist + adjacent_candidate->step_cost;

						//set adj cell parent in flow chart
						adjacent_candidate->previous = curr_cell;
						
						//don't add it to the open list
					}
				}
				else
				{
					//calculate G and H in flow chart
					adjacent_candidate->step_cost = adjacent_candidate->step_cost+1;
					adjacent_candidate->total_cost = adjacent_candidate->manhattan_dist + adjacent_candidate->step_cost;

					//set adj cell parent in flow chart
					adjacent_candidate->previous = curr_cell;
					
					//add it to the open list
					open_list.push(adjacent_candidate);
				}
			}
		}
		print_progress(props, maze, num_expansions);
	}
	
	//cell* current_cell;
	//priority_queue <cell*, vector<cell*>, NodeGreater > frontier;
	//frontier.push(props.start);
	//open_list.push(props.start);

	//int expansions = 0;
	//int goal_counter = 0;

		print_solution_astar_3(maze, props);
		memory_cleanup(maze, props);
		
		return false;
}

//called recursively
bool astar_3_helper(cell** maze, maze_props props, int x, int y, vector<cell*> &goals_list, int &goal_counter)
{
	//bounds check
	if(x<0 || x>props.num_cols-1 || y<0 || y>props.num_rows-1)
	{
		return false;
	}

	cell* candidate_cell = &(maze[y][x]);
	//check for a goal, erase it from goals_list
	for(unsigned int i=0; i<goals_list.size(); i++) {
		if(candidate_cell == goals_list.at(i))
		{
			goals_list.erase(goals_list.begin()+i);
			candidate_cell->goal_order=goal_counter;
			goal_counter++;
			i--;
			cout << "Goal found at: (" << candidate_cell->x << ", " << candidate_cell->y << ")" << endl;
			return true;
		}
	}

	//check for wall
	if(candidate_cell->wall==true)
	{
		cout << "Visited at: (" << candidate_cell->x << ", " << candidate_cell->y << ")" << endl;
		return false;
	}

	//check if visited
	if(candidate_cell->visited==true)
	{
		cout << "Wall at: (" << candidate_cell->x << ", " << candidate_cell->y << ")" << endl;
		return false;
	}

	//mark as visited
	candidate_cell->visited=true;
	cout << "Visiting (" << candidate_cell->x << ", " << candidate_cell->y << ")" << endl;

	//explore neighbors recursively
	if((candidate_cell->x < props.num_cols-1 && astar_3_helper(maze, props, x+1, y, goals_list, goal_counter)) //right neighbor
	|| 	(candidate_cell->y > 0 && astar_3_helper(maze, props, x, y-1, goals_list, goal_counter)) //down neighbor
	|| 	(candidate_cell->x > 0 && astar_3_helper(maze, props, x-1, y, goals_list, goal_counter)) //down neighbor
	|| 	(candidate_cell->y < props.num_rows-1 && astar_3_helper(maze, props, x, y+1, goals_list, goal_counter))) //down neighbor
	{
		return true;
	}
	return false;
}

int main(int argc, char* argv[])
{

	if (argc!=3) {
		cout<< "Error, usage: ./mp1 [search flag] [maze file name]" <<endl;
		return 1;
	}
	
	char mode = argv[1][0];
	string m_size = argv[2];
	
	int num_cols = 0;
	int num_rows = 0;

	
	ifstream myfile (m_size.c_str());
	
	if(!myfile.is_open()) {
		cout << "Error opening file" <<endl;
		
		return 1;
	}
	
	cell** maze;
	maze_props props;

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


	switch (mode) {
	
		case 'b' :
			set_manhattan_distances(maze, props);
			bfs(maze, props);
			break;

		case 'd' :
			set_manhattan_distances(maze, props);
			dfs(maze, props);
			break;
			
		case 'g' :
			set_manhattan_distances(maze, props);
			greedy(maze, props);
			break;
			
		case 'a' :
			set_manhattan_distances(maze, props);
			astar(maze, props);
			break;

		case 'A' :
			set_manhattan_distances_3(maze, props, props.goals);
			astar_3(maze, props);
			break;

		default:
			cout<<"invalid search flags, choose: b, d, g, a, A"<<endl;
			return 1;
	}

	if(DEBUG_INIT) {
		cout << "The init is at row: " << props.start->x << " and col: " << props.start->y << endl;
		cout << "The goal is at row: " << props.goal->x << " and col: " << props.goal->y << endl;
	}
	
	//double test_dist = calc__manhattan_dist(props.start, props.goal);
	//cout << "Manhattan distance from start to goal: " << test_dist << endl;
}
