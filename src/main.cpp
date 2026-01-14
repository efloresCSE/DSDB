/*
 * Author: Edgar Flores
 * Project: Final_Project: DATABASE
 * Purpose: The purpose of this program
 * is to allow the user to create tables,
 * insert into tables, and select entries from
 * these tables
 *
 */
#include "sql.h"
using namespace std;

void introduction();

int main()
{
    introduction();

    SQL test;
    test.run();

    return 0;
}

void introduction()
{
    string border(70, '*');
    cout << border << endl;
    cout << "The purpose of this program is to allow the user"
            " to work with a DATABASE!\n";
    cout << border << endl;
}
