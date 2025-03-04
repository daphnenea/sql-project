#ifndef LOGICAL_H
#define LOGICAL_H

#include "operator.h"
#include "string"

class Logical: public Operator{

    public:
        const int and_priority = 2;
        const int or_priority = 1;
        

        Logical(){
            //set_priority(priority);
            set_type_op(LOGICAL);
            
        };

        Logical(string str):Operator(str){

            if(str == "and"){
                _priority = and_priority;
            }
            else{
                _priority = or_priority;
            }
            set_type_op(LOGICAL);
            _operator = str;
            // cout << "end of logical constructor" << endl;
        };

        int get_type(){
            return LOGICAL;
        };

        int get_type_operator()
        {
            return LOGICAL;
        }

        string get_token(){

            return _operator;
        }

        string get_operator_logical(){

            return _operator;
        };

        int get_priority(){

            return _priority;
        }

    private:
        string _operator;
        int _priority;
        
};



#endif