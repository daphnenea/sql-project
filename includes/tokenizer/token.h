#ifndef TOKENIZE_H
#define TOKENIZE_H
#include "state_machine_functions.h"
#include <iostream>
#include <cstring>

using namespace std;
class Tokens{
public:
    Tokens();
    Tokens(std::string str, int type);
    friend ostream& operator <<(ostream& outs, const Tokens& t);
    int type() const;
    int token_type(int state);
    std::string type_string() const;
    std::string token_str() const;
private:
    std::string _token;
    int _type;
};

#endif
