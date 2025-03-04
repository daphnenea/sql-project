#ifndef CONSTANTS_TOKENIZER_H
#define CONSTANTS_TOKENIZER_H

const int MAX_COLUMNS = 256;
const int MAX_ROWS = 100;
const int MAX_BUFFER = 200; //temp data storage used for data transfer

const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "><=!+-%&|";
const char SPACES[] = {' ', '\t', '\n', '\0'};
//included a quote in punc... XXXX
const char PUNC[] = "?.,:;'`~!";
//added for parser
const char PAREN[] = {'(', ')'};
const char STAR_[] = "*";
const char QUOTE_[] = "\"";

const int START_DOUBLE = 0;
const int START_SPACES = 4;
const int START_ALPHA = 6;
const int START_OPERATOR = 20;
const int START_PUNC = 10;

//token types:
const int TOKEN_NUMBER_ = 1;
const int TOKEN_ALPHA_ = 2;
const int TOKEN_SPACE_ = 3;
const int TOKEN_OPERATOR_ = 4;
const int TOKEN_PUNC_ = 5;

//added for parser
const int TOKEN_PAREN_ = 8;
const int TOKEN_STAR_ = 9;
const int TOKEN_QUOTE_ = 10;

const int TOKEN_UNKNOWN_ = -1;
const int TOKEN_END_ = -2;

#endif