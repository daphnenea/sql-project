#include <iostream>
#include <iomanip>
#include "includes/sql/sql.h"
#include <string>


using namespace std;

int main() {
    cout << "\n\n"
         << endl;
    cout << "\n\n\n==========================================" << endl;
    cout << "updated\n" << endl;
    SQL sql = SQL();
    string user_input;
    
    cout << "Enter a SQL command or 'quit' to exit." << endl;
    
    while(sql.is_valid() && user_input != "quit") {
        Table cmd_tbl;
        cout << "SQL> ";
        getline(cin, user_input);
        if(user_input == "quit"){
            break;
        }
        cout << endl; 
        cmd_tbl = sql.command(user_input);
        cout << cmd_tbl << endl;
    }

    if(!(sql.is_valid())){
        cout << endl << "Please input a valid command.";
        return main();
    }
    
    cout << "\n\n\nGoodbye!" << endl;
    cout << "==========================================" << endl;
    return 0;
}
