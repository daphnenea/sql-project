#ifndef STOKENIZE_H
#define STOKENIZE_H
#include "token.h"
#include "constants_tok.h"

using namespace std;
class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator>> (STokenizer& s, Tokens& t);

    //set a new string as the input string
    void set_string(char str[]);   
    void report();
private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    //template <typename T>
    void make_table(int _table[MAX_ROWS][MAX_COLUMNS]); 
    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int& start_state, string& token);
    bool gettoken(char input[], int& pos, int& start_state, std::string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};

#endif