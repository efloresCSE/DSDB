#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <iostream>
#include <string>
using namespace std;

//Global constants for rows and columns of table
const int MAX_ROWS = 30;
const int MAX_COLUMNS = 256;

//Precondition: _table is our adjacency matrix
//Postcondition: Fills all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]);

//Precondition: _table is our adjacency matrix, state is a row in the table
//Postcondition: Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state);

//Precondition: _table is our adjacency matrix, state is a row in the table
//Postcondition: Mark this state (row) with a 0 (not success)
void mark_fail(int _table[][MAX_COLUMNS], int state);

//Precondition: _table is our adjacency matrix, state is a row in the table
//Postcondition: returns true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state);

//Precondition: _table is our adjacency matrix, row is a row in our table
//from and to make up the range of columns, and state is
//the state that we wish to mark these cells.
//Postcondition: Mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], int from,
                int to, int state);

//Precondition: Similar to function above except
//Mark columns represented by the string columns[] for this row
//We will be given "west" and we make every column that corresponds with
//the ascii value of each char as the state desired
//Postcondition: marks a range of cells in this array
void mark_cells(int row, int _table[][MAX_COLUMNS],
                const char columns[], int state);

//Precondition: _table is our adjacency matrix, row is a row in our table
//column is a column in the table, state is the desired state change.
//Postcondition: Marks a single cell with a desired state
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state);

//Precondition: _table is our adjacency matric,
//row_from and row_to mark a range of rows to print
//and col_from and col_to make up a range of columns to print.
//Postcondition: prints the desired range from our table
void print_table(int _table[][MAX_COLUMNS], int row_from=0,
            int row_to=MAX_ROWS, int col_from=0, int col_to=MAX_COLUMNS);

//Precondition: s is the string to be analyzed.
//_pos shows the position of the char on the string
//Postcondition: show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(string s/*char s[]*/, int _pos);

//Precondition: Input is the char to be analyzed
//Postcondition: returns true if input is a punctuation char
bool ispunct(char input);

#endif // STATE_MACHINE_H
