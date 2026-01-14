#include "stokenizer.h"
#include <cstring>
#include <cctype>

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];
using namespace std;

// Postcondition: sets _pos to zero. Sets _buffer to empty
// creates adjacency matrix for desired states.
STokenizer::STokenizer()
{
    _buffer[0] = '\0';
    make_table(_table);
    _pos = 0;
}

// Postcondition: _buffer memeber variable has been initialized
// to the value of str. _pos is still zero.
// creates adjacency matrix for desired states.
STokenizer::STokenizer(char str[])
{
    if (strlen(str) == 0)
        throw error("Error, no input");
    strncpy(_buffer, str, MAX_BUFFER - 1);
    _buffer[MAX_BUFFER - 1] = '\0';
    make_table(_table);
    _pos = 0;
}

ostream &operator<<(ostream &outs, STokenizer &s)
{
    outs << s._buffer;
    return outs;
}

// Postcondition: returns true if there are no more tokens
bool STokenizer::done()
{
    int len = static_cast<int>(strlen(_buffer));
    return _pos > len;
}

// Postcondition: returns true if there are more tokens
bool STokenizer::more()
{
    if (debug)
    {
        cout << "strlen: " << strlen(_buffer) << endl;
    }
    // I removed the equality sign
    int len = static_cast<int>(strlen(_buffer));
    return _pos < len;
}

// Postcondition: extracts one token (similar tot he way cin >> work)
// from Stokenizer s and stores the string and its type in a Token object
STokenizer &operator>>(STokenizer &s, Token &t)
{
    string token_string;

    const int DIGIT_STATE = 1, SPACE_STATE = 4,
              ALPHA_STATE = 6, PUNCT_STATE = 8, QUOT_STATE = 10;
    const int DIGIT_TYPE = 1, SPACE_TYPE = 4,
              ALPHA_TYPE = 2, PUNCT_TYPE = 3, QUOT_TYPE = 5,
              UNKNOWN_TYPE = 6;

    // checks if digit
    if (isdigit(s._buffer[s._pos]))
    {
        // gets the token, the starting state is dependant on the
        // first char being evaluated
        s.get_token(DIGIT_STATE, token_string);
        // sets token with valid string and type
        t.set(token_string, DIGIT_TYPE);
    }
    // checks if space
    else if (isspace(s._buffer[s._pos]))
    {
        s.get_token(SPACE_STATE, token_string);
        t.set(token_string, SPACE_TYPE);
    }
    // checks if alpha
    else if (isalpha(s._buffer[s._pos]))
    {
        s.get_token(ALPHA_STATE, token_string);
        t.set(token_string, ALPHA_TYPE);
    }
    // checks if punct
    else if (ispunct(s._buffer[s._pos]))
    {
        s.get_token(PUNCT_STATE, token_string);
        t.set(token_string, PUNCT_TYPE);
    }
    else if (s._buffer[s._pos] == '\"')
    {
        s.get_token(QUOT_STATE, token_string);
        t.set(token_string, QUOT_TYPE);
    }
    else
    {
        t.set(token_string, UNKNOWN_TYPE);
        s._pos++;
        //        throw error("Error, Unknown Input!");
    }

    return s;
}

// Postcondition: sets a new string str into the _buffer member variable
void STokenizer::set_string(char str[])
{
    strncpy(_buffer, str, MAX_BUFFER - 1);
    _buffer[MAX_BUFFER - 1] = '\0';
    // this is ok
    _pos = 0;
}

// Precondition: we are given a starting state, and _buffer has
// a phrase in it
// Postconditon: extract the longest string that match
// one of the acceptable token types
bool STokenizer::get_token(int start_state, string &token)
{
    bool success = false;
    int new_state;
    int orgPos = _pos;
    string aux = string(_buffer);

    // debug
    if (debug)
    {
        cout << "aux: " << aux << endl;
    }
    if (debug)
    {
        cout << "pos1: " << _pos << endl;
        cout << "starting char " << _buffer[_pos] << endl;
        cout << "starting_state: " << start_state << endl;
        cout << "strlen: " << strlen(_buffer) << endl;
    }

    new_state = start_state;

    int buf_len = static_cast<int>(strlen(_buffer));

    // while_pos is less than the entire _buffer length
    // the length is equal to six (it includes the null char)
    for (; _pos < buf_len; ++_pos)
    {
        // check the state
        new_state = int(_table[new_state][int(_buffer[_pos])]);
        // if we haven't reached the end of the input string, and if
        // the state is not a fail state
        if (new_state != -1 && _buffer[_pos] != '\0')
        {
            success = is_success(_table, new_state);
        }
        // if we go into a fail state, return the valid token part
        // up to the fail state
        else
        {
            // this code is used in differentiating decimals.
            // 32. and 32.. etc.
            // check "output with difficult chars"
            if (int(_table[start_state][int(_buffer[_pos - 1])]) == 2)
            {
                if (!is_success(_table, int(_table[start_state]
                                                  [int(_buffer[_pos - 1])])))
                {
                    _pos--;
                }
            }
            token = aux.substr(orgPos, _pos - orgPos);
            return false;
        }
    }

    success = is_success(_table, new_state);
    // checks if we end on a success state or not,
    // if not we will seperate the final tokens
    // like in 3.
    if (!success)
    {
        _pos--;
        token = aux.substr(orgPos, _pos - orgPos);
        return false;
    }

    // sets valid token part to string
    if (debug)
    {
        cout << "pos: " << _pos << endl;
    }
    token = aux.substr(orgPos, _pos - orgPos);
    if (debug)
    {
        cout << token << endl;
    }
    return success;
}

// Postcondition: create state machines for all the types we will
// recognize: doubles, words, etc
void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    // NUMBER MACHINE
    init_table(_table);
    mark_fail(_table, 0);
    mark_fail(_table, 2);
    mark_success(_table, 1);
    mark_success(_table, 3);
    mark_fail(_table, 4);
    mark_cells(0, _table, "0123456789", 1);
    mark_cells(0, _table, ".", 2);
    mark_cells(0, _table, " \n\t", -1);
    mark_cells(1, _table, " \n\t", -1);
    mark_cells(2, _table, " \n\t", -1);
    mark_cells(3, _table, " \n\t", -1);
    mark_cells(1, _table, "0123456789", 1);
    mark_cells(1, _table, ".", 2);
    mark_cells(2, _table, "0123456789", 3);
    mark_cells(3, _table, "0123456789", 3);
    mark_cells(2, _table, ".", -1);
    mark_cells(3, _table, ".", -1);
    mark_cells(2, _table, " \n\t", -1);
    mark_cells(3, _table, " \n\t", -1);

    // SPACE MACHINE
    mark_success(_table, 4);
    mark_fail(_table, 5);
    mark_cells(4, _table, " \n\t", 4);
    mark_cells(5, _table,
               "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
               ".!?,_-:;/\'\"\\()+-=",
               -10);

    // adds unicode accents into alphabet state machine
    char accented_letters[52] =
        {// ´acute [resumé]
         char(193), char(201), char(205), char(211), char(218), char(221),
         char(225), char(233), char(237), char(243), char(250), char(253),
         char(180),
         // ∼ (Tilde) [mañana]
         char(195), char(209), char(213), char(227), char(241), char(245),
         char(126),
         // ¸ (Cedilla)[façade]
         char(231), char(184),
         //^ ˆ (Circumflex) [château]
         char(194), char(202), char(206), char(212), char(219), char(226),
         char(234), char(238), char(244), char(251), char(94),
         //` (Grave) [voilà]
         char(192), char(200), char(204), char(210), char(217), char(224),
         char(232), char(236), char(242), char(249)};

    // ALPHABET MACHINE
    mark_success(_table, 6);
    mark_fail(_table, 7);
    mark_cells(6, _table, accented_letters, 6);
    mark_cells(6, _table,
               "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 6);
    mark_cells(7, _table,
               "0123456789 \n.!?,_-:;/@#$%^&*()'\"\\()+-=\t", -1);

    // PUNCTUATION MACHINE
    mark_success(_table, 8);
    mark_fail(_table, 9);
    mark_cells(8, _table,
               ".!?,_-*=:><@#$&^%;\'+()", 8);
    mark_cells(9, _table,
               "\"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
               "abcdefghijklmnopqrstuvwxyz \t",
               -1);

    // Quotation machine
    mark_success(_table, 10);
    mark_fail(_table, 11);
    mark_cells(10, _table,
               "\"", 10);
    mark_cells(11, _table,
               "0123456789ABCDEFGHIJKLMNOPQRSTUVWX"
               "YZabcdefghijklmnopqrstuvwxyz \t.!?,_@#$%^&*&()-*=:><;\'",
               -1);
}
