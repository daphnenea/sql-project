#include "ftokenize.h"
#include <cstring>
#include <iostream>
using namespace std;
    
FTokenizer::FTokenizer(char* fname) {
    _more = true;
    _pos = 0;
    _blockPos = 0;
}

Tokens FTokenizer::next_token() {
    Tokens t;

    return t;
}
bool FTokenizer::more() {

    return false;
}       
int FTokenizer::pos() {

    return 1;
}          
int FTokenizer::block_pos() {

    return 1;
}     

FTokenizer& operator>> (FTokenizer& f, Tokens& t) {
    
    return f;
}

bool FTokenizer::get_new_block(){

    return false;
} //gets the new block from the file