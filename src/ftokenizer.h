#ifndef FTOKENIZER_H
#define FTOKENIZER_H

#include "mylib.h"
#include "stokenizer.h"

using namespace std;

class FTokenizer
{
public:
    //each block holds up to 500 chars
    const int MAX_BLOCK = MAX_BUFFER;

    //Precondition: fname is the name of a file
    //Postcondition: opens file and sets it to input file stream variable
    //_f.
    //sets _pos and _blockpos to 0. sets _more to true or false depending
    //on the file given. Initializes _stk to an empty STokenizer object.
    FTokenizer(string fname);

    //Postcondition: returns the next token from STokenizer object _stk
    Token next_token();

    //Postcondition: returns the current value of _more
    bool more();
    //returns the value of _pos
    int pos();
    //returns the value of _blockpos
    int block_pos();

    //Precondition: the file is not empty and _stk is not empty.
    //If there are still tokens in the file, will call get_new_block
    //to refill_stk.
    //If non of these conditions are met will return an empty token and
    //set _more to false.
    //Precondition: Recieves a token from the file and sets it to t
    friend FTokenizer& operator >> (FTokenizer& f, Token& t);

private:
    bool get_new_block(); //gets the new block from the file

    std::ifstream _f;   //file being tokenized

    STokenizer _stk;     //The STokenizer object to tokenize current block

    int _pos;           //Current position in the file

    int _blockPos;      //Current position in the current block

    bool _more;         //false if last token of the last block
                        //  has been processed and now we are at
                        //  the end of the last block.
};
#endif // FTOKENIZER_H
