#ifndef RESULTSET_H
#define RESULTSET_H

#include "token.h"

class ResultSet: public Token{
    public:
        ResultSet() {
            set_type(RESULT_SET);
        };

        ResultSet(vectorlong result_vec){

            set_type(RESULT_SET);
            _result_vec = result_vec;
        };

        vectorlong get_result(){
            return _result_vec;
        }

        void set_result(vectorlong result_vec){
            _result_vec = result_vec;
        }

    private:
        string _token;
        int _type;
        vectorlong _result_vec;
};


#endif