#ifndef FILE_RECORD_H 
#define FILE_RECORD_H 
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

//----------------------------------------------------------------------------
//                      S T R U C T   R E C O R D
//----------------------------------------------------------------------------

struct FileRecord{

    static const int MAX = 100;
    static const int ROWS = MAX +1;
    static const int COLS = MAX +1;
    int recno;
    
    char _record[ROWS][COLS];
    // vector <string>_rec;

    //when you construct a Record, it's either empty or it
    //  contains a word
    FileRecord();
    FileRecord(char str[ROWS][COLS]);
    FileRecord(const vector<string>& vec);
    long write(fstream& outs);              //returns the FileRecord number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file
    friend ostream& operator<<(ostream& outs,
                               const FileRecord& r);                 
};


#endif 