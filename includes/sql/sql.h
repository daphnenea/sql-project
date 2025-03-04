#ifndef SQL_H
#define SQL_H
#include "../table/table.h"
#include "../table/typedefs.h"
#include "../parser/parser.h"
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class SQL{

    public:
    SQL();                              //Default constructor
    Table command(string input_cmd);    //takes in input string and returns a table for the given command
    vectorlong select_recnos();         //returns the vector of record numbers
    bool is_valid();                    //returns private member variable bool flag which determines if user input is a valid command 

    private:
    //ADDED
    Map<string, Table> table_map;       //map of table objects (table name, table object)
    vectorstr _cmd_list;                //vector that holds all the command strings
    bool _is_valid_cmd;                 //bool flag to determine if input is valid
    vectorlong _recnos;                 //vector that holds selected record numbers

};

#endif