#ifndef TABLE_H
#define TABLE_H
#include "typedefs.h"
#include "../bplustree/mpair.h"
#include "../binary_file/utilities.h"
#include "../binary_file/file_record.h"
#include "../binary_file/utilities.h"
#include "../token/logical.h"
#include "../token/relational.h"
#include "../token/operator.h"
#include "../token/lparen.h"
#include "../token/rparen.h"
#include "../token/resultset.h"
#include "../token/tokenstr.h"
#include "../stack/MyStack.h"
#include "../token/constants.h"
#include "string"
#include <algorithm>

using namespace std;

class Table {

    static int serial_number;                                               //serial number which is unique for each table created

    public:
    
        Table();                                                            //Default Table constructor (should not be called)
        Table(string file_name);                                            //Table constructor with one parameter: file name
        Table(string file_name, vectorstr categories);                      //Table constructor with two parameters: file name and field names
        void insert_into(vectorstr row);                                    //inserts data into table
        Table select(vectorstr categories, string str1, string op, string str2); //returns Table object of requested search conditions
        Table select(vectorstr categories, Queue<Token*> conditions);   //returns Table object of requested search conditions
        Table select(vectorstr categories, const vectorstr& conditions);//returns Table object of requested search conditions
        vectorlong select_recnos();                                     //returns member variable vectorlong of selected recnos (resets each command)
        vectorstr get_fields();                                         //returns memeber variable vectorstr of field names



        friend ostream &operator<<(ostream& outs, const Table& table) {
            
            bool debug = false;
            FileRecord r2;
            fstream fileStream; 
            long bytes;
            //open the file for reading and writing.
            open_fileRW(fileStream, table._table_name.c_str());

            int i = 0;
            bytes = r2.read(fileStream, i);
            if(debug) cout << "number of records: " << table._recnos << endl;
            cout << endl << "Table name: " << table._table_name.c_str() << ", records: " << table._recnos << endl;
            cout << setw(27) << "record";
            for(int k = 0; k < table._field_count; k++){
                if(debug) cout << "field count :" << table._field_count << endl;
                cout << setw(25) << table._fields[k];
            }
            cout << endl << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
            for(int i = 0; i < table._recnos; i++){
                
                outs << setw(25) << i << r2 << endl;
                //r2._record to print
                bytes = r2.read(fileStream, i + 1);
                
            }

            fileStream.close();
            
            return outs;
        };

    private:
        string _table_name;
        long _recnos = 0;   
        int _field_count = 0;
        vectorlong _select_vec;
        vectorstr _old_categories;

        //vector of field multimaps 
        vector<mmap_sl> _mmap_vec;

        //vector of field names
        vectorstr _fields;

        //map of fields (field, index)
        map_sl _field_map;

        //vector of integers for record numbers
        vector<FileRecord> _record_vec;
        

        vectorlong rpn_eval(const Queue<Token*>& conditions);       //helper function to evaluate for rpn (returns vectorlong of selected recnos)
        void printQueue(const Queue<Token*>& q);                    //prints queue (debug function)
        vectorlong eval(string str1, string str2, string op);       //helper function to evaluate relational statements called in rpn_eval() 
                                                                    //returns vectorlong of selected recnos

        vectorlong logical_eval(vectorlong vec1, vectorlong vec2, string op);       //helper function to evaluate logical statements called in rpn_eval()
                                                                                    //returns vectorlong of selected recnos
        Queue<Token*> shunting_yard(const Queue<Token*>& infix);                    //returns postfix queue 

        void reorder_insert(Table &selected_recnos, vectorstr insert_data, long recno, FileRecord& fileRec, fstream& fileStream); //reorders indicies of fields

};


#endif