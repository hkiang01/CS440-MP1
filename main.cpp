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
		while(getline(myfile, curr_pic))
		{
			small_cols = curr_pic.size();
			small_rows++;
		}
	}
	else
	{
		std::cout << "Unable to open file." << std::endl;
	}
}