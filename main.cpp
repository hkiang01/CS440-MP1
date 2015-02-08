#include <fstream>
#include <iostream>


int main(void)
{

	// allocate on heap
	int small_cols = 0;
	int small_rows = 0;

	std::ifstream myfile ("smallMaze.txt");
	//std::ofstream outfile ("output_small.txt");

	if(myfile.is_open())
	{
		std::string curr_line;
		while(getline(myfile, curr_line))
		{
			small_cols = curr_line.size();
			small_rows++;


			for(std::string::iterator it = curr_line.begin(); it != curr_line.end(); it++)
			{
				char test = *it;
				if(test == '%')
				{
					//wall

				}
				else if(test == 'P')
				{
					//starting point
				}
				else if(test == '.')
				{
					//goal
				}
				else
				{
					//open space 
				}
			}
		}
	}
	else
	{
		std::cout << "Unable to open file." << std::endl;
	}
}