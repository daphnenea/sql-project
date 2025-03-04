#include "sql.h"
#include "../token/token.h"

using namespace std;

SQL::SQL(){
    bool debug = false;
    ofstream tables;
    tables.open("created_tables.txt"); 
    string table_name;

    _is_valid_cmd = true;

    if (tables.is_open()) {
        if(debug) cout << "file is open" << endl;

        tables.close();
    } 
    else {
        cout << "Failed to open file" << endl;
    }
    
}   

Table SQL::command(string input_cmd){

    bool debug = false;
    Table cmd_tbl;
    Table select;
    char input_c[300];
    strcpy(input_c, input_cmd.c_str());
    Parser pars(input_c);
    mmap_ss pars_tree;

    pars_tree = pars.parse_tree();

    if(pars_tree["command"][0] == "create" || pars_tree["command"][0] == "make"){

        try{
            if(!(pars_tree.contains("table_name"))){
                throw std::runtime_error("No table name given to make table");
            }
            else if (!(pars_tree.contains("col"))){
                throw std::runtime_error("No field names given to make table");
            }
            else{
                if (debug) cout << "in make table condition else" << endl;
                cmd_tbl = Table(pars_tree["table_name"][0], pars_tree["col"]); 
            }
            //add the table into the map
            if(!(table_map.contains(pars_tree["table_name"][0]))){
                table_map.insert(pars_tree["table_name"][0], cmd_tbl);
            }

            if(debug) cout << "print table: " << cmd_tbl <<endl;
        }
        catch (const std::runtime_error& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
    else if(pars_tree["command"][0] == "insert"){

        if(debug) cout << "insert argument: " << pars_tree["values"] << endl;
        cout << "table name of pars_tree:  " << pars_tree["table_name"][0] << endl;
        cmd_tbl = Table(pars_tree["table_name"][0]); 
        cmd_tbl.insert_into(pars_tree["values"]);
        if(debug) cout << "print table: " << cmd_tbl <<endl;

    }
    else if(pars_tree["command"][0] == "select"){
        cmd_tbl = Table(pars_tree["table_name"][0]);
        if(pars_tree["fields"][0] == "*"){
            if(!(pars_tree.contains("condition"))){
                Queue<Token*> empty;
                select = cmd_tbl.select(cmd_tbl.get_fields(), empty);
            }
            else{
                select = cmd_tbl.select(cmd_tbl.get_fields(), pars_tree["condition"]);
            }
        }
        else{
            if(!(pars_tree.contains("condition"))){
                Queue<Token*> empty;
                select = cmd_tbl.select(pars_tree["fields"], empty);
            }
            else{
                select = cmd_tbl.select(pars_tree["fields"], pars_tree["condition"]);
            }
        }

            _recnos = cmd_tbl.select_recnos();
            if(debug) cout << "SELECT RECNOS: " << _recnos << endl;
            
        if(debug) cout << "print table: " << select << endl;

        return select;
    }
    else if(pars_tree["command"][0] == "drop"){

        cout << "not implemented.. " << endl;
    }
    else {
        _is_valid_cmd = false;
    }

    _recnos = cmd_tbl.select_recnos();
    if(debug) cout << "selected recnos (END OF FUNCTION): " << _recnos << endl;

    return cmd_tbl;
}

bool SQL::is_valid(){
    return _is_valid_cmd;
}

vectorlong SQL::select_recnos(){

    return _recnos;
} 