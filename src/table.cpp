#include "table.h"
#include "file_functions.h"

// loads existing table
Table::Table(string name)
{
    // check if file exists
    string binName = name;
    if (binName.find('.') > binName.size())
        binName += ".bin";
    if (!file_exists(binName.c_str()))
        throw error("FILE DOES NOT EXIST");
    recordCount = 0;

    // set filename to name
    filename = name;

    // build field list vector from text file
    fstream txt;
    string txtname = name;
    string temp;
    t_open_fileRW(txt, txtname += "_fields.txt");
    int count = 0;
    txt >> temp;
    while (!txt.eof())
    {
        fieldList.push_back(temp);
        count++;
        txt >> temp;
    }
    txt.close();

    // push back appropriate amount of empty mmaps
    for (size_t i = 0; i < fieldList.size(); ++i)
    {
        indices.push_back(MMap<string, int>());
    }

    // fill indices mmaps
    fstream f;
    open_fileRW(f, binName.c_str());
    Record r;
    vector<string> fields;
    int recno = 0;

    // read a record
    r.read(f, recno);
    fields = get_field_values(r);
    while (f.gcount())
    {

        // go through field values of each record and
        // insert them in their right place
        for (size_t j = 0; j < fields.size(); j++)
        {
            indices[j][fields[j]] += recno;
        }
        recno++;
        r.read(f, recno);
        fields = get_field_values(r);
    }
    recordCount = recno;
    f.close();
}

// creates table with name and field list
Table::Table(const string name, vector<string> field_list)
{

    recordCount = 0;
    // save file name
    filename = name;

    // save field list values
    for (unsigned int i = 0; i < field_list.size(); ++i)
    {
        fieldList += field_list[i];
    }

    // create a file called table_field_values.txt
    // this will hold the field values so when we close the program
    // we can re-access it
    fstream txt;
    string txtname = name;
    t_open_fileRW(txt, txtname += "_fields.txt");

    // push appropriate ammount of empty mmaps
    for (size_t i = 0; i < field_list.size(); ++i)
    {
        txt << field_list[i] << " ";
        indices.push_back(MMap<string, int>());
    }
    txt.close();

    // create empty bin file
    fstream f;
    string binName = filename;
    if (binName.find('.') > binName.size())
        binName += ".bin";
    open_fileRW(f, binName.c_str());
    f.close();
}

// inserts values into table
void Table::insert(const vector<string> field_values)
{
    if (field_values.size() != fieldList.size())
        throw error("Not enough values were entered");

    Record temp(field_values);

    save_list(temp);

    // go through list of indexes insert values of each
    // field and there recno into the appropriate multimap
    for (size_t i = 0; i < field_values.size(); ++i)
    {
        // indices.push_back(MMap<string, int>());
        indices[i][field_values[i]] += temp.getRecno();
    }
    recordCount += 1;
}

// get all records from table and sets them to a temp table
Table Table::select_all()
{
    Record r;
    string name = filename + "_temp_";
    name += to_string(getTemp());

    Table tempT(name, fieldList);
    vector<string> fields;

    // read from current tables b-file
    fstream f;
    string binName = filename;
    if (binName.find('.') > binName.size())
        binName += ".bin";
    open_fileRW(f, binName.c_str());

    int recno = 0;
    r.read(f, recno);
    fields = get_field_values(r);

    // insert each record into temp T
    while (f.gcount())
    {
        tempT.insert(fields);
        recno++;
        r.read(f, recno);
        fields = get_field_values(r);
    }
    f.close();

    // return tempT
    return tempT;
}

// Evalutes "RPN"
Table Table::select_all(vector<string> RPN)
{
    string name = filename + "_temp_";
    name += to_string(getTemp());

    string first;
    string second;
    vector<vector<int>> recordnums;
    vector<int> rn;
    vector<int> RFirst;
    vector<int> RSecond;
    vector<Record> records;
    Table tempT(name, fieldList);
    vector<string> operandStack;

    for (size_t i = 0; i < RPN.size(); ++i)
    {
        // operands
        if (RPN[i] != "=" && RPN[i] != ">" && RPN[i] != "<" && RPN[i] != "<=" && RPN[i] != ">=" && RPN[i] != "and" && RPN[i] != "or")
        {
            operandStack.push_back(RPN[i]);
        }
        // operators
        else
        {
            if (RPN[i] == "=")
            {
                // pop first two operands
                // do the operations->get record numebrs->push
                // into record numbers vector
                first = operandStack.back();
                operandStack.pop_back();
                second = operandStack.back();
                operandStack.pop_back();
                rn = get_recno_of(second, "=", first);
                recordnums.push_back(rn);
            }
            else if (RPN[i] == ">" || RPN[i] == "<" || RPN[i] == "<=" || RPN[i] == ">=")
            {
                // pop first two operands
                // do the operations->get record numebers->push
                // into record numbers vector
                first = operandStack.back();
                operandStack.pop_back();
                second = operandStack.back();
                operandStack.pop_back();
                rn = get_recno_of(second, RPN[i], first);
                recordnums.push_back(rn);
            }
            // if we get and or or we need to do union or intersection
            else if (RPN[i] == "and" || RPN[i] == "or")
            {
                // pop last two vector of records
                RFirst = recordnums.back();
                recordnums.pop_back();
                RSecond = recordnums.back();
                recordnums.pop_back();
                // do set intersection or union on these vectors
                // and push the result back into the recordnumbers
                // vector
                if (RPN[i] == "and")
                {
                    // intersection
                    rn = intersection(RSecond, RFirst);
                    recordnums.push_back(rn);
                }
                else if (RPN[i] == "or")
                {
                    rn = VUnion(RSecond, RFirst);
                    recordnums.push_back(rn);
                }
            }
        }
    }

    // sort to keep in order that they appear in table
    // VSort(recordnums.back());
    // Push back
    records = get_records(recordnums.back());
    tempT.fill_table(records);

    return tempT;
}

// print table like in prompt
void Table::print_table(ostream &outs)
{
    const int fieldWidth = 15;
    const char separator = ' ';

    // print field names
    outs << "Table name: " << filename << ", "
         << "records: " << recordCount << endl;
    outs << left << setw(6) << setfill(separator) << "record";
    for (unsigned int i = 0; i < fieldList.size(); ++i)
    {
        outs << right << setw(fieldWidth) << setfill(separator)
             << fieldList[i];
    }
    outs << endl
         << endl;

    // output records
    fstream f;
    string binName = filename;
    if (binName.find('.') > binName.size())
        binName += ".bin";
    open_fileRW(f, binName.c_str());
    Record r;
    for (int recno = 0; recno < recordCount; ++recno)
    {
        r.read(f, recno);
        r.setFieldCount(fieldList.size());
        outs << right << setw(6) << setfill(separator)
             << recno;
        outs << r << endl;
    }
    f.close();
}

ostream &operator<<(ostream &outs, Table &t)
{
    t.print_table(outs);
    return outs;
}

// use this in select, gets {field op value} and returns
// the appropriate vector of record numbers
vector<int> Table::get_recno_of(const string &field,
                                string op, string val)
{
    int row = -1;
    vector<string> entries;
    stringstream ss;
    vector<int> recnos;
    vector<int> NE = {};
    int j = 0;
    for (size_t i = 0; i < fieldList.size(); ++i)
    {
        // if field is in the field list
        // return the approp value
        if (field == fieldList[i])
        {
            row = (int)i;
            break;
        }
        // set row to negative 1
        else
        {
            row = -1;
        }
    }
    // if row is negative one, throw error
    //(does not exist)
    if (row == -1)
    {
        throw error("Field does not exist");
    }
    else
    {
        // equality
        if (op == "=")
        {
            // check if the value is in indices
            // if so return the appropriate records
            if (indices[row].contains(val))
                return indices[row][val];
            // if not, output a message and continue
            else
            {
                cout << "(" << val << " is not found in indices)" << endl;
                return NE;
            }
        }
        // greater than
        else if (op == ">")
        {
            // check if the record is in indices
            if (indices[row].contains(val))
            {
                for (MMap<string, int>::Iterator it =
                         indices[row].lower_bound(val);
                     it != indices[row].end(); it++)
                {
                    // create a mpair object with val
                    MPair<string, int> ey(val);

                    // load string stream with recnos
                    // skipping the first record
                    if (ey != *it)
                        ss << *it;
                }
                // fill recnos vector with recnos
                while (ss >> j)
                {
                    // make sure j doesn't show up twice
                    if (!(find(recnos.begin(), recnos.end(), j) != recnos.end()))
                    {
                        // push it into our vector
                        recnos.push_back(j);
                    }
                }
                return recnos;
            }
            else
            {
                // start at the beginning, go until the end
                for (MMap<string, int>::Iterator it =
                         indices[row].begin();
                     it != indices[row].end(); it++)
                {
                    // create an mpair object of val and compare
                    MPair<string, int> ey(val);
                    if (*it > ey)
                    {
                        ss << *it;
                    }
                }
                while (ss >> j)
                {
                    if (!(find(recnos.begin(), recnos.end(), j) != recnos.end()))
                    {
                        recnos.push_back(j);
                    }
                }
                return recnos;
            }
        }
        // just like greater than
        else if (op == "<")
        {
            if (indices[row].contains(val))
            {
                for (MMap<string, int>::Iterator it =
                         indices[row].begin();
                     it != indices[row].upper_bound(val); it++)
                {
                    MPair<string, int> ey(val);
                    // load string stream with recnos
                    if (ey != *it)
                        ss << *it;
                }
                // fill recnos vector with recnos
                while (ss >> j)
                {
                    if (!(find(recnos.begin(), recnos.end(), j) != recnos.end()))
                    {
                        // need to no
                        recnos.push_back(j);
                    }
                }
                return recnos;
            }
            else
            {
                // start at the beginning, go until the end
                for (MMap<string, int>::Iterator it =
                         indices[row].begin();
                     it != indices[row].end(); it++)
                {
                    // create an mpair object of val and compare
                    MPair<string, int> ey(val);
                    if (*it < ey)
                    {
                        ss << *it;
                    }
                }
                while (ss >> j)
                {
                    if (!(find(recnos.begin(), recnos.end(), j) != recnos.end()))
                    {
                        recnos.push_back(j);
                    }
                }
                return recnos;
            }
        }
        // just like greater than
        else if (op == ">=")
        {
            // don't skip the first element this time
            if (indices[row].contains(val))
            {
                for (MMap<string, int>::Iterator it =
                         indices[row].lower_bound(val);
                     it != indices[row].end(); it++)
                {
                    // load string stream with recnos
                    ss << *it;
                }
                // fill recnos vector with recnos
                while (ss >> j)
                {
                    if (!(find(recnos.begin(), recnos.end(), j) != recnos.end()))
                    {
                        recnos.push_back(j);
                    }
                }
                return recnos;
            }
            else
            {
                // start at the beginning, go until the end
                for (MMap<string, int>::Iterator it =
                         indices[row].begin();
                     it != indices[row].end(); it++)
                {
                    // create an mpair object of val and compare
                    MPair<string, int> ey(val);
                    if (*it > ey)
                    {
                        ss << *it;
                    }
                }
                while (ss >> j)
                {
                    if (!(find(recnos.begin(), recnos.end(), j) != recnos.end()))
                    {
                        recnos.push_back(j);
                    }
                }
                return recnos;
            }
        }
        // just like <
        else if (op == "<=")
        {
            if (indices[row].contains(val))
            {
                for (MMap<string, int>::Iterator it =
                         indices[row].begin();
                     it != indices[row].upper_bound(val); it++)
                {
                    // load string stream with recnos
                    ss << *it;
                }
                // fill recnos vector with recnos
                while (ss >> j)
                {
                    if (!(find(recnos.begin(), recnos.end(), j) != recnos.end()))
                    {
                        recnos.push_back(j);
                    }
                }
                return recnos;
            }
            else
            {
                // start at the beginning, go until the end
                for (MMap<string, int>::Iterator it =
                         indices[row].begin();
                     it != indices[row].end(); it++)
                {
                    // create an mpair object of val and compare
                    MPair<string, int> ey(val);
                    if (*it < ey)
                    {
                        ss << *it;
                    }
                }
                while (ss >> j)
                {
                    if (!(find(recnos.begin(), recnos.end(), j) != recnos.end()))
                    {
                        recnos.push_back(j);
                    }
                }
                return recnos;
            }
        }
    }
    return NE;
}

// Post: gets field values from a record
vector<string> Table::get_field_values(Record r)
{
    vector<string> fields;
    string temp;

    // not necessary
    stringstream ss;

    ss << r;

    // cheanged to use getEntry
    for (unsigned int i = 0; i < fieldList.size(); ++i)
    {
        temp = r.getEntry(i);
        fields.push_back(temp);
    }

    return fields;
}

// gets desired records from file
vector<Record> Table::get_records(vector<int> recnos)
{
    fstream f;
    string binName = filename;
    if (binName.find('.') > binName.size())
        binName += ".bin";

    open_fileRW(f, binName.c_str());

    vector<Record> records;

    Record r;
    for (size_t i = 0; i < recnos.size(); ++i)
    {
        // if non existant record
        if (recnos[i] == -1)
        {
            f.close();
            return records;
        }
        r.read(f, recnos[i]);
        records.push_back(r);
    }

    f.close();
    return records;
}

// fills table with records in a vector
void Table::fill_table(vector<Record> records)
{
    vector<string> fields;
    for (size_t i = 0; i < records.size(); ++i)
    {
        // get field values from record
        fields = get_field_values(records[i]);
        // insert into this table
        insert(fields);
    }
}

// gets a random number to differentiate temp tables
int Table::getTemp()
{
    Random rand;
    return rand.GetNext(0, 200);
}

// cleans the table function
void Table::clean_up()
{
    for (size_t i = 0; i < indices.size(); ++i)
        indices[i].clearMap();

    // remove temp file and txt files
    string binName = filename;
    if (binName.find('.') > binName.size())
        binName += ".bin";
    remove(binName.c_str());

    string txtName = filename;
    txtName += "_fields.txt";
    remove(txtName.c_str());
}

// saves a record to the b-file
void Table::save_list(Record &list)
{
    fstream f;
    string binName = filename;

    // Ensure filename has ".bin" extension
    if (binName.find('.') == string::npos)
    {
        binName += ".bin";
    }

    try
    {
        // Open file in read/write and binary mode
        open_fileRW(f, binName.c_str());

        // Write the record and set its record number
        int Recno = list.write(f);
        list.setRecno(Recno);
    }
    catch (const exception &e)
    {
        cerr << "Error in save_list: " << e.what() << endl;
        // Handle cleanup or rethrow if needed
    }
    catch (...)
    {
        cerr << "Unknown error in save_list." << endl;
    }

    // Close file safely
    if (f.is_open())
    {
        f.close();
    }
}
