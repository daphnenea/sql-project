#ifndef TABLE_CPP
#define TABLE_CPP
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <assert.h>
#include "string"
#include "table.h"

    int Table::serial_number = 1000;

    Table::Table(){
        //empty argument constructor should not be called
        //cout << "The default table constructor is being called" << endl;
    }

    Table::Table(string file_name){ 

        _field_count = 0; 
        bool debug = false;
        
        if(debug) cout << "_FIELDS size : " << _fields.size() <<endl; 

        _table_name = file_name;
        if(debug) cout << "table name : "<< _table_name << endl;

        vectorstr categories;
        ifstream all_categories_read;
        all_categories_read.open(file_name + ".txt");
        string categories_string;


        // - - - - REVIEW THE WHILE LOOP - - - CHECK CONDITION - - - -  
        while(getline(all_categories_read, categories_string)){

            categories.push_back(categories_string);
        }

        if(debug)cout<<" CONSTRUCTOR categories size: " << categories.size() << endl;
        _fields = categories; 

        if(debug) cout << "BEFORE PUSHBACK _FIELDS -> CATEGORIES : " << _fields.size() << endl;

        if(debug)cout<<"categories: "<<categories<<endl;
        all_categories_read.close();
        ofstream all_categories;
        all_categories.open(file_name + ".txt");

        _fields.clear();

        for(int i = 0; i < categories.size(); i++){
            _field_count++;

            _fields.push_back(categories[i]);
            if(debug) cout << "category " << categories[i] << endl;

            all_categories << categories[i] << endl;

            //INSERT A NEW MULTIMAP FOR EACH FIELD IN MEMBER VECTOR MMAP
            mmap_sl new_mmap;
            _mmap_vec.push_back(new_mmap);

            //INSERT INTO MEMBER MAP (HODLS FIELDS)
            _field_map.insert(categories[i], i);
            
        }

        if(debug) cout << "AFTER PUSHBACK _FIELDS -> CATEGORIES : " << _fields.size() << endl;

        if(debug) cout <<"field map: " << _field_map <<endl;
        all_categories.close();

        //create binary file
        fstream fileStream;
        
        open_fileRW(fileStream, _table_name.c_str());
        FileRecord read_record;
        int g_count = read_record.read(fileStream, _recnos);
        while(g_count)
        {
            // read_record.read(fileStream, _recnos);
            for(int i = 0; i < categories.size(); i++)
            {
                _mmap_vec[i][read_record._record[i]] += _recnos;
            }
            _recnos++;
            g_count = read_record.read(fileStream, _recnos);
            //push into maps

        }
        if(debug)cout<<"recnos: "<<_recnos<<endl;
        fileStream.close();
        
    }

//    Table t("student", fields);

    Table::Table(string file_name, vectorstr categories){
        bool debug = false;
        _fields.clear();

        _table_name = file_name;
        if(debug) cout << "table name : "<< _table_name << endl;
        if(debug) cout << "_fields.size: " << _fields.size() <<endl;
        ofstream all_categories;
        all_categories.open(file_name + ".txt");

        for(int i = 0; i < categories.size(); i++){
            _field_count++;

            _fields.push_back(categories[i]);
            if(debug) cout << "category " << categories[i] << endl;

            all_categories << categories[i] << endl;

            //INSERT A NEW MULTIMAP FOR EACH FIELD IN MEMBER VECTOR MMAP
            mmap_sl new_mmap;
            _mmap_vec.push_back(new_mmap);

            //INSERT INTO MEMBER MAP (HODLS FIELDS)
            _field_map.insert(categories[i], i);
            
        }
        if(debug) cout <<"field map: " << _field_map <<endl;
        all_categories.close();

        //create binary file
        fstream fileStream;
        
        open_fileW(fileStream, _table_name.c_str());
        if(debug) cout << "CONSTRUCT 2 _FIELDS -> CATEGORIES : " << _fields.size() << endl;

        fileStream.close();

        /*
        * Save field names in a txt file 
        * Create a binary file (same as table name)
        */

    }

    /*
    *   HOW INSERT_INTO() IS CALLED
    ************************************
    *   row = {"Joe", "Gomez", "20"};
    *   t.insert_into(row);
    */

    void Table::insert_into(vectorstr row) {

        bool debug = false;

        FileRecord new_rec(row);
        _recnos++;
        if(debug) cout << "recno " << _recnos << endl;

        //push every mmap item vector-> field and record num
        
        fstream tableFileStream;
        if(debug) cout << "This is the table name: " << _table_name.c_str() << endl;

        open_fileRW(tableFileStream, _table_name.c_str());

        if(debug) cout << "file name: " << _table_name.c_str() << endl;

        new_rec.recno = new_rec.write(tableFileStream);
        
        if(debug) cout << endl << "recno: " << new_rec.recno << endl;
        if(debug) cout <<"_field_map.size()  " << _field_map.size() <<endl;

        for(int i = 0; i < _mmap_vec.size(); i++){

            //_mmap_vec[i].insert(row[i], new_rec.recno);

            _mmap_vec[i][row[i]].push_back(new_rec.recno);

            if(debug) cout <<endl<< "mmap : " << endl;
            //if(debug) cout << _mmap_vec[i] <<endl;  
            
        }
        
        if(debug) cout << "new rec : " << new_rec <<endl; 

        if(debug) tableFileStream << new_rec << endl;
        tableFileStream.close(); 

    }

    vectorlong Table::select_recnos(){
        
        return _select_vec;
    }

    Table Table::select(vectorstr categories, string str1, string op, string str2){

        bool debug = false;
        long category_index;
        long bytes;
        string new_file_name = _table_name + "_" + to_string(serial_number);
        Table selected_recnos(new_file_name, categories);
        vectorlong select_recnos;
        vectorstr insert_data;
        FileRecord recordFile;
        fstream fileStream;

        // select_recnos._fields  -> {age, fname, lname}
        // _fields -> {fname, lname, age}

        // if(category_index == -1){
        //     assert("field does not exist");
        // }

        _select_vec.clear();
        category_index = _field_map.get(str1);

        if(debug) cout << "selected_recnos._fields.size(): " << selected_recnos._fields.size() <<endl;
        if(debug) cout << "parameter vecstr categories.size(): " << categories.size() <<endl;

    /*
    * cout << "Here are all the Yaos:" << endl;
    * cout << t.select({"fname", "lname", "age"}, "lname", "=", "Yao");
    */ 

        fileStream.open(_table_name);
        if(op == "="){
            if(debug)cout << "[category_index]  " << category_index <<endl;
            if(_mmap_vec[category_index].contains(str2)){

                select_recnos = _mmap_vec[category_index].get(str2);
                if(debug) cout << "selected record nums: " << select_recnos << endl;
                
                if(debug) cout << "select_renos.size: " << select_recnos.size()  << endl;

                for(int i = 0; i < select_recnos.size(); i++){

                    if(debug) cout<<"select_renos["<<i<<"]: "<<select_recnos[i] << endl;
                    bytes = recordFile.read(fileStream, select_recnos[i]);
                    if(debug) cout << "after read select_renos[" << i << "]: " << select_recnos[i] << endl;
                
                    insert_data.clear();
                    for(int j = 0; j < categories.size(); j++){
                        
                        insert_data.push_back(recordFile._record[j]);
                    }

                    reorder_insert(selected_recnos, insert_data, select_recnos[i], recordFile, fileStream);
                }
                
            }
            else{
                cout << "item doesn't exist" <<endl;
            }
        }
        else if(op == ">"){
            mmap_sl::Iterator it = _mmap_vec[category_index].upper_bound(str2);

            for(mmap_sl::Iterator i = it; i != _mmap_vec[category_index].end(); i++){

                // for(int k = 0; k < (*it).value_list.size(); k++){
                //     select_recnos.push_back((*it).value_list[k]);
                // }
                //***MISTAKE*** i is the one moving (changing in the for loop)not it ***

                for(int k = 0; k < (*i).value_list.size(); k++){
                    select_recnos.push_back((*i).value_list[k]);
                }
                
                if(debug) cout << "value list: " << (*i).value_list;

            }
            //get vector for result set
            for(int j = 0; j < select_recnos.size(); j++){
                    
                if(debug) cout<<"select_renos["<<j<<"]: "<<select_recnos[j] << endl;
                    bytes = recordFile.read(fileStream, select_recnos[j]);
                    if(debug) cout << "after read select_renos[" << j << "]: " << select_recnos[j] << endl;
                
                    insert_data.clear();
                    for(int i = 0; i < categories.size(); i++){
                        
                        insert_data.push_back(recordFile._record[i]);
                    }
        
                reorder_insert(selected_recnos, insert_data, select_recnos[j], recordFile, fileStream); 
            }
        }
        else if(op == "<="){
            mmap_sl::Iterator it = _mmap_vec[category_index].upper_bound(str2);

            for(mmap_sl::Iterator i = _mmap_vec[category_index].begin(); i != it; i++){
                for(int k = 0; k < (*i).value_list.size(); k++){
                    select_recnos.push_back((*i).value_list[k]);
                }
                
                if(debug) cout << "value list: " << (*i).value_list;
            }
            for(int j = 0; j < select_recnos.size(); j++){
                    
                if(debug) cout<<"select_renos["<<j<<"]: "<<select_recnos[j] << endl;
                    bytes = recordFile.read(fileStream, select_recnos[j]);
                    if(debug) cout << "after read select_renos[" << j << "]: " << select_recnos[j] << endl;
                
                    insert_data.clear();
                    for(int i = 0; i < categories.size(); i++){
                        
                        insert_data.push_back(recordFile._record[i]);
                    }
        
                    reorder_insert(selected_recnos, insert_data, select_recnos[j], recordFile, fileStream);
            }

        }    
        else if(op == "<"){
            mmap_sl::Iterator it = _mmap_vec[category_index].lower_bound(str2);

            for(mmap_sl::Iterator i = _mmap_vec[category_index].begin(); i != it; i++){
                for(int k = 0; k < (*i).value_list.size(); k++){
                    select_recnos.push_back((*i).value_list[k]);
                }
                
                
                if(debug) cout << "value list: " << (*i).value_list;
            }
            for(int j = 0; j < select_recnos.size(); j++){
                    
                if(debug) cout<<"select_renos["<<j<<"]: "<<select_recnos[j] << endl;
                    bytes = recordFile.read(fileStream, select_recnos[j]);
                    if(debug) cout << "after read select_renos[" << j << "]: " << select_recnos[j] << endl;
                
                    insert_data.clear();
                    for(int i = 0; i < categories.size(); i++){
                        
                        insert_data.push_back(recordFile._record[i]);
                    }
        
                reorder_insert(selected_recnos, insert_data, select_recnos[j], recordFile, fileStream);
            }
        }
        else if(op == ">="){
            mmap_sl::Iterator it = _mmap_vec[category_index].lower_bound(str2);

            for(mmap_sl::Iterator i = it; i != _mmap_vec[category_index].end(); i++){

                // for(int k = 0; k < (*it).value_list.size(); k++){
                //     select_recnos.push_back((*it).value_list[k]);
                // }
                //*** i is the one moving (changing in the for loop) not iterator it
                for(int k = 0; k < (*i).value_list.size(); k++){
                    select_recnos.push_back((*i).value_list[k]);
                }
                
                if(debug) cout << "value list: " << (*i).value_list;

            }
            //
            for(int j = 0; j < select_recnos.size(); j++){
                    
                if(debug) cout<<"select_renos["<<j<<"]: "<<select_recnos[j] << endl;
                    bytes = recordFile.read(fileStream, select_recnos[j]);
                    if(debug) cout << "after read select_renos[" << j << "]: " << select_recnos[j] << endl;
                
                    insert_data.clear();
                    for(int i = 0; i < categories.size(); i++){
                        
                        insert_data.push_back(recordFile._record[i]);
                    }
                reorder_insert(selected_recnos, insert_data, select_recnos[j], recordFile, fileStream);
            }

        }
        else {
            assert("not a valid operator");
        }

        if(debug) cout << endl << "recno vec : " << select_recnos <<endl;

        serial_number++;
        _select_vec = select_recnos;

        fileStream.close();

        return selected_recnos;
    }

    
    Queue<Token*> Table::shunting_yard(const Queue<Token*>& infix){

        bool debug = false;
        Queue<Token*> output_queue;
        Stack<Token*> operator_stack;
        int relational_precedence = 4;
        for(Queue<Token*>::Iterator it = infix.begin(); it != infix.end(); it++){

            Token* print = *it;
            if(debug)cout << "token: [" << *print << "] type: [" << (*it)->get_type() <<"]"<< endl;

            if((*it)->get_type() == TOKEN_STR){
                output_queue.push(*it);
            }
            else if((*it)->get_type() == LPAREN){
                operator_stack.push(*it);
            }
            else if((*it)->get_type() == RPAREN){
                Stack<Token*>::Iterator iter = operator_stack.begin();
                while((*iter)->get_type() != LPAREN){
                    output_queue.push(operator_stack.pop());
                    iter = operator_stack.begin();
                    if(iter == operator_stack.end()){
                        break;
                    }
                }
                Token* top = operator_stack.top();

                assert(top->get_type() == LPAREN && "top is not left paren");

                operator_stack.pop();
            }
            else if((!operator_stack.empty()) && (static_cast<Operator*>(operator_stack.top())->get_priority() >= static_cast<Operator*>(*it)->get_priority())){
                //cout <<"in precedence condition" << endl;
                while(!operator_stack.empty() && (static_cast<Operator*>(operator_stack.top())->get_priority() >= static_cast<Operator*>(*it)->get_priority())){
                    output_queue.push(operator_stack.pop());    
                }
                operator_stack.push(*it);
            }
            else {
                if(debug) cout << "get token top: " << (operator_stack.top())->get_token() << endl;
                if(debug ) cout << "get token *it: " << (*it)->get_token() << endl;
                operator_stack.push(*it);
                //assert(false && "not a valid operator");
            }
            if(debug ) cout << "output_q: ";
            if(debug ) printQueue(output_queue);
            if(debug ) cout << "operator stack: " << operator_stack << endl;
        }
        
        while(!operator_stack.empty()){
            output_queue.push(operator_stack.pop());
        }

        return output_queue;

    }

    vectorlong Table::logical_eval(vectorlong vec1, vectorlong vec2, string op){
    
        //set_union (first1, last1, first2, last2, result)
        //set_intersection (first1, last1, first2, last2, result)

        sort(vec1.begin(), vec1.end());
        sort(vec2.begin(), vec2.end());

        vectorlong evaluated;
        vectorlong::iterator it;
        if(op == "and"){
                                                                                //insert iterator to manage excess
            set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), inserter(evaluated, evaluated.end()) );       

        }
        else if(op == "or"){

            set_union(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), inserter(evaluated, evaluated.end()) );

        }
        else{
            assert("not a valid operator");
        }

        return evaluated;
    }   


    vectorlong Table::eval(string str1, string str2, string op){
        bool debug = false;
        vectorlong select_recnos;
        Relational comparison_op;
        long category_index;

        category_index = _field_map.get(str1);

        if(op == "="){
            if(debug)cout << "[category_index]  " << category_index <<endl;
            if(_mmap_vec[category_index].contains(str2)){

                select_recnos = _mmap_vec[category_index].get(str2);
            }
        }
        else if(op == ">"){
            mmap_sl::Iterator it = _mmap_vec[category_index].upper_bound(str2);

            for(mmap_sl::Iterator i = it; i != _mmap_vec[category_index].end(); i++){

                for(int k = 0; k < (*i).value_list.size(); k++){
                    select_recnos.push_back((*i).value_list[k]);
                }
            
            }
        }
        else if(op == "<="){
            mmap_sl::Iterator it = _mmap_vec[category_index].upper_bound(str2);

            for(mmap_sl::Iterator i = _mmap_vec[category_index].begin(); i != it; i++){
                for(int k = 0; k < (*i).value_list.size(); k++){
                    select_recnos.push_back((*i).value_list[k]);
                }
                
                if(debug) cout << "value list: " << (*i).value_list;
            }
        }
        else if(op == "<"){
            mmap_sl::Iterator it = _mmap_vec[category_index].lower_bound(str2);

            for(mmap_sl::Iterator i = _mmap_vec[category_index].begin(); i != it; i++){
                for(int k = 0; k < (*i).value_list.size(); k++){
                    select_recnos.push_back((*i).value_list[k]);
                }
                
                
                if(debug) cout << "value list: " << (*i).value_list;
            }
        }
        else if(op == ">="){
            mmap_sl::Iterator it = _mmap_vec[category_index].lower_bound(str2);

            for(mmap_sl::Iterator i = it; i != _mmap_vec[category_index].end(); i++){
                
                for(int k = 0; k < (*i).value_list.size(); k++){
                    select_recnos.push_back((*i).value_list[k]);
                }
                
                if(debug) cout << "value list: " << (*i).value_list;

            }

        }
        else {
            assert("not a valid operator");
        }

        return select_recnos;
    }

    vectorlong Table::rpn_eval(const Queue<Token*>& conditions){
        Stack<Token*> result_stack;
        vectorlong selected_recnos;
        bool debug = false;
        //cout<<"conditions queue in rpn eval: ";
        //printQueue(conditions);
        // cout<<endl;

        for(Queue<Token*>::Iterator it = conditions.begin(); it != conditions.end(); it++){
       
            if((*it)->get_type() == TOKEN_STR){

                result_stack.push(*it);
            }
            else {
                //else if for result set
                vectorlong result_vec;
                if(debug) cout<<"(*it)->get_type(): " <<(*it)->get_type()<<endl;
                if((*it)->get_type() == RELATIONAL)
                {                   
                    string str2 = static_cast<TokenStr*>(result_stack.pop())->get_type_tokenstr();
                    string str1 = static_cast<TokenStr*>(result_stack.pop())->get_type_tokenstr();
                    result_vec = eval(str1, str2, static_cast<Relational*>(*it)->get_operator_reltional());
                    result_stack.push(new ResultSet(result_vec));    
                }
                else if((*it)->get_type() == LOGICAL)
                {
                    //else if(static_cast<Operator*>(*it)->get_type_op() == LOGICAL)
                    vectorlong str2 = static_cast<ResultSet*>(result_stack.pop())->get_result();
                    vectorlong str1 = static_cast<ResultSet*>(result_stack.pop())->get_result();
                    result_vec = logical_eval(str1, str2, static_cast<Logical*>(*it)->get_operator_logical());
                    result_stack.push(new ResultSet(result_vec));
                }
            }


        }

        selected_recnos = static_cast<ResultSet*>(result_stack.pop())->get_result();
        if(debug) cout << "selected recnos vec: " << selected_recnos << endl;
        return selected_recnos;
    }

    //reorder the fields
    void Table::reorder_insert(Table &result_table, vectorstr insert_data, long recno, FileRecord& fileRec, fstream& fileStream) {
        bool debug = false;
        vectorstr reordered_insert_data;
        string field;
        int field_index;

        if(debug) cout << "result_table field size(): " << result_table._fields.size() << endl;
            //read data
        fileRec.read(fileStream, recno);
        for(int i = 0; i < result_table._fields.size(); i++){

            field = result_table._fields[i];
            if(debug) cout<<"field: "<<field<<endl;
            field_index = _field_map.get(field);
            if(debug)cout<<"field_index: "<<field_index<<endl;
            if(debug)cout<<"insert_data[field_index]: "<<insert_data[field_index]<<endl;
            //read data

            reordered_insert_data.push_back(fileRec._record[field_index]);

            if(debug) cout << "reordered vec: " << reordered_insert_data <<endl;
            if(debug) cout << "i is : " << i << endl;
        }

        if(debug) cout<<"insert_data: "<<insert_data<<endl;

        result_table.insert_into(reordered_insert_data);
    }


    Table Table::select(vectorstr categories, Queue<Token*> conditions){

        //shunting yard -> gets postfix to use in rpn algorithm
        //rpn_eval() -> returns selected recno vector (same logic from prev select)
        //create table using vector in this func
        //insert data into new table (same logic from prev select)
        //return that table 

        bool debug = false;
        long category_index;
        long bytes;

        // if(debug) cout << "QUEUE: " << conditions <<endl;

        string new_file_name = _table_name + "_" + to_string(serial_number);
        Table result_table(new_file_name, categories);
        
        vectorlong rpn_vec;
        vectorstr insert_data = _fields;
        FileRecord recordFile;
        fstream fileStream;

        //check if queue is empty
            //if empty, rpn_vec contains 0-total # of records
            //else, rpn_vec = rpn_eval(conditions)

        if(conditions.empty()){
            for(int i = 0; i < _recnos; i++){
                rpn_vec.push_back(i);
            }
        }
        else{
            rpn_vec = rpn_eval(conditions);
        }

        
        // if(category_index == -1){
        //     assert("field does not exist");
        // }

        open_fileRW(fileStream, _table_name.c_str());

        _select_vec.clear();
        for(int j = 0; j < rpn_vec.size(); j++){
                    
                if(debug) cout<<"select_renos["<<j<<"]: "<<rpn_vec[j] << endl;
                    bytes = recordFile.read(fileStream, rpn_vec[j]);
                    if(debug) cout << "after read select_renos[" << j << "]: " << rpn_vec[j] << endl;
                
                    insert_data.clear();

                    reorder_insert(result_table, insert_data, rpn_vec[j], recordFile, fileStream);
                    
                    if(debug) cout << "insert recno: " << rpn_vec[j] <<endl;
        }
        if(debug)cout<<"After for loop q select:"<<endl;
        serial_number++;
        _select_vec = rpn_vec;

        fileStream.close();
        if(debug)cout<<"Before return"<<endl;
        return result_table;
    }

    //debug printing
    void Table::printQueue(const Queue<Token*>& q) {
        Queue<Token*> temp(q); 
        cout <<endl;
        while (!temp.empty()) {
            Token* tok = temp.front();
            cout << "[" << tok->get_token() << "]->";

            temp.pop();
        }
        cout << "|||" << endl;
        cout << endl;
    }

    Table Table::select(vectorstr categories, const vectorstr& conditions){
        bool debug = false;
        string new_file_name = _table_name + "_" + to_string(serial_number);
        Table result_table(new_file_name, categories);

        Queue<Token*> postfix;
        Queue<Token*> condition_q;
        if(debug) cout << "condition vec: "<< conditions <<endl;

        for(int i = 0; i < conditions.size(); i++){
            string debug_type;

            if(conditions[i] == "<" || conditions[i] == "<=" || conditions[i] == ">" || conditions[i] == ">=" || conditions[i] == "="){
                Relational* tok = new Relational(conditions[i]);
                if(debug) debug_type = "operator";
                condition_q.push(tok);
                if(debug)cout << tok->get_type() << " ";
            }
            else if(conditions[i] == "("){
                LParen* tok = new LParen(conditions[i]);
                if(debug) debug_type = "lparen";
                condition_q.push(tok);
                if(debug)cout << tok->get_type() << " ";
            }
            else if(conditions[i] == ")"){
               RParen* tok = new RParen(conditions[i]);
               if(debug) debug_type = "rparen";
                condition_q.push(tok);
                if(debug)cout << tok->get_type() << " ";
            }
            else if(conditions[i] == "and"){
                Logical* tok = new Logical(conditions[i]);
                if(debug) debug_type = "and";
                condition_q.push(tok);
                if(debug)cout << tok->get_type() << " ";
            }
            else if(conditions[i] == "or"){
                Logical* tok = new Logical(conditions[i]);
                if(debug) debug_type = "or";
                condition_q.push(tok);
                if(debug)cout << tok->get_type() << " ";
            }
            else{
                TokenStr* tok = new TokenStr(conditions[i]);
                if(debug) debug_type = "token str";
                condition_q.push(tok);
                if(debug)cout << tok->get_type() << " ";
                
            }
            
            // if(debug) cout << "conditions[i]: " << conditions[i] <<endl;
            if(debug) cout << "type: " <<  debug_type <<endl;
        }
        if(debug) cout << "before shunting yard queue: ";
        if(debug) printQueue(condition_q);

        postfix = shunting_yard(condition_q);

        if(debug) cout << "shunting yard return queue: ";
        if(debug) printQueue(postfix);

        result_table = select(categories, postfix);

        return result_table;
    }

    vectorstr Table::get_fields(){

        return _fields;
    }



#endif