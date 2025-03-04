#ifndef LPAREN_H
#define LPAREN_H
#include "operator.h"

class LParen: public Operator{

    const int priority = -2;

    public:
        LParen() {
            set_type(LPAREN);
            _token = "(";
        }
        
        LParen(string str): Operator(str){
            set_type(LPAREN);
            _token = str;
        }

        int get_priority(){

            return priority;
        }

        int get_type(){
            return LPAREN;
        }
        void set_type_lparen(string str){
            _token = str;
        }
        string get_token_lparen(){
            return _token;
        }

    private:
        string _token;
        int _type;
};

#endif