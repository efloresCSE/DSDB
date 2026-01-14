#ifndef RECORD_H
#define RECORD_H

#include "mylib.h"

static const int MAX = 15;

using namespace std;

class Record
{
public:
/*
 * *************************************************************
 *                      C T O R
 * *************************************************************
*/
    Record();
    //single argument CTOR
    Record(vector<string> str);
/*
 * *************************************************************
 *               F I L E    F U N C T I O N S
 * *************************************************************
*/
    //write to binary file
    long write(fstream&outs);

    //read from binary file
    long read(fstream& in, long recno);
/*
 * *************************************************************
 *             O U T P U T   F U N C T I O N S
 * *************************************************************
*/
    //output record
    void print_record(ostream& outs) const;
    //overloaded insertion operator
    friend ostream& operator <<(ostream& outs, const Record&r);

/*
 * *************************************************************
 *  A C C E S O R    A N D   M U T A T O R   F U N C T I O N S
 * *************************************************************
*/
    //getting and setting private variables
    int getRecno(){return recno;}
    int getFieldCount(){return fieldCount;}
    int setRecno(int recNo){
        if (recNo < 0) {
            cerr << "Invalid recno value: " << recNo << endl;  // Debug log
            throw runtime_error("recno must be non-negative.");
        }
        recno = recNo;
        return recno;  // Optional: to verify it worked
    }
    void setFieldCount(int fc){fieldCount = fc;}

    //return specific entry in record
    string getEntry(int index){return record[index];}

/*
 * *************************************************************
 *            P R I V A T E   V A R I A B L E S
 * *************************************************************
*/
private:
    //keep track of fields entered for nice output
    int fieldCount;
    //recno
    int recno;
    //2d char array
    char record[MAX][MAX];
};

#endif // RECORD_H
