#ifndef FILE_RECORD_CPP 
#define FILE_RECORD_CPP

#include "file_record.h"

using namespace std;

//----------------------------------------------------------------------------
//                       R E C O R D
//----------------------------------------------------------------------------

//default constructor
FileRecord::FileRecord(){
    for(int i = 0; i < ROWS; ++i){
        //initialize chars to null 
        for(int j = 0; j < COLS; j++) {
            _record[i][j] = '\0';
        }
    }
    recno = -1;
}

//constructor for 2D char array
FileRecord::FileRecord(char str[][COLS]){

    int i;
    for(i = 0; i < ROWS; i++){
        strncpy(_record[i], str[i], MAX);
    }

    for(int k = 0; k < i; k++){
        // chars to null 
        for(int j = 0; j < COLS; j++) {
            _record[i][j] = '\0';
        }
    }

    recno = -1;
}

//constructor for vector of strings
FileRecord::FileRecord(const vector<string>& vec){
    
    for(int i = 0; i < vec.size(); i++){
        strncpy(_record[i], vec[i].c_str(), MAX);
    }

    for(int i = vec.size(); i < ROWS; i++){
        // chars to null 
        for(int j = 0; j < COLS; j++) {
            _record[i][j] = '\0';
        }
    }

    recno = -1;
}

long FileRecord::write(fstream &outs){
    //r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record

    //write to the end of the file.
    outs.seekg(0, outs.end);


    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer
    int i;
    for(i = 0; i < ROWS; i++){
         outs.write(_record[i], sizeof(_record[i]));
        // _record[i][sizeof(_record[i])] = '\0';
    }

    return pos/sizeof(_record);  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos = recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))

    for(int i = 0; i < ROWS; i++){
        ins.read(_record[i], sizeof(_record[i]));
        _record[ins.gcount()][0] = '\0';
    }
    
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    
    return ins.gcount();

}


ostream& operator<<(ostream& outs,
                    const FileRecord& r){


    //do i have to iterate through each string to find where it ends @ null
    //can i call the read function to find its end position bc i think my operator is printing until max which is 
    //printing out ten million spaces

    for(int i = 0; i < FileRecord::MAX; i++){ 
        //dont print empty strings
        if(strlen(r._record[i]) > 0){                   
            outs  << setw(25) << r._record[i] << " ";
        }
    }
    return outs;
}

#endif