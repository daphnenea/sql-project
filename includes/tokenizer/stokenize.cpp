#include <iostream>
#include <cstring>
#include "stokenize.h"

using namespace std;

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];
bool done;

STokenizer::STokenizer() {                      //default constructor
    _pos = 0;
    make_table(_table);
}
    
STokenizer::STokenizer(char str[]) {           //parameterized constructor 
    _pos = 0;
    make_table(_table);

    set_string(str);
}
    
bool STokenizer::done(){

    if(_pos == strlen(_buffer)+1) {
        return true;
    }
    if(_buffer[_pos] == '\0'){
        _pos++;
        return false;
    }

    return false;
}            //true: there are no more tokens

bool STokenizer::more() {

    return !done();
}            //true: there are more tokens

    //set a new string as the input string
void STokenizer::set_string(char str[]) {

    strncpy(_buffer, str, MAX_BUFFER - 1);
    _buffer[MAX_BUFFER -1] = '\0';          //manually insert null at end of string
    _pos = 0;

}

//---------------
//extract one token (very similar to the way cin >> works)    
STokenizer& operator>> (STokenizer& s, Tokens& t) {
    
    string extractToken;
    int start_state = 0;
    if (s.get_token(start_state, extractToken)){
        //cout<<"get token returned true for state: "<< start_state << endl;
        t = Tokens(extractToken,(t.token_type(start_state)));
    }
    else{
        cout<<"get token returned false: "<<start_state<<endl;

    }
    return s;
    /*
    if (s.get_token(start_state, extractToken)) {
        t = Token(extractToken,(t.token_type(start_state)));
    }
    */
}

    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
void STokenizer::make_table(int _table[][MAX_COLUMNS]) {

    init_table(_table);

/*
    _table[0][0] = 0;  
    _table[1][0] = 1;
    _table[2][0] = 0;
    _table[3][0] = 1;
    _table[4][0] = 1;
    _table[5][0] = 0;
    _table[6][0] = 1;
    _table[0]['a'] = 1;
    _table[0]['b'] = 2;
    _table[0]['c'] = 4;
    _table[0]['d'] = 5;
    _table[1]['a'] = 1;
    _table[1]['b'] = 2;
    _table[2]['a'] = 3;
    _table[3]['a'] = 3;
    _table[4]['c'] = 4;
    _table[4]['d'] = 5;
    _table[5]['c'] = 6;
    _table[6]['c'] = 6;

*/
   //print_table(_table);

    mark_fail(_table, 0);
    mark_success(_table, 1);
    mark_success(_table, 2);
    mark_success(_table, 3);
    mark_success(_table, 4);
    mark_success(_table, 5);

    //added for parser
    mark_success(_table, 8);
    mark_success(_table, 9);
    mark_success(_table, 10);

    mark_fail(_table, 6);
    mark_success(_table, 7);
    mark_cells(0, _table, DIGITS, 1);
    mark_cells(0, _table, ALFA, 2);
    mark_cells(0, _table, SPACES, 3);
    mark_cells(0, _table, OPERATORS, 4);
    mark_cells(0, _table, PUNC, 5);
    //added for parser
    mark_cells(0, _table, PAREN, 8);
    mark_cells(0, _table, STAR_, 9);
    mark_cells(0, _table, QUOTE_, 10);

    //Marking Digits as 1
    mark_cells(1, _table, DIGITS, 1);
    //Marking for decimal
    mark_cell(1, _table, '.', 6);

    mark_cells(2, _table, ALFA, 2);
    mark_cells(3, _table, SPACES, 3);
    mark_cells(4, _table, OPERATORS, 4);
    mark_cells(5, _table, PUNC, 5);
    mark_cells(6, _table, DIGITS, 7);
    mark_cells(7, _table, DIGITS, 7);
    mark_cells(10, _table, QUOTE_, 10);

    //cout<<"print_table: " <<endl;
    //print_table(_table);
}

/*

STokenizer::get_token(a,b){get_token(a, b, pos, state, table){

    hard coded table for 'a' 'b' state machine
    _table[0][0] = 0;  
    _table[1][0] = 1;
    _table[2][0] = 0;
    _table[3][0] = 1;
    _table[0]['a'] = 1;
    _table[0]['b'] = 2;
    _table[1]['a'] = 1;
    _table[1]['b'] = 2;
    _table[2]['a'] = 3;
    _table[3]['a'] = 3;
    }

*/

bool STokenizer::gettoken(char input[], int& pos, int& start_state, std::string& token) {
    bool debug = false;
    char current_char = input[pos];
    int current_state = start_state;
    int success_pos = 0;
    int success_state = 0;
    int start_pos = pos;
    int unsuccess_pos = 0;
    int unsuccess_state = 0;
    
    current_state = _table[start_state][current_char];
    if(debug)cout << "current state: " << current_state << endl;

                                                                // cout << "before while loop" << endl;
                                                                //cout << "first state: " << current_state << "  ";
    if(debug)cout << "first character: " << input[pos] << endl;

    while(current_state != -1 && input[pos] != '\0' ){
        //PROCESS....
                                                                     //  cout << " current state: " << current_state;
                                                                     //  cout << ", current character: " << current_char;
                                                                     //  cout << ", current position: " << pos << endl;
        if(is_success(_table, current_state)) {
            success_state = current_state;
            success_pos = pos;
            // cout << ", success state is " << success_state;
            // cout << " success pos is " << pos << endl;
        }
        else
        {
            unsuccess_state = current_state;
            // is_unsuccessful = true;                //if the state is not successful, and not a fail state (exits loop) then it is an unsuccess state
            unsuccess_pos = pos;
            // cout << ", unsuccess state is " << unsuccess_state <<endl;
            // cout << " unsuccess pos is " << unsuccess_pos << endl;
        }

        pos++;
        //find the next character:
        current_char = input[pos];
        //find the next state:
        current_state = _table[current_state][current_char];
                                                                    // cout << "cur state: " << current_state << endl;
    }

    if(debug){
        if(input[_pos] == '\0'){
            // cout << "string length: " << strlen(input) << endl;
            // cout << "the position is null" << endl;
            // cout << "current state is: " << current_state;
            // cout << endl << "current pos is: " << _pos << endl;
            if (input[_pos+134] == '\0') {
            cout << "input pos+134: " << input[_pos+134] << endl;
            }
            else {
                cout << "not null" << endl;
            }
        }
    }

    if(success_state != -1) {
        start_state = success_state;
                                                                        //cout << "end pos: " << pos << endl; 
        if(unsuccess_pos > success_pos)
        {
                                                                        //cout << "unsuccess state: " << unsuccess_state;
            for(int i = start_pos; i < unsuccess_pos; i++) {
            token += input[i];
                                                                            //cout << "current token: " << token << endl;
            }
            pos = unsuccess_pos;
        }
        else{
            for(int i = start_pos; i < pos; i++) {
            token += input[i];
                                                                            //cout << "current token: " << token << endl;
            }
        }    

        return true;
    }
    else if(success_state == -1) {
        pos = start_pos;
                                                                        //cout << "no success state" << endl;
        return false;
    }

    return false;
}

bool STokenizer::get_token(int& start_state, string& token) {

    // CHECK CONDITION FOR NOT WITHIN THE POSITION 
    
    if (gettoken(_buffer, _pos, start_state, token)) {
        return true;
    }

    return false;
}

void STokenizer::report() {
    cout << "--------------------------------------------------------" << endl;
    cout << "|         current position: " << _pos << ",  ";
    cout << "current char: " << _buffer[_pos] << "         |" << endl;
    cout << "--------------------------------------------------------" << endl;

}