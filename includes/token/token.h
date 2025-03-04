#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include "constants.h"

using namespace std;

class Token{
    public:

    Token(){
        set_type(TOKEN);
        _token = "";
    }

    Token(string str){
        set_type(TOKEN);
        _token = str;
    };

    friend ostream &operator<<(ostream& outs, const Token& token) {
        
        outs << token._token;
        return outs;
    };

    virtual int get_type(){

        //cout << "in get_type token" << endl;
        return TOKEN;
    };

    virtual string get_token(){
        return _token;
    }

    void set_type(int type){
        _type = type;
    };
        
    private:
    string _token;
    int _type;


};

#endif