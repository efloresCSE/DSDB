#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#include "mylib.h"

using namespace std;
//Post: opens file for writing, not necessary
void open_fileW(fstream& f, const char filename[])
{
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}
//Post: returns true if file exits
bool file_exists(const char filename[])
{
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "
                      <<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

//Post: opens a file for reading and writing
void open_fileRW(fstream& f, const char filename[])
{
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "
                          <<filename<<endl;
        }
    }
    else{
        //opens for input and output
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }
}

//checks if a text file exists
bool t_file_exists(string file_name)
{
    const bool debug = false;
    fstream ff;
    ff.open (file_name,
        std::fstream::in);
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "
                      <<file_name<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "
                  <<file_name<<endl;
    ff.close();
    return true;
}

//Post: opens a text file for reading and writing
void t_open_fileRW(fstream& f, const string file_name)
{
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!t_file_exists(file_name)){
        f.open(file_name, std::fstream::out);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<file_name<<"]"
               <<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "
                          <<file_name<<endl;
        }
    }
    else{
        //opens for input and output
        f.open (file_name,
            std::fstream::in | std::fstream::out);
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<file_name<<"]"<<endl;
            throw("file failed to open.");
        }
    }
}

#endif // FILE_FUNCTIONS_H
