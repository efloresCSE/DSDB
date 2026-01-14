#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Token
{
public:

    //Default Constructor
    //Postcondition: the token has been initialized to an empty token
    Token(): _token(" "), _type(0){}

    //Two argument Constructor
    //Postcondition: the token has been initialized such that str is the
    //string part of the token, and type is the type of this string
    Token(string str, int type);

    //Postcondition: This will output the token ex. |it|
    friend ostream& operator << (ostream& outs, const Token& t);

    //Postcondition: returns the type of the token in int form
    int type();

    //Postcondition: /this tells you what type the token is
    //ex. ALPHA, NUMBER, SPACE, NUMBER
    string type_string();

    //Postcondition: returns the string of the token
    string token_str();

    //Postcondition: sets the token such that str is the
    //string part of the token, and type is the type of this string
    void set(string token, int type);

private:

    //string part of the token
    string _token;

    //type of token
    int _type;

};

#endif // TOKEN_H
