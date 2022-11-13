#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace std;
//===================================================================================================================================================
#define NEIGHBOURS_COUNT 4
//===================================================================================================================================================
struct cell
{
	int row;
	int col;
	int color;
	bool is_satisfied;
	int next_step_stat;
};
//===================================================================================================================================================
int number_of_rows(const string map_name)
{
	ifstream input_file(map_name);
	char ch;
	int num_of_rows = 0;
	while (input_file.get(ch))
	{
		if (ch == '\n')
		{	
			num_of_rows++;
		}
	}
	num_of_rows++;
	return num_of_rows;
}

int number_of_cols(const string map_name)
{
	ifstream input_file(map_name);
	char ch;
	int num_of_cols = 0;
	while (input_file.get(ch))
	{
		if (ch == '\n')
		{
			break;
		}
		num_of_cols++;
	}
	return num_of_cols;
}

void create_cells(const string map_name, vector<vector<struct cell>>& cells, int& row_count, int& col_count)
{
	ifstream input_file(map_name);
	char ch;
	for (int i = 0; i < row_count; i++)
	{
		for (int j = 0; j < col_count; j++)
		{
			cells[i][j].row = i;
			cells[i][j].col = j;
			cells[i][j].next_step_stat = 0;
			input_file.get(ch);
			if (ch == 'E')
				cells[i][j].color = 0;
			else if (ch == 'R')
				cells[i][j].color = 1;
			else if (ch == 'B')
				cells[i][j].color = 2;
		}
		input_file.get(ch);
	}
	return;
}

bool cell_is_empty(struct cell sample)
{
	if (sample.color == 0)
		return true;
	return false;
}

bool is_in_map(int& row, int& col, int& row_count, int& col_count)
{
	if (((row >= 0) && (row < row_count)) && ((col >= 0) && (col < col_count)))
		return true;
	else
		return false;
}

bool are_same(struct cell cell_1, struct cell cell_2)
{
	if (((cell_1.color == 0) || (cell_2.color == 0)) || ((cell_1.color == cell_2.color)))
		return true;
	return false;
}

bool is_satisfied(vector<vector<struct cell>>& cells, int& row, int& col, int& min_of_satisfaction, int& row_count, int& col_count)
{
	int same_neighbours = 0;
	int neighbours_count = 0;
	vector<int> row_delta = { -1,0,1,0 };
	vector<int> col_delta = { 0,1,0,-1 };
	if (!(cell_is_empty(cells[row][col])))
	{
		for (int i = 0; i < NEIGHBOURS_COUNT; i++)
		{
			int neighbour_row = row + row_delta[i];
			int neighbour_col = col + col_delta[i];
			if (is_in_map(neighbour_row, neighbour_col, row_count, col_count))
			{
				neighbours_count++;
				if (are_same(cells[row][col], cells[neighbour_row][neighbour_col]))
					same_neighbours++;
			}
		}
		double satisfaction_percent;
		if (neighbours_count != 0)
		{
			satisfaction_percent = (same_neighbours*100.0) / neighbours_count;
		}
		else
		{
			satisfaction_percent = 100;
		}
		if (satisfaction_percent >= min_of_satisfaction)
			return true;
		return false;
	}
	else
	{
		return true;
	}
}

bool all_are_satisfied(vector<vector<struct cell>>& cells, int& row_count, int& col_count)
{
	for (int i = 0; i < row_count; i++)
	{
		for (int j = 0; j < col_count; j++)
		{
			if (!(cells[i][j].is_satisfied))
			{
				return false;
			}
		}
	}
	return true;
}

void refresh_status_of_cells(vector<vector<struct cell>>& cells, int& row_count, int& col_count, int& min_of_satisfaction)
{
	for (int i = 0; i < row_count; i++)
		for (int j = 0; j < col_count; j++)
		{
			cells[i][j].next_step_stat = 0;
			if (is_satisfied(cells, i, j, min_of_satisfaction, row_count, col_count))
				cells[i][j].is_satisfied = true;
			else
				cells[i][j].is_satisfied = false;
		}
}

vector<struct cell> find_unsatisfied_cells(vector<vector<struct cell>>& cells, int& row_count, int& col_count)
{
	vector <struct cell> unsatisfied_cells;
	for (int i = 0; i < row_count; i++)
	{
		for (int j = 0; j < col_count; j++)
		{
			if (!(cells[i][j].is_satisfied))
			{
				unsatisfied_cells.push_back(cells[i][j]);
			}
		}
	}
	return unsatisfied_cells;
}

vector<struct cell> find_empty_cells(vector<vector<struct cell>>& cells, int& row_count, int& col_count)
{
	vector <struct cell> empty_cells;
	for (int i = 0; i < row_count; i++)
	{
		for (int j = 0; j < col_count; j++)
		{
			if (cell_is_empty(cells[i][j]))
			{
				empty_cells.push_back(cells[i][j]);
			}
		}
	}
	return empty_cells;
}

int random_index(int num)
{
	srand(time(0));
	return (rand() % num);
}

bool is_same_cell(struct cell cell_1, struct cell cell_2)
{
	if ((cell_1.row == cell_2.row) && (cell_1.col == cell_2.col))
		return true;
	return false;
}

void show_cells(vector<vector<struct cell>>& cells, int& row_count, int& col_count)
{
	for (int i = 0; i < row_count; i++)
	{
		for (int j = 0; j < col_count; j++)
		{
			if (cells[i][j].color == 0)
				cout << 'E';
			if (cells[i][j].color == 1)
				cout << 'R';
			if (cells[i][j].color == 2)
				cout << 'B';
		}
		cout << endl;
	}
}

int find_destination_index(const vector <struct cell>& destination_cells, const vector <struct cell>& unsatisfied_cells)
{
	int destination_index = random_index(destination_cells.size());
	while (is_same_cell(unsatisfied_cells[0], destination_cells[destination_index]))
	{
		destination_index = random_index(destination_cells.size());
	}
	return destination_index;
}

void swap_cells(vector<vector<struct cell>>& cells, vector <struct cell>& destination_cells, int destination_index, vector <struct cell>& unsatisfied_cells)
{
	cells[destination_cells[destination_index].row][destination_cells[destination_index].col].color = unsatisfied_cells[0].color;
	cells[destination_cells[destination_index].row][destination_cells[destination_index].col].next_step_stat = 1;
	if (cells[unsatisfied_cells[0].row][unsatisfied_cells[0].col].next_step_stat == 0)
		cells[unsatisfied_cells[0].row][unsatisfied_cells[0].col].color = 0;

	unsatisfied_cells.erase(unsatisfied_cells.begin());
	destination_cells.erase(destination_cells.begin() + destination_index);
}

void simulate(vector<vector<struct cell>>& cells, int& row_count, int& col_count, int& simulation_steps, int& min_of_satisfaction)
{
	int step = simulation_steps;
	while (step != 0)
	{
		if (all_are_satisfied(cells, row_count, col_count))
			return;

		vector <struct cell> unsatisfied_cells = find_unsatisfied_cells(cells, row_count, col_count);
		vector <struct cell> empty_cells = find_empty_cells(cells, row_count, col_count);
		vector <struct cell> destination_cells = unsatisfied_cells;
		destination_cells.insert(destination_cells.end(), empty_cells.begin(), empty_cells.end());
		int unsatisfied_cells_count = unsatisfied_cells.size();
		for (int i = 0; i < unsatisfied_cells_count; i++)
		{
			int destination_index = find_destination_index(destination_cells, unsatisfied_cells);
			swap_cells(cells, destination_cells, destination_index, unsatisfied_cells);
		}
		refresh_status_of_cells(cells, row_count, col_count, min_of_satisfaction);
		step -= 1;
	}
}

void create_ppm_file(vector<vector<struct cell>>& cells, const char* file_name, int& row_count, int& col_count, int pixel_size = 50)
{
	FILE* name = fopen(file_name, "w");
	ofstream outfile;
	outfile.open(file_name);
	outfile << "P3\n" << col_count*pixel_size << " " << row_count*pixel_size << " " << "1" << endl;

	for (int i = 0; i < row_count*pixel_size; i++)
	{
		for (int j = 0; j < col_count*pixel_size; j++)
		{
			if (cells[i / pixel_size][j / pixel_size].color == 0)
				outfile << "1" << " " << "1" << " " << "1" << "\t";
			if (cells[i / pixel_size][j / pixel_size].color == 1)
				outfile << "1" << " " << "0" << " " << "0" << "\t";
			if (cells[i / pixel_size][j / pixel_size].color == 2)
				outfile << "0" << " " << "0" << " " << "1" << "\t";
		}
		outfile << endl;
	}

}

void show_unsatisfied_cells_count(vector<vector<struct cell>>& cells, int& row_count, int& col_count)
{
	cout << find_unsatisfied_cells(cells, row_count, col_count).size() << endl;
}

string convert_to_string(char* a)
{
	string s = a;
	return s;
}

string process_input(int argc, vector<string> args, int& min_of_satisfaction, int& simulation_steps)
{
	int j = 0;
	for (int i = 0; i < argc; i++)
	{
		if (args[i] == "-p")
		{
			min_of_satisfaction = stoi(args[i + 1]);
		}
		if (args[i] == "-s")
		{
			simulation_steps = stoi(args[i + 1]);
		}
		if (args[i] == "-f")
		{
			j = i + 1;
		}
	}
	return args[j];
}
//===================================================================================================================================================
int main(int argc, char* argv[])
{
	string map_name;
	int min_of_satisfaction = 30;
	int simulation_steps = -1;

	vector <string> args;
	for (int i = 0; i < argc; i++)
	{
		args.push_back(convert_to_string(argv[i]));
	}
	map_name = process_input(argc, args, min_of_satisfaction, simulation_steps);

	int row_count = number_of_rows(map_name);
	int col_count = number_of_cols(map_name);
	vector <vector<struct cell>> cells(row_count, vector <struct cell>(col_count));

	create_cells(map_name, cells, row_count, col_count);
	refresh_status_of_cells(cells, row_count, col_count, min_of_satisfaction);
	simulate(cells, row_count, col_count, simulation_steps, min_of_satisfaction);
	show_unsatisfied_cells_count(cells, row_count, col_count);
	show_cells(cells, row_count, col_count);
	create_ppm_file(cells, "final.ppm", row_count, col_count);

	return 0;
}