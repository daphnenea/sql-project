#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <set>
#include <vector>
#include <string>
#include "../../includes/table/typedefs.h"
#include "../../includes/table/table.h"
#include "../../includes/stl_utils/vector_utilities.h"
#include "../../includes//bplustree/map.h"
#include "../../includes//bplustree/multimap.h"

using namespace std;

Table make_table(){
    vectorstr fields={"fname", "lname", "age"};
    
    Table t("student", fields);
    vectorstr row;
    row = {"Joe", "Gomez", "20"};
    t.insert_into(row);
    row = {"Karen", "Orozco", "21"};
    t.insert_into(row);
    row = {"Flo", "Yao", "29"};
    t.insert_into(row);
    row = {"Jack", "Yao", "19"};
    t.insert_into(row);
    row = {"Flo", "Jackson", "20"};
    t.insert_into(row);
    row = {"Flo", "Gomez", "20"};
    t.insert_into(row);
    row = {"Karen", "Jackson", "15"};
    t.insert_into(row);

    return t;
}

bool test_logical_eval(bool debug = false){

  /*************************************************************************************
   *********************************  TESTING FUNCTION:  *******************************
        vectorlong Table::logical_eval(vectorlong vec1, vectorlong vec2, string op)
  **************************************************************************************/    


  cout << "--------------------------- ENTERING LOGICAL EVAL TESTS ---------------------------" << endl;

  
  cout << "--------------------------- END OF LOGICAL EVAL TESTS ---------------------------" << endl;

  return true;
}

bool test_stub(bool debug = false)
{
  if (debug){
    cout << "testB:: test-sub() entering test_sub" << endl;
  }

  return true;
}

bool test_shunting_yard(bool debug = false){

    /***********************************************************************************
    **********************************  TESTING FUNCTION:  *****************************
              Queue<Token*> Table::shunting_yard(const Queue<Token*>& infix)
    ************************************************************************************/    

    cout << "--------------------------- ENTERING SHUNTING YARD TESTS ---------------------------" << endl;

    Queue<Token *> post;
    const string divider = "******************************************************************";
    Table t = make_table();
    cout << "here is the table: " << endl
        << t << endl;
    post = Queue<Token *>();
    post.push(new TokenStr("age"));
    post.push(new Relational("<"));
    post.push(new TokenStr("20"));
    post.push(new Logical("and"));
    post.push(new TokenStr("age"));
    post.push(new Relational(">"));
    post.push(new TokenStr("17"));
    post.push(new Logical("or"));
    post.push(new TokenStr("lname"));
    post.push(new Relational("="));
    post.push(new TokenStr("Gomez"));

    Queue<Token*> post2;
    post2 = Queue<Token *>();
    post2.push(new TokenStr("fname"));
    post2.push(new Relational("="));
    post2.push(new TokenStr("Joe"));
    post2.push(new Logical("or"));
    post2.push(new TokenStr("lname"));
    post2.push(new Relational("<"));
    post2.push(new TokenStr("Yao"));
    post2.push(new Logical("and"));
    post2.push(new TokenStr("age"));
    post2.push(new Relational(">"));
    post2.push(new TokenStr("15"));

    Queue<Token*> post3;
    post3 = Queue<Token *>();
    post3.push(new TokenStr("fname"));
    post3.push(new Relational("="));
    post3.push(new TokenStr("Joe"));
    post3.push(new Logical("or"));
    post3.push(new TokenStr("lname"));
    post3.push(new Relational("="));
    post3.push(new TokenStr("Yao"));
    post3.push(new Logical("or"));
    post3.push(new TokenStr("age"));
    post3.push(new Relational("="));
    post3.push(new TokenStr("15"));

    Queue<Token*> post4;
    post4 = Queue<Token *>();
    post4.push(new TokenStr("fname"));
    post4.push(new Relational("="));
    post4.push(new TokenStr("Joe"));
    post4.push(new Logical("and"));
    post4.push(new TokenStr("lname"));
    post4.push(new Relational("<"));
    post4.push(new TokenStr("Yao"));
    post4.push(new Logical("and"));
    post4.push(new TokenStr("age"));
    post4.push(new Relational(">"));
    post4.push(new TokenStr("15"));

    Queue<Token*> post5;
    post5 = Queue<Token *>();
    post5.push(new TokenStr("fname"));
    post5.push(new Relational("="));
    post5.push(new TokenStr("Joe"));
    post5.push(new Logical("or"));
    post5.push(new TokenStr("lname"));
    post5.push(new Relational("<"));
    post5.push(new TokenStr("Yao"));
    post5.push(new Logical("and"));
    post5.push(new TokenStr("age"));
    post5.push(new Relational(">"));
    post5.push(new TokenStr("15"));

    Queue<Token*> post6;
    post6 = Queue<Token *>();
    post6.push(new LParen("("));
    post6.push(new LParen("("));
    post6.push(new TokenStr("fname"));
    post6.push(new Relational("="));
    post6.push(new TokenStr("Joe"));
    post6.push(new RParen(")"));
    post6.push(new Logical("or"));
    post6.push(new TokenStr("lname"));
    post6.push(new Relational("<"));
    post6.push(new TokenStr("Yao"));
    post6.push(new RParen(")"));
    post6.push(new Logical("and"));
    post6.push(new LParen("("));
    post6.push(new TokenStr("age"));
    post6.push(new Relational(">"));
    post6.push(new TokenStr("15"));
    post6.push(new RParen(")"));


    Queue<Token*> post7;
    post7 = Queue<Token *>();
    post7.push(new TokenStr("age"));
    post7.push(new Relational(">="));
    post7.push(new TokenStr("20"));
    post7.push(new Logical("or"));
    post7.push(new TokenStr("lname"));
    post7.push(new Relational("="));
    post7.push(new TokenStr("Yao"));
    post7.push(new Logical("and"));
    post7.push(new TokenStr("fname"));
    post7.push(new Relational("="));
    post7.push(new TokenStr("Karen"));

    // cout << "post6: ----- ";
    // t.printQueue(post6);

    vector<Queue<Token*>> test_vec = {post, post2, post3, post4, post5, post6, post7};
    Queue<Token*> result;

    // for(int i = 0; i < test_vec.size(); i++){
    //   result = t.shunting_yard(test_vec[i]);
    //   cout << "post" << "(input_q): ";
    //   t.printQueue(test_vec[i]);
    //   cout << endl;
    //   cout << "result_q" << ": ";
    //   t.printQueue(result);
    //   cout << divider << endl;
    // }
  
  cout << "--------------------------- END OF SHUNTING YARD TESTS ---------------------------" << endl;

  return true;
}

TEST(TEST_STUB, TestStub) {

  EXPECT_EQ(0, test_logical_eval(false));
  EXPECT_EQ(1, test_stub(false));
  EXPECT_EQ(2, test_shunting_yard(false));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

