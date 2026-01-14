#include "ftokenizer.h"

FTokenizer::FTokenizer(string fname)
{
    //opens file declares _f to be an input stream for it
    if(debug){cout << fname << endl;}
    if(fname.find('.') > fname.size())
        fname += ".txt";
    _f.open(fname);
    if(_f.fail())
        throw error("File does not exist");
    else
    {
        if(debug){cout << fname<< " was opened\n";}
    }

    //initializes member variables
    _more = (!_f.eof());
    _blockPos = 0;
    _pos = 0;
    _stk = STokenizer();
}

//grabs next token in file
Token FTokenizer::next_token()
{
    Token t;
    _stk >> t;
    return t;
}

//these are documented in .h file
bool FTokenizer::more()
{
    return _more;
}

int FTokenizer::pos()
{
    return _pos;
}

int FTokenizer::block_pos()
{
    return _blockPos;
}

FTokenizer& operator >> (FTokenizer& f, Token& t)
{
    //if the STokenizer is empty
    if(!f._stk.more())
    {
        //if we can't grab a new block to fill the STokenizer
        if(!f.get_new_block())
        {
            //return empty token and set _more to false
            f._more = false;
            t = Token();
            return f;
        }
    }
    //if the stokenizer is not empty, set t to a token from the file
    t = f.next_token();
    return f;
}


bool FTokenizer::get_new_block()
{
    //check if file is empty
    if(!_f.eof())
    {
        //grabs 500 tokens
        char s[MAX_BUFFER];
        _f.read(s, MAX_BUFFER);
        //if no tokens were grabbed return false
        if(_f.gcount() == 0)
            return false;

        //if we were only to semi fill the char array,
        //mark the terminating null at the appropriate position
        //and fill _stk using set string function
        if(!_f)
        {
            if(debug)
            {
                cout << "we were only able to get " << _f.gcount() << endl;
            }
            s[_f.gcount()] = '\0';
            _stk.set_string(s);
        }

        //mark the terminating null and fill _stk using the set string
        //function
        if(debug){cout << s << endl;}
        s[MAX_BUFFER-1] = '\0';
        _stk.set_string(s);

        return true;

    }
    //return false if file is empty
    else
    {
        return false;
    }

}
