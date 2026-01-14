#ifndef ERROR_H
#define ERROR_H
#include <exception>

//global constant, allows all files to use debug flag
const bool debug = false;

//this is a class for error handling
//built personal exception class, inheriting
//from the generic exception class
class error: public std::exception
{
    //pointer for a message
    const char* message = nullptr;
    error() {};
public:
    //initialize with a message
    error(const char* s) throw(): message(s) {}
    //overloaded what to return message
    const char* what() const throw() {return message;}
};

#endif // ERROR_H
