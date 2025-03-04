#ifndef RELATIONAL_H
#define RELATIONAL_H

#include "operator.h"

//set_operator_type (will indicate relational or logical)
//^ Operator
//Operator():
//
class Relational: public Operator{

    const int priority = 4; 

    public:
        Relational(){
            set_priority(priority);
            set_type_op(RELATIONAL);
        }
        
        Relational(string str):Operator(str){
            set_priority(priority);
            set_type_op(RELATIONAL);
            _operator = str;
        };

        int get_priority(){

            return priority;
        }

        int get_type_operator()
        {
            return RELATIONAL;
        }

        int get_type(){
            return RELATIONAL;
        }

        string get_token(){
            return _operator;
        }

        string get_operator_reltional(){

            return _operator;
        };
        

    private:
        string _operator;
        

};



#endif