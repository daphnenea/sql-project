#include "token.h"

Tokens::Tokens() : _type(0){                                          //default constructor
}
    
Tokens::Tokens(string str, int type) : _token(str), _type(type){      //parameterized constructor
}

//prints out tokens
ostream& operator<<(ostream& outs, const Tokens& t) {

    outs << "|" << t._token << "|";

    return outs;
}

int Tokens::token_type(int state) {
    if(state == 1 || state == 7) {
        return TOKEN_NUMBER_;
    } 
    else if(state == 2) {
        return TOKEN_ALPHA_;
    }
    else if(state == 3) {
        return TOKEN_SPACE_;
    }
    else if(state == 4) {
        return TOKEN_OPERATOR_;
    }
    else if (state == 5) {
        return TOKEN_PUNC_;
    }
    else if(state == 8){
        return TOKEN_PAREN_;
    }
    else if(state == 9){
        return TOKEN_STAR_;
    }
    else if(state == 10){
        return TOKEN_QUOTE_;
    }
    else {
        return TOKEN_UNKNOWN_;
    }
}

int Tokens::type() const{   
    
    return _type;
}

string Tokens::type_string() const{          //set types 

    if(_type == TOKEN_NUMBER_) {
        return "NUMBER";
    }
    else if(_type == TOKEN_ALPHA_) {
        return "ALPHA";
    }
    else if(_type == TOKEN_SPACE_) {
        return "SPACE";
    }
    else if(_type == TOKEN_OPERATOR_) {
        return "OPERATOR";
    }
    else if(_type == TOKEN_PUNC_) {
        return "PUNC";
    }
    else if(_type == TOKEN_STAR_){
        return "STAR";
    }
    else if(_type == TOKEN_PAREN_){
        return "PAREN";
    }
    else if(_type == TOKEN_QUOTE_){
        return "QUOTE";
    }
    else {
        return "ERROR";
    }
}

string Tokens::token_str() const{ 

    return _token;
}


