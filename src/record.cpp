#include "record.h"

Record::Record()
{
    // set recno to -1 and fieldcount to 0
    recno = -1;
    fieldCount = 0;

    // set the 2d array entries to NULL
    memset(record, 0, sizeof(record)); // Proper initialization
}

Record::Record(vector<string> input)
{
    // set recno to -1 and fieldcount to 0
    fieldCount = 0;
    recno = -1;

    // convert input to record data
    for (size_t i = 0; i < input.size() && i < static_cast<size_t>(MAX); ++i)
    {
        strncpy(record[i], input[i].c_str(), MAX - 1);
        record[i][MAX - 1] = '\0'; // Ensure null-termination
        fieldCount++;
    }
}

long Record::write(fstream &outs)
{
    // makes sure it starts writing at end of file
    outs.seekg(0, outs.end);

    // write to the end of the file.
    // will return last position read
    long pos = outs.tellp();

    // writes to file
    outs.write(&record[0][0], sizeof(record));

    // returns recno
    return pos / sizeof(record);
}

long Record::read(fstream &ins, long recno)
{
    long pos = recno * sizeof(record);
    ins.seekg(pos, ios_base::beg);
    ins.read(&record[0][0], sizeof(record));

    fieldCount = 0;
    for (int i = 0; i < MAX; i++)
    {
        if (record[i][0] != '\0' && isprint(static_cast<unsigned char>(record[i][0])))
        {
            fieldCount++;
        }
        else
        {
            break;
        }
    }
    return ins.gcount();
}

// print record nicely
void Record::print_record(ostream &outs) const
{
    const int fieldWidth = 15;
    const char separator = ' ';

    for (int i = 0; i < fieldCount; i++)
    {
        outs << right << setw(fieldWidth) << setfill(separator)
             << record[i];
    }
}

ostream &operator<<(ostream &outs, const Record &r)
{
    r.print_record(outs);
    return outs;
}
