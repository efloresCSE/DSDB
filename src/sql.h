#ifndef SQL_H
#define SQL_H

#include "table.h"
#include "parser.h"


class SQL
{
public:
/*
 * *************************************************************
 *                      C T O R
 * *************************************************************
*/
    //constructor
    SQL();
/*
 * *************************************************************
 *              R U N    F U N C T I O N S
 * *************************************************************
*/
    //runs sql through terminal
    void run();
    //runs sql commands from a given batch file
    void run_batch(string filename);

/*
 * *************************************************************
 *      C O M M A N D   D I S P L A Y   F U N C T I O N S
 * *************************************************************
*/
    //displays a message after create
    void display_create(string command, ostream& outs = cout);
    //displays a message after insert
    void display_insert(string command, ostream& outs = cout);
    //displays a message after select all
    void display_select_all(string command, Table t, ostream& outs = cout);
/*
 * *************************************************************
 *       T E X T     F I L E     F U N C T I O N S
 * *************************************************************
*/
    //checks if a text file exists
    bool t_file_exists(string file_name);
    //opens a text file for reading and writing
    void t_open_fileRW(fstream& f, const string file_name);

/*
 * *************************************************************
 *              P R I V A T E   V A R I A B L E S
 * *************************************************************
*/
private:
    //command number
    int commNum;
    //a command
    string command;
    //A parse tree that holds our tokens
    MMap<string, string> ptree;
};
#endif // SQL_H
