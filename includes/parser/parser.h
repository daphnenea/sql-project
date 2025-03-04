#ifndef PARSER_H
#define PARSER_H
#include "../table/typedefs.h"
#include "../token/token.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;


//command type enums used in state machine and numeric values of token types

enum CommandType {

    ZERO,   //0
    SELECT,  
    STAR,
    FROM,
    WHERE,
    INSERT,    
    INTO,
    VALUES,
    DROP,
    CREATE,
    MAKE,
    TABLE,
    FIELDS,
    
    SYMBOL  //13   
};


class Parser {

    public:

    Parser();                                   //Default constructor
    Parser(char* input);                        //One parameter constructor that takes in the user input to be parsed
    void set_string(char* str);                 //Tokenize input buffer into input queue
    string get_string();                        //returns private member Queue that holds input strings
    
    int get_column(string input_str);           //returns enum correlated to token type
    bool get_parser_tree(Queue<string> input_q);    //returns bool representing un/success state
    map_sl build_keyword_list();                    //creates map of keyword commands

    bool fail();                            //bool flag to return if its in a fail state
    mmap_ss parse_tree();                   //returns parser tree created in get_parser_tree()
    
    void make_adjacency_table();            //creates adjacency table for token types (state machine)

    //initialize table to unsuccessful states
    void init_table_pars(int _table[][256]) {

        for(int i = 0; i < 100; ++i) {
            for (int j = 0; j < 256; ++j){
                _table[i][j] = -1;
                //cout << _table[i][j] << " ";
            }
        }

    }
    
    private:

    bool _state_flag;
    mmap_ss _parser_tree;
    map_sl _keyword_map;
    Queue<string> _input_q;
    char* _input_str;
    int _adjacency_table[100][256];

};


#endif