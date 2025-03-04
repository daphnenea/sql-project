#ifndef OPERATOR_H
#define OPERATOR_H
#include "token.h"
#include "constants.h"

class Operator: public Token{
    public:
        Operator(){
            set_type(OPERATOR);
            set_type_op(0);
            set_priority(0);
            _token = "";
        };

        Operator(string str): Token(str) {
            set_type(OPERATOR);
            set_type_op(0);
            _token = str;
        };

        int get_type(){
            
            return OPERATOR;
        }
        virtual int get_type_operator()
        {
            return 0;
        }
        void set_type_operator(int type_operator)
        {
            _type_operator = type_operator;
        }

        int get_type_op(){

            return _type;
        }
        void set_type_op(int type){

            _type = type;
        }

        virtual int get_priority(){

            return _priority;
        }

        string get_token(){
            return _token;
        }

        virtual void set_priority(int priority){

            _priority = priority;
        }

    private:
        string _token;
        int _type;
        int _priority;
        int _type_operator;
};




#endif