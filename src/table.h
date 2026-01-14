#ifndef TABLE_H
#define TABLE_H

#include "map.h"
#include "mmap.h"
#include "record.h"
#include "error.h"



using namespace std;

class Table
{
public:
/*
 * *************************************************************
 *                       C T O R S
 * *************************************************************
*/
    //loads existing table
    Table(string name);
    //creates table with name and field list
    Table(string name, vector<string> field_list);

/*
 * *************************************************************
 *           C O M M A N D    F U N C T I O N S
 * *************************************************************
*/
    //inserts values into table
    void insert(const vector<string> field_values);

    //Returns all records from table as a temp table
    //e.g Select * from student
    Table select_all();

    //Evaluates an RPN expression, and return valid records
    //from table as a temp table
    //e.g select * from student where lname = Jo and fname = Bob
    Table select_all(vector<string> RPN);

/*
 * *************************************************************
 *                       O U T P U T
 * *************************************************************
*/
    //prints a nice table in the way that was demonstracted
    void print_table(ostream& outs);
    friend ostream& operator <<(ostream& outs, Table&t);


/*
 * *************************************************************
 *              F I L E     F U N C T I O N S
 * *************************************************************
*/
    //saves a record to the b-file
    void save_list(Record& list);

/*
 * *************************************************************
 *     A C C E S O R    A N D   E X T R A   F U N C T I O N S
 * *************************************************************
*/
    //return vector of record numbers that satisfy the values given
    vector<int> get_recno_of(const string & field, string op, string val);

    //returns vector of records that match the record numbers given
    vector<Record> get_records(vector<int> recnos);

    //gets field values from record
    vector<string> get_field_values(Record r);

    //fills a table with a given vector of records
    void fill_table(vector<Record> records);

    //gets a random number to add to temp table name
    int getTemp();

    //gets the name of the table
    string getName(){return filename;}

    //cleans up temp table data. Deletes all files associated with it
    void clean_up();

/*
 * *************************************************************
 *              P R I V A T E   V A R I A B L E S
 * *************************************************************
*/
private:

    //Vector that holds a mmap of string to record number
    //each mmap symbolizes a field such as lastname, firstname or age
    vector<MMap<string, int>> indices;

    //the fields given to us by a user
    vector<string> fieldList;

    //the name of our table
    string filename;

    //how many records in a table
    int recordCount;
};

#endif // TABLE_H
