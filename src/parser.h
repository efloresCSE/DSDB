#ifndef PARSER_H
#define PARSER_H

#include "stokenizer.h"
#include "map.h"
#include "mmap.h"

using namespace std;

const int PROWS = 30;
const int PCOLS = 30;

class Parser
{
public:
/*
 * *************************************************************
 *                      C T O R
 * *************************************************************
*/
    //CTOR-> Take in string, create adjacency table
    Parser();
    Parser(char* phrase);
/*
 * *************************************************************
 *             T A B L E   F U N C T I O N S
 * *************************************************************
*/
    //Post: Create table
    void makeTable();
    //Post: Initialize tble
    void initTable();
    //Post: Prints table
    void printTable(int row_from = 0, int row_to = PROWS,
                    int col_from = 0, int col_to = PCOLS);
    //mark row for sucess
    void mark_success(int state);
    //mmark row for failure
    void mark_fail(int state);
    //mark specific cell
    void mark_cell(int row, int col, int state);
    bool is_success(int state);
/*
 * *************************************************************
 *              S H U N T I N G     Y A R D
 * *************************************************************
*/
    //shunting yard on parse_tree entries, and returns RPN as vector
    vector<string> shuntingYard();
    void loadPrecedence(map<string, int> &operators);

/*
 * *************************************************************
 *          P A R S E   T R E E   F U N C T I O N S
 * *************************************************************
*/
    //sets user input to appropriate keys in mmap
    void setMMap();
    MMap<string, string> get_parse_tree(){return parse_tree;}

/*
 * *************************************************************
 *               E X T R A   F U N C T I O N S
 * *************************************************************
*/
    //Post: Initialize Keyswords
    void initKeys();
    void set_string(char command[]);
private:

/*
 * *************************************************************
 *            P R I V A T E     V A R I A B L E S
 * *************************************************************
*/
    //enum of indeces
    enum indeces {ZERO, CREATE, TABLE, SYMBOL, FIELDS,
                  INSERT, INTO, VALUES, SELECT, STAR, FROM, WHERE, RELATIONAL, LOGICAL
                 , BATCH};
    //our stokenizer
    STokenizer stk;

    //vector that holds parsed tokens from user input
    vector<string> commands;

    //our keywords and their indexed values
    Map<string, indeces> keywords;

    //our parse tree, will hold all tokens in their correct place
    //extracted from user input
    MMap<string, string> parse_tree;

    //our table
    int table[PROWS][PCOLS];

};

#endif // PARSER_H
