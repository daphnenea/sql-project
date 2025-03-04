#ifndef RPAREN_H
#define RPAREN_H

#include "token.h"
#include "constants.h"

class RParen: public Token{
    public:
        RParen(){
            set_type(RPAREN);
            _token = ")";
        };

        RParen(string str): Token(str){
            set_type(RPAREN);
            _token = str;
        };
        
        int get_type(){
            return RPAREN;
        }

        void set_type_rparen(string str){
            _token = str;
        };

    private:
        string _token;
        int _type;
};


#endif