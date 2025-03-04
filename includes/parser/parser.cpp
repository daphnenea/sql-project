#ifndef PARSER_CPP
#define PARSER_CPP
#include "parser.h"
#include "../tokenizer/stokenize.h"
#include "../bplustree/multimap.h"
#include "../bplustree/map.h"
#include "../bplustree/bplustree.h"
#include "../bplustree/pair.h"
#include "../bplustree/btree_array_funcs.h"
#include "../tokenizer/state_machine_functions.h"
#include "../queue/MyQueue.h"


    Parser::Parser(){

    }

    Parser::Parser(char* input){

        //sest input string
        _input_str = input;

        //build keyword map
        build_keyword_list();

        //initialize "make" adjacency matrix (state table)
        make_adjacency_table();

        set_string(input);
        
        
    }

    void Parser::set_string(char* str){
       
        //tokenize input buffer into input queue
        
        bool debug = false;
        int eNum, state;
        if(debug) cout << "in set string" <<endl; 

        while(!_input_q.empty())
        {
            _input_q.pop();
        }
        int quote_count = 0;
        string quote_string;
        STokenizer stk(str);
        Tokens t;
        stk>>t;
        while(stk.more()){
            //process token here...
            //cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;

            if(t.type() != TOKEN_SPACE_ && t.type() != TOKEN_PUNC_ &&  t.type() != TOKEN_QUOTE_ && quote_count <= 0){
               
               _input_q.push(t.token_str()); 
            }
            if(quote_count == 1)
            {
                if(t.type() !=  TOKEN_QUOTE_)
                    quote_string += t.token_str();
            }
            if(t.type() == TOKEN_QUOTE_)
            {
                quote_count++;
                if(quote_count == 2){
                    if(debug) cout<<"quote_string: "<<quote_string<<endl;
                    _input_q.push(quote_string);
                    quote_count = 0;
                    //when found quote -> clear string (assign empty str)
                    quote_string = "";
                }
            }

            if(debug) cout << "enum for " << t.token_str() << " :" << eNum << endl;

            t = Tokens();

            stk >> t;
        }

        if(debug) cout << "tokenized queue: " << _input_q << endl;

        //set fail flag
        _state_flag = get_parser_tree(_input_q);

    }

    string Parser::get_string(){

        return _input_str;
    }

    int Parser::get_column(string input_str){

        //if string is not in keyword map then return symbol enum
        build_keyword_list();
        
        if(!(_keyword_map.contains(input_str))){
            return SYMBOL;
        }

       return _keyword_map.at(input_str);
    }

    bool Parser::get_parser_tree(Queue<string> input_q){
        //set parser tree
        bool debug = false;
        int eNum; 
        int state = 0;
        bool success_state = false;
        bool fail_state = false;
        string tok;
        string y = "yes";
        string star = "*";
        _parser_tree.clear();
        for(Queue<string>::Iterator it = input_q.begin(); it != input_q.end(); it++){
            //cout<<" in queue loop get_parser_tree: "<<endl;
            tok = *it;

            eNum = get_column(tok);

            state = _adjacency_table[state][eNum];
            // print_table(_adjacency_table);
            
            
            if(debug) cout<<"state: "<<state<<endl;
            switch(state){
                case 1: 
                    _parser_tree["command"] += tok;
                    break;
                case 2:
                    _parser_tree["fields"] += star;
                    break;
                case 3:
                    _parser_tree["fields"] += tok;
                    break;
                case 4:
                    //from doesnt matter
                    break;
                case 5:
                    _parser_tree["table_name"] += tok;
                    success_state = true;
                    break;
                case 6:
                    _parser_tree["where"] += y;
                    break;
                case 7:
                    _parser_tree["condition"] += tok;
                    success_state = true;
                    break;
                case 8:
                    _parser_tree["command"] += tok;
                    break;   
                case 9:
                    //into doesnt matter
                    break;
                case 10:
                    _parser_tree["table_name"] += tok;
                    break;
                case 11:
                    //values doesnt matter
                    break;
                case 12:
                    _parser_tree["values"] += tok;
                    success_state = true;
                    break;
                case 13:
                    _parser_tree["command"] += tok;
                    break;
                case 14:
                    //table doesnt matter
                    break;   
                case 15:
                    _parser_tree["table_name"] += tok;
                    break;
                case 16:
                    //fields doesnt matter
                    break;  
                case 17:
                    _parser_tree["col"] += tok;
                    success_state = true;
                    break;
                case 18:
                    _parser_tree["drop"] += tok;
                    break;
                case 19:
                    //table doesnt matter
                    break;
                case 20:
                    _parser_tree["table_name"] += tok;
                    success_state = 0;
                    break;    
            }
        }
        if(debug) cout<<"parser tree in get_parser_tree"<< _parser_tree<<endl;

        //return true if ends in success state, else return false
        if(success_state){
            return success_state;
        }
        else{
            return false;
        }
    }

    map_sl Parser::build_keyword_list(){
        
        _keyword_map.insert("*", STAR);
        _keyword_map.insert("from", FROM);
        _keyword_map.insert("insert", INSERT);
        _keyword_map.insert("into", INTO);
        _keyword_map.insert("drop", DROP);
        _keyword_map.insert("select", SELECT);
        _keyword_map.insert("create", CREATE);
        _keyword_map.insert("make", MAKE);
        _keyword_map.insert("where", WHERE);
        _keyword_map.insert("fields", FIELDS);
        _keyword_map.insert("values", VALUES);
        _keyword_map.insert("table", TABLE);
        

        //cout << endl << "keyword map: " << endl << _keyword_map << endl; 

        //_keyword_map.print_map_table_form();


        return _keyword_map;
    }

    bool Parser::fail(){

        return _state_flag;
    }

    mmap_ss Parser::parse_tree(){
        bool debug = false;

        get_parser_tree(_input_q);
        if(debug) cout<<"_parser_tree in parse tree:"<<endl;
        if(debug) cout<<_parser_tree<<endl;
        if(debug) cout<<"print lookup in parse_tree: "<<endl;
        //_parser_tree.print_lookup();
        return _parser_tree;
    }

    void Parser::make_adjacency_table(){

        init_table_pars(_adjacency_table);
        
        mark_fail(_adjacency_table, 0);

        //select branch
        mark_cells(0, _adjacency_table, SELECT, SELECT, 1);
        mark_cells(1, _adjacency_table, STAR, STAR, 2);
        mark_cells(1, _adjacency_table, SYMBOL, SYMBOL, 3);
        mark_cells(3, _adjacency_table, SYMBOL, SYMBOL, 3);
        mark_cells(2, _adjacency_table, FROM, FROM, 4);
        mark_cells(3, _adjacency_table, FROM, FROM, 4);
        mark_cells(4, _adjacency_table, SYMBOL, SYMBOL, 5);
        mark_success(_adjacency_table, 5);
        mark_cells(5, _adjacency_table, WHERE, WHERE, 6);
        mark_cells(6, _adjacency_table, SYMBOL, SYMBOL, 7);
        mark_cells(7, _adjacency_table, SYMBOL, SYMBOL, 7);
        mark_success(_adjacency_table, 7);

        //insert branch
        mark_cells(0, _adjacency_table, INSERT, INSERT, 8);
        mark_cells(8, _adjacency_table, INTO, INTO, 9);
        mark_cells(9, _adjacency_table, SYMBOL, SYMBOL, 10);
        mark_cells(10, _adjacency_table, VALUES, VALUES, 11);
        mark_cells(11, _adjacency_table, SYMBOL, SYMBOL, 12);
        mark_cells(12, _adjacency_table, SYMBOL, SYMBOL, 12);
        mark_success(_adjacency_table, 12);

        //create & make branch
        mark_cells(0, _adjacency_table, CREATE, CREATE, 13);
        mark_cells(0, _adjacency_table, MAKE, MAKE, 13);
        mark_cells(13, _adjacency_table, TABLE, TABLE, 14);
        mark_cells(14, _adjacency_table, SYMBOL, SYMBOL, 15);
        mark_cells(15, _adjacency_table, FIELDS, FIELDS, 16);
        mark_cells(16, _adjacency_table, SYMBOL, SYMBOL, 17);
        mark_cells(17, _adjacency_table, SYMBOL, SYMBOL, 17);
        mark_success(_adjacency_table, 17);
        
        //drop branch
        mark_cells(0, _adjacency_table, DROP, DROP, 18);
        mark_cells(18, _adjacency_table, TABLE, TABLE, 19);
        mark_cells(19, _adjacency_table, SYMBOL, SYMBOL, 20);
        mark_success(_adjacency_table, 20);
        //cout<<"_adjacency_table: "<<endl;
        // print_table(_adjacency_table);
    }




#endif