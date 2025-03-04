#ifndef TOKENSTR_H
#define TOKENSTR_H
#include "token.h"
#include "constants.h"


class TokenStr: public Token{
    public:
        TokenStr(){
            set_type(TOKEN_STR);
        };

        TokenStr(string str): Token(str){
            set_type(TOKEN_STR);
            _token = str;
        };
        
        int get_type(){
            return TOKEN_STR;
        }

        string get_type_tokenstr(){

            return _token;
        };

        void set_type_tokenstr(string token){

            _token = token;
        }



    private:
        string _token;
        int _type; 
};



#endif