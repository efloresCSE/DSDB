#include "parser.h"
#include <cctype> // <-- ADDED (for std::isspace)

Parser::Parser()
{
    initKeys();
    makeTable();
}

Parser::Parser(char *phrase)
{
    bool debug = false;
    string temp2 = "";
    bool comma = false;

    // make our table
    makeTable();

    // --- FIX: allow batch filenames like FinalTest or FinalTest.txt or examples/FinalTest.txt (and quoted paths) ---
    {
        std::string original = phrase ? std::string(phrase) : std::string();

        auto ltrim = [](std::string &s)
        {
            size_t i = 0;
            while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i])))
                i++;
            s.erase(0, i);
        };
        auto rtrim = [](std::string &s)
        {
            size_t i = s.size();
            while (i > 0 && std::isspace(static_cast<unsigned char>(s[i - 1])))
                i--;
            s.erase(i);
        };

        ltrim(original);

        // starts with "batch" and either ends or has whitespace after it
        if (original.rfind("batch", 0) == 0 &&
            (original.size() == 5 || std::isspace(static_cast<unsigned char>(original[5]))))
        {
            std::string fname = original.substr(5); // everything after 'batch'
            ltrim(fname);
            rtrim(fname);

            // strip surrounding quotes
            if (fname.size() >= 2 && fname.front() == '"' && fname.back() == '"')
                fname = fname.substr(1, fname.size() - 2);

            if (fname.empty())
                throw error("Invalid Input: batch needs a filename");

            commands.clear();
            commands.push_back("batch");
            commands.push_back(fname);

            initKeys();
            setMMap();
            return;
        }
    }
    // --- end FIX ---

    // tokenize string and store tokens in vector
    stk.set_string(phrase);
    // while more
    while (stk.more())
    {
        Token temp = Token();
        // grab a token
        stk >> temp;
        if (temp.token_str() != " " && temp.token_str() != "," && temp.token_str() != "\t")
            comma = false;
        // clear quotatoion marks
        if (temp.token_str() == "\"" && stk.more())
        {
            stk >> temp;
            while (temp.token_str()[0] != '\"' && stk.more())
            {
                temp2 += temp.token_str();
                stk >> temp;
            }
            commands.push_back(temp2);
            temp2.clear();
        }
        if (temp.token_str() == ",")
            comma = true;
        else if (temp.token_str() == "*" || temp.token_str() == "<" ||
                 temp.token_str() == "=" ||
                 temp.token_str() == ">" ||
                 temp.token_str() == "<=" ||
                 temp.token_str() == ">=" ||
                 temp.type_string() == "ALPHA" || temp.type_string() == "NUMBER")
            commands.push_back(temp.token_str());
    }

    // if no value after comma
    if (comma)
        throw error("Error: Missing value after comma");

    // initialize keywords to values in
    // if command is not found in indices, give the
    // value of symbol
    initKeys();

    // Setup parse tree
    // organizes tokens
    setMMap();

    if (debug)
    {
        cout << "PRINT TABLE" << endl;
        printTable();
        cout << endl;

        cout << "TOKENS" << endl;
        for (size_t i = 0; i < commands.size(); ++i)
        {
            cout << commands[i];
        }
        cout << endl;

        // MMAP
        cout << "PARSE_TREE" << endl;
        cout << "Commands: " << parse_tree["command"] << endl;
        cout << "table name: " << parse_tree["table_name"] << endl;
        cout << "fields " << parse_tree["fields"] << endl;
        cout << "values " << parse_tree["values"] << endl;
        cout << "relational" << parse_tree["relational"] << endl;
        cout << "logical" << parse_tree["logical"] << endl;
        cout << "file name" << parse_tree["file_name"] << endl;
    }
}

// set each token to its proper key
// E.G command = insert, select,
void Parser::setMMap()
{
    bool debug = false;
    int nextRow = 0;
    // go through commands
    for (size_t i = 0; i < commands.size(); ++i)
    {
        if (debug)
            cout << commands[i] << "->";
        nextRow = table[nextRow][keywords[commands[i]]];
        if (nextRow != -1)
        {
            if (keywords.contains(commands[i]) && keywords[commands[i]] != SYMBOL)
            {
                // add commands to parse tree
                switch (nextRow)
                {
                case 1:
                    parse_tree["command"] += commands[i];
                    break;
                case 6:
                    parse_tree["command"] += commands[i];
                    break;
                case 11:
                    parse_tree["command"] += commands[i];
                    break;
                case 12:
                    parse_tree["fields"] += commands[i];
                    break;
                case 17:
                    parse_tree["relational"] += commands[i];
                    break;
                case 15:
                    if (keywords[commands[i]] == LOGICAL)
                    {
                        parse_tree["logical"] += commands[i];
                        break;
                    }
                    else
                    {
                        break;
                    }
                case 19:
                    parse_tree["command"] += commands[i];
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch (nextRow)
                {
                // values, fields and tablename to
                case 3:
                    parse_tree["table_name"] += commands[i];
                    break;
                case 5:
                    parse_tree["fields"] += commands[i];
                    break;
                case 8:
                    parse_tree["table_name"] += commands[i];
                    break;
                case 10:
                    parse_tree["values"] += commands[i];
                    break;
                case 12:
                    parse_tree["fields"] += commands[i];
                    break;
                case 14:
                    parse_tree["table_name"] += commands[i];
                    break;
                case 16:
                    parse_tree["fields"] += commands[i];
                    break;
                case 18:
                    parse_tree["values"] += commands[i];
                    break;
                case 20:
                    parse_tree["file_name"] += commands[i];
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            throw error("Invalid input!");
        }
    }
    // we must end in a success state
    if (!is_success(nextRow))
    {
        throw error("Invalid Input: Check Syntax");
    }
}

// turns infix to postfix
vector<string> Parser::shuntingYard()
{
    // shunting yard on ptree
    MMap<string, string> temp;

    vector<string> opstack;
    map<string, int> op;
    loadPrecedence(op);
    vector<string> RPN;

    // get all strings in the order that they came in
    // store in expressions
    for (size_t i = 0; i < parse_tree["values"].size(); ++i)
    {
        // kept value fields and relation in the same order
        // or order for logical will change
        RPN += parse_tree["fields"][i + 1];
        RPN += parse_tree["values"][i];
        RPN += parse_tree["relational"][i];

        // if there is a logical expression take it in
        if (i < parse_tree["logical"].size())
        {
            // check precedence
            while (!opstack.empty() && op.at(opstack.back()) >= op.at(parse_tree["logical"][i]))
            {
                RPN += opstack.back();
                opstack.pop_back();
            }
            opstack.push_back(parse_tree["logical"][i]);
        }
    }

    for (size_t i = opstack.size(); i > 0; --i)
    {
        RPN += opstack[i - 1];
        opstack.pop_back();
    }

    return RPN;
}

// load the precedence between and or or
void Parser::loadPrecedence(map<string, int> &operators)
{
    operators["and"] = 2;
    operators["or"] = 1;
}

void Parser::initKeys()
{
    keywords["make"] = CREATE;
    keywords["create"] = CREATE;
    keywords["insert"] = INSERT;
    keywords["into"] = INTO;
    keywords["values"] = VALUES;
    keywords["table"] = TABLE;
    keywords["fields"] = FIELDS;
    keywords["select"] = SELECT;
    keywords["batch"] = BATCH;

    keywords["*"] = STAR;
    keywords["from"] = FROM;
    keywords["where"] = WHERE;
    keywords[">"] = RELATIONAL;
    keywords["="] = RELATIONAL;
    keywords["<"] = RELATIONAL;
    keywords["<="] = RELATIONAL;
    keywords[">="] = RELATIONAL;
    keywords["and"] = LOGICAL;
    keywords["or"] = LOGICAL;

    // set any unknown values to symbol
    // this can be a value, field, or tablename depending on what is
    // before it
    for (size_t i = 0; i < commands.size(); i++)
    {
        if (!keywords.contains(commands[i]))
        {
            keywords[commands[i]] = SYMBOL;
        }
    }
}
// initialize table
void Parser::initTable()
{
    for (int i = 0; i < PROWS; ++i)
    {
        for (int j = 0; j < PCOLS; ++j)
        {
            if (j == 0)
            {
                table[i][j] = 0;
            }
            else
                table[i][j] = -1;
        }
    }
}

// Precondition: _table is our adjacency matric,
// row_from and row_to mark a range of rows to print
// and col_from and col_to make up a range of columns to print.
// Postcondition: prints the desired range from our table
void Parser::printTable(int row_from, int row_to, int col_from, int col_to)
{
    for (int i = row_from; i <= row_to && i < PROWS; ++i)
    {
        // prints out rows and their contents
        for (int j = col_from; j <= col_to && j < PCOLS; ++j)
            cout << "|" << table[i][j] << "|";
        cout << endl;
    }
}

// Precondition: _table is our adjacency matrix, state is a row in the table
// Postcondition: Mark this state (row) with a 1 (success)
void Parser::mark_success(int state)
{
    table[state][0] = 1;
}

// Precondition: _table is our adjacency matrix, state is a row in the table
// Postcondition: Mark this state (row) with a 0 (not success)
void Parser::mark_fail(int state)
{
    table[state][0] = 0;
}

// Precondition: Similar to function above except
// Mark columns represented by the string columns[] for this row
// We will be given "west" and we make every column that corresponds with
// the ascii value of each char as the state desired
// Postcondition: marks a range of cells in this array
void Parser::mark_cell(int row, int column, int state)
{
    for (int i = 0; i < PROWS; ++i)
    {
        for (int j = 0; j < PCOLS; ++j)
        {
            if (i == row && j == column)
            {
                table[i][j] = state;
            }
        }
    }
}

// Precondition: _table is our adjacency matrix, state is a row in the table
// Postcondition: returns true if state is a success state
bool Parser::is_success(int state)
{
    if (table[state][0] == 1)
        return true;
    else
        return false;
}

// This state machine should represent the grammar for command line
// interpretation
void Parser::makeTable()
{
    // initialize our table to all negative 1
    initTable();

    // MAKE/CREATE MACHINE
    mark_fail(0);
    mark_fail(1);
    mark_fail(2);
    mark_fail(3);
    mark_fail(4);
    mark_success(5);
    mark_cell(0, CREATE, 1);
    mark_cell(1, TABLE, 2);
    mark_cell(2, SYMBOL, 3);
    mark_cell(3, FIELDS, 4);
    mark_cell(4, SYMBOL, 5);
    mark_cell(5, SYMBOL, 5);

    // INSERT MACHINE
    mark_fail(6);
    mark_fail(7);
    mark_fail(8);
    mark_fail(9);
    mark_success(10);
    mark_cell(0, INSERT, 6);
    mark_cell(6, INTO, 7);
    mark_cell(7, SYMBOL, 8);
    mark_cell(8, VALUES, 9);
    mark_cell(9, SYMBOL, 10);
    mark_cell(10, SYMBOL, 10);

    // SELECT MACHINE
    mark_fail(11);
    mark_fail(12);
    mark_fail(13);
    mark_success(14);
    mark_fail(15);
    mark_fail(16);
    mark_fail(17);
    mark_success(18);

    mark_cell(0, SELECT, 11);
    mark_cell(11, STAR, 12);
    mark_cell(11, SYMBOL, 12);
    mark_cell(12, FROM, 13);
    mark_cell(13, SYMBOL, 14);
    mark_cell(14, WHERE, 15);
    mark_cell(15, SYMBOL, 16);
    mark_cell(16, RELATIONAL, 17);
    mark_cell(17, SYMBOL, 18);
    mark_cell(18, LOGICAL, 15);

    // Batch Machine
    mark_fail(19);
    mark_success(20);
    mark_cell(0, BATCH, 19);
    mark_cell(19, SYMBOL, 20);
}
