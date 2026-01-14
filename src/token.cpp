#include "stokenizer.h"

// documentation in header file as well

// two argument constructor for token
Token::Token(string str, int type)
{
    _token = str;
    _type = type;
}

// sets values for member variables of token
void Token::set(string token, int type)
{
    _token = token;
    _type = type;
}

// returns type translated to string form
string Token::type_string()
{
    if (_type == 1)
        return "NUMBER";
    else if (_type == 2)
        return "ALPHA";
    else if (_type == 3)
        return "PUNCT";
    else if (_type == 4)
        return "SPACE";
    else if (_type == 5)
        return "QUOT";
    else if (_type == 6)
        return "UNKNOWN";

    return "UNKNOWN";
}

// returns string member variable
string Token::token_str()
{
    return _token;
}

// returns type in its int form
int Token::type()
{
    return _type;
}

// outputs string part of token ex. |3.14|
ostream &operator<<(ostream &outs, const Token &t)
{
    outs << t._token;
    return outs;
}
