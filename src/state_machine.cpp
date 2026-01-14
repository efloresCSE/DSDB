#include "stokenizer.h"
#include "state_machine.h"

// This has the same documentation as the .h file

// Precondition: Input is the char to be analyzed
// Postcondition: returns true if input is a punctuation char
bool ispunct(char input)
{
    string punctuation = ".!@?,_#&$^%+()*><-:;=\'";

    for (size_t i = 0; input != '\0' && i < punctuation.length(); ++i)
        if (input == punctuation[i])
            return true;
    return false;
}

// Precondition: _table is our adjacency matrix
// Postcondition: Fills all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS])
{
    for (int i = 0; i < MAX_ROWS; ++i)
    {
        for (int j = 0; j < MAX_COLUMNS; ++j)
        {
            if (j == 0)
            {
                _table[i][j] = 0;
            }
            else
                _table[i][j] = -1;
        }
    }
}

// Precondition: _table is our adjacency matric,
// row_from and row_to mark a range of rows to print
// and col_from and col_to make up a range of columns to print.
// Postcondition: prints the desired range from our table
void print_table(int _table[][MAX_COLUMNS], int row_from, int row_to, int col_from, int col_to)
{
    for (int i = row_from; i <= row_to && i < MAX_ROWS; ++i)
    {
        // prints out rows and their contents
        for (int j = col_from; j <= col_to && j < MAX_COLUMNS; ++j)
            cout << "|" << _table[i][j] << "|";
        cout << endl;
    }
}

// Precondition: _table is our adjacency matrix, state is a row in the table
// Postcondition: Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 1;
}

// Precondition: _table is our adjacency matrix, state is a row in the table
// Postcondition: Mark this state (row) with a 0 (not success)
void mark_fail(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 0;
}

// Precondition: _table is our adjacency matrix, state is a row in the table
// Postcondition: returns true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state)
{
    if (_table[state][0] == 1)
        return true;
    else
        return false;
}

// Precondition: s is the string to be analyzed.
//_pos shows the position of the char on the string
// Postcondition: show string s and mark this position on the string:
// hello world   pos: 7
//        ^
void show_string(string s /*char s[]*/, int _pos)
{
    cout << s << "  Pos: " << _pos << endl;
    for (int i = 0; i < _pos; ++i)
        cout << " ";
    cout << "^" << endl;
}

// Precondition: _table is our adjacency matrix, row is a row in our table
// from and to make up the range of columns, and state is
// the state that we wish to mark these cells.
// Postcondition: Mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state)
{
    for (; from <= to; ++from)
    {
        _table[row][from] = state;
    }
}

// Precondition: Similar to function above except
// Mark columns represented by the string columns[] for this row
// We will be given "west" and we make every column that corresponds with
// the ascii value of each char as the state desired
// Postcondition: marks a range of cells in this array
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state)
{
    for (int i = 0; i < MAX_ROWS; ++i)
    {
        for (int j = 0; j < MAX_COLUMNS; ++j)
        {
            if (i == row && j == column)
            {
                table[i][j] = state;
            }
        }
    }
}

// Precondition: _table is our adjacency matrix, row is a row in our table
// column is a column in the table, state is the desired state change.
// Postcondition: Marks a single cell with a desired state
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state)
{
    for (int i = 0; columns[i] != '\0'; ++i)
    {
        _table[row][int(columns[i])] = state;
    }
}
