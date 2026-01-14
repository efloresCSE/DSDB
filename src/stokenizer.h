#ifndef STOKENIZER_H
#define STOKENIZER_H

#include "token.h"
#include "state_machine.h"
#include "error.h"


const int MAX_BUFFER = 200;

class STokenizer
{
public:

    //Postcondition: sets _pos to zero. Sets _buffer to empty
    //creates adjacency matrix for desired states.
    STokenizer();

    //Postcondition: _buffer memeber variable has been initialized
    //to the value of str. _pos is still zero.
    //creates adjacency matrix for desired states.
    STokenizer(char str[]);

    //Postcondition: returns true if there are no more tokens
    bool done();

    //Postcondition: returns true if there are more tokens
    bool more();

    //Postcondition: extracts one token (similar tot he way cin >> work)
    //from Stokenizer s and stores the string
    //and its type in a Token object
    friend STokenizer &operator >> (STokenizer&s, Token& t);

    //Postcondition: sets a new string str
    //into the _buffer member variable
    void set_string(char str[]);

    //Postcondition: outputs everything in the STokneizer, for debugging
    friend ostream &operator << (ostream& outs, STokenizer&s);

private:

    //Postcondition: create state machines for all the types we will
    //recognize: doubles, words, etc
    void make_table(int table[][MAX_COLUMNS]);

    //Precondition: we are given a starting state,
    //and _buffer has a phrase in it
    //Postconditon: extract the longest string that match
    //one of the acceptable token types
    bool get_token(int start_state, string& token);

    //holds the input string
    char _buffer[MAX_BUFFER];

    //_pos maintains the current position in the string
    int _pos;

    //our state machines
    static int _table[MAX_ROWS][MAX_COLUMNS];

};

#endif // STOKENIZER_H
