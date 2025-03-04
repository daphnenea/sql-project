#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <cassert>
#include "btree_array_funcs.h"
using namespace std;

template <class T>
class BPlusTree {

public:
    class Iterator {

    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):
                                                it(_it), key_ptr(_key_ptr){}

        T operator *(){

            return it->data[key_ptr];
        }

        Iterator operator++(){

            if(key_ptr != it->data_count -1) {
                
                key_ptr++;

            }
            else{

                key_ptr = 0;
                it = it->next; 
            }

            return Iterator(it, key_ptr);
        }

        Iterator operator++(int un_used){

            return ++(*this);
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){

            int key_ptr_1 = lhs.key_ptr;
            int key_ptr_2 = rhs.key_ptr;

            if(key_ptr_1 == key_ptr_2 && lhs.it == rhs.it){
                return true;
            }

            //if you are at the same index of the same node return true
            return false;
        }
        
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            
            return !(lhs == rhs);
        }
        
        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){

            return !it;
        }
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }
    

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false){
        child_count = 0;
        data_count = 0;
        next = nullptr;
    }

    BPlusTree(T *a, int size, bool dups = false){

        child_count = 0;
        data_count = 0;
        next = nullptr;
        T* walker = a;
        for(int i = 0; i < size; i++)
        {   
            insert(*walker);
            walker++;
        }

    }

    //big three:
    BPlusTree(const BPlusTree<T>& other){
        //cout<<"BPlusTree Copy Constructor"<<endl;
        copy_tree(other);
    }

    ~BPlusTree(){
        this->clear_tree();
    }

    BPlusTree<T>& operator =(const BPlusTree<T>& RHS){
        //cout<<"BPlusTree Assignment Operator"<<endl;
        if(this != &RHS) {
            clear_tree(); 
            copy_tree(RHS);
            dups_ok = RHS.dups_ok;
        }
        return *this;
    }

    void copy_tree(const BPlusTree<T>& other){  //copy other into this object

    //IMPLEMENT POINTERS
        BPlusTree<T>* last = nullptr;
        copy_tree(other, last);
        dups_ok = other.dups_ok;
    }

    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node){
       
        copy_array(data, other.data, data_count, other.data_count);

        child_count = 0;

        for(int i = 0; i < other.child_count; i++){
            child_count++;
            subset[i] = new BPlusTree<T>();

            //recurse through tree
            subset[i]->copy_tree(*(other.subset[i]), last_node);
            
        }
        if(last_node != nullptr && is_leaf()){
        
            last_node->next = this;
        }
        last_node = this;

    }

    void insert(const T& entry){   //insert entry into the tree

        loose_insert(entry);

        if(data_count > MAXIMUM){
            BPlusTree* new_root = new BPlusTree<T>();
            copy_array(new_root->data, data, new_root->data_count, data_count);
            copy_array(new_root->subset, subset, new_root->child_count, child_count);
            data_count = 0;
            child_count = 1;
            subset[0] = new_root;
            fix_excess(0);
        }

    }

    void remove(const T& entry){   //remove entry from the tree
    

    }

    void clear_tree(){             

        if(is_leaf()){
            data_count = 0;
        }
        else {
            for(int i = 0; i < child_count; i++){
                subset[i]->clear_tree();
                delete subset[i];
                subset[i] = nullptr;
            }
            child_count = 0;
            data_count = 0;
        }

        //set child count to zero BEFORE deleting
    }


    bool contains(const T& entry) const{ //true if entry can be found
    
        if(find_ptr(entry) != nullptr) {
            return true;
        } 
            
        return false;
    }

    T& get(const T& entry){              //return a reference to entry

        if(contains(entry)){
            return get_existing(entry);
        }
        insert(entry);
        return get_existing(entry);
    }

    const T& get(const T& entry)const{   //return a reference to entry

        return get_existing(entry);
    }

    /*
    * ---------------------------------------------------------------
    * This routing explains plainly how the BPlusTree is organized.
    * i = first_ge
    *       |      found         |    !found         |
    * ------|--------------------|-------------------|
    * leaf  |  you're done       | it's not there    |
    *       |                    |                   |
    * ------|--------------------|-------------------|
    * !leaf | subset[i+1]->get() |  subset[i]->get() |
    *       | (inner node)       |                   |
    * ------|--------------------|-------------------|
    *       |                    |                   |
    * ---------------------------------------------------------------
    *
    */

    T& get_existing(const T& entry){     //return a reference to entry

        assert(contains(entry) && "no entry found");

        T* found = find_ptr(entry);
        return *found;
    }

    Iterator find(const T& key){         //return an iterator to this key.

        int i = first_ge(data, data_count, key);

        //check if index returned by first_ge is within size
        if(i < data_count && data[i] == key) {
            
            if(is_leaf()){
                return Iterator(this, i);
            }
            else{
                return subset[i + 1]->find(key);   
            }

        }
        else if(is_leaf()) {

            return Iterator(nullptr, i);
        }
        else{

            return subset[i]->find(key);
        
        }

    }                                    //     NULL if not there.

    Iterator lower_bound(const T& key){  //return first that goes NOT BEFORE

        int i = first_ge(data, data_count, key);

        // check if index returned by first_ge is within size and the element is >= key
        if (i < data_count && data[i] >= key) {
            if (is_leaf()) {

                return Iterator(this, i);
            } 
            else {

                return subset[i]->lower_bound(key);
            }
        } 
        else if (is_leaf()) {

            return Iterator(nullptr, data_count);
        } 
        else {

            return subset[i]->lower_bound(key);
        }
    }                                     // key entry or next if does not
                                         // exist: >= entry

    Iterator upper_bound(const T& key){  //return first that goes AFTER key
    
        int i = first_ge(data, data_count, key);

        // check if index returned by first_ge is within size and the element is > key
        if (i < data_count && data[i] > key) {
            if (is_leaf()) {
                return Iterator(this, i);
            } 
            else {
                return subset[i]->lower_bound(key);
            }
        } 
        else if (is_leaf()) {
            // if leaf and key > all elements, return end iterator
            return Iterator(nullptr, data_count);
        } 
        else {
            
            return subset[i]->lower_bound(key);
        }
    }                                     //exist or not, the next entry  >entry

    void get_size(int& count) {

        if(is_leaf()){
            for(int i = 0; i < data_count; i++){
                count++;
            }
        }
        else{
            for(int j = 0; j < child_count; j++){

                count++;
                subset[j]->get_size(count);
            }
        }
    }

    void get_size(int& count) const{

        if(is_leaf()){
            for(int i = 0; i < data_count; i++){
                count++;
            }
        }
        else{
            for(int j = 0; j < child_count; j++){
                if(j < 0){
                    count++;
                }
                subset[j]->get_size(count);
            }
            subset[data_count]->get_size(count);
        }
    }

    int size() const{                    //count the number of elements

        int count = 0;
        
        get_size(count);

        return count;
    }

    int size(){                    //count the number of elements

        int count = 0;
        
        get_size(count);

        return count;
    }

    bool empty() const{                  //true if the tree is empty

        if(data_count == 0){
            return true;
        }
        return false;
    }


    void print_leaves(ostream &outs = cout) const {
        if (is_leaf()) {
           for (int i = 0; i < data_count; i++) {
                outs << "[" << data[i] << "]";
            }
            outs << endl; 
        } else {
            for (int i = 0; i < child_count; i++) {
                subset[i]->print_leaves(outs);
            }
        }
    }

    void print_tree(int level = 0, ostream &outs=cout) const{

        if(is_leaf()){
            for(int i = data_count -1; i > -1; i--){
                outs << setw(4 * level) << "" << "[" << data[i] << "]" << endl;
            }
        }
        else {
            subset[data_count]-> print_tree(level +1);
            for(int i = data_count -1; i > -1; i--){
                outs << setw(4 * level) << "" << "[" << data[i] << "]" << endl;
                subset[i]->print_tree(level +1);
            }
        }
    }

    friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me){

        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid(){

        return true;
    }

    void in_ord(string& str){

        if(is_leaf()) {
            
            for(int i = 0; i < data_count; i++){

                str += to_string(data[i]) +  "|";
            }
        }
        else{
            
            for(int j = 0; j < child_count; j++){

                if(j > 0){
                    str += to_string(data[j - 1]) +  "|";
                }

                subset[j]->in_ord(str);
                
            }
        }

    }

    string in_order(){

        string inord = "";

        in_ord(inord);

        return inord;
    }

    void pre_ord(string& str){

        if(is_leaf()) {
            
            for(int i = 0; i < data_count; i++){

                str += to_string(data[i]) +  "|";
            }
        }
        else{
            
            for(int j = 0; j < child_count; j++){

                if(j != child_count - 1){
                    str += to_string(data[j]) +  "|";
                }
            
                subset[j]->pre_ord(str);
                
            }
        }

    }

    string pre_order(){

        string preord = "";

        pre_ord(preord);

        return preord;
    }

    void post_ord(string& str){

        if(is_leaf()) {
            
            for(int i = 0; i < data_count; i++){

                str += to_string(data[i]) +  "|";
            }
        }
        else{
            for(int j = 0; j < child_count; j++){

                subset[j]->post_ord(str);
                
                if (j > 0) {
                    str += to_string(data[j - 1]) +  "|";
                }
            }
        }
    }

    string post_order() {

        string postord = "";

        post_ord(postord);

        return postord;
    }

    Iterator begin(){

        if(is_leaf()){

            return Iterator(this, 0);
        }
        else {

            return subset[0]->begin();
        }
    }

    Iterator end(){

        return Iterator(nullptr);
    }

    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;

    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;

    bool is_leaf() const{

        return child_count == 0;
    }             //true if this is a leaf node

    const T* find_ptr(const T& entry) const{
    
        int size = data_count;

        int i = first_ge(data, size, entry);

        //check if index returned by first_ge is within size
        if(i < data_count && data[i] == entry) {
            if(is_leaf())
                return &data[i];
            else
                return subset[i+1]->find_ptr(entry);
        }
        else if(is_leaf()) {

            return nullptr;
        }
        else{

            return subset[i]->find_ptr(entry);
        
        }

    }         //return a pointer to this key.
                                         // NULL if not there.


    T* find_ptr(const T& entry){ 
    
        int size = data_count;

        int i = first_ge(data, size, entry);

        //check if index returned by first_ge is within size
        if(i < data_count && data[i] == entry) {
            if(is_leaf())
                return &data[i];
            else
                return subset[i+1]->find_ptr(entry);
        }
        else if(is_leaf()) {

            return nullptr;
        }
        else{

            return subset[i]->find_ptr(entry);
        
        }

    }         //return a pointer to this key.
                                         

    //insert element functions

    void loose_insert(const T& entry){
        /*
            BT->BPT:
            now we have four cases: leaf, not leaf  / found, not found


            For fix_excess: add an if statement
        
        */

        int size = data_count;

        int i = first_ge(data, size, entry);

        bool found; 
        found = (i < data_count)  && (data[i] == entry);

        if(is_leaf()){
            if(found && !dups_ok){

                data[i] = entry;
            }
            else if(found && dups_ok){

                data[i] = data[i] + entry;
            }
            else {

                insert_item(data, i, data_count, entry);
            }
        }
        else {
            if(found){

                subset[i + 1]->loose_insert(entry);
                fix_excess(i + 1);
            }
            else {
                subset[i]->loose_insert(entry);
                fix_excess(i);
            }
        }

            /*
            IF LEAF:
            * if found:
            *     overwrite
            * 
            * if not found:
            *      insert at index i (first_ge)
            *      
            */
    
            /*
            IF NOT LEAF:
            * if found:
            *   subset[i + 1]->loose_insert(entry);
            *   fix_excess(i + 1);
            * 
            * else:
            *   //just breadcrumbs
            * 
            *   subset[i]->loose_insert(entry);
            *   fix_excess(i);
            */


    }   //allows MAXIMUM+1 data elements in
                                         //   the root


    void fix_excess(int i){
            
            
        if(subset[i]->data_count > MAXIMUM)
        {
            if(i == data_count)
            {
                subset[data_count + 1] = new BPlusTree<T>();
                child_count++; 
                split(subset[i]->data, subset[i]->data_count, subset[data_count + 1]->data, subset[data_count + 1]->data_count);
                split(subset[i]->subset, subset[i]->child_count, subset[data_count + 1]->subset, 
                subset[data_count + 1]->child_count);
                T append_item = subset[i]->data[subset[i]->data_count-1];
                attach_item(data, data_count, append_item);
                detach_item(subset[i]->data, subset[i]->data_count, append_item);
                if(subset[i + 1]->is_leaf()){
                    //prepend item to leaf
                    prepend_item(subset[i + 1]->data, subset[i + 1]->data_count, append_item);
                    //connect pointers
                    subset[i + 1]->next = subset[i]->next;
                    subset[i]->next = subset[i + 1];
                }
            }
            else { 
                insert_item(subset, (i + 1), child_count, new BPlusTree<T>());
                split(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
                split(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);
                T insert = subset[i]->data[subset[i]->data_count-1];
                insert_item(data, i, data_count, insert);
                detach_item(subset[i]->data, subset[i]->data_count, insert);
                if(subset[i + 1]->is_leaf()){
                    //prepend item to leaf
                    prepend_item(subset[i + 1]->data, subset[i + 1]->data_count, insert);
                    //connect pointers
                    subset[i + 1]->next = subset[i]->next;
                    subset[i]->next = subset[i + 1];
                }
            }

        }
    }              //fix excess in child i

    //remove element functions:
    void loose_remove(const T& entry){


    }  //allows MINIMUM-1 data elements
                                        //  in the root

    BPlusTree<T>* fix_shortage(int i){  //fix shortage in child i
    
        return &BPlusTree<T>();
    }
    // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node(){
    
        return &BPlusTree<T>();
    }

    void get_smallest(T& entry){      //entry := leftmost leaf

    }

    void get_biggest(T& entry){       //entry := rightmost leaf
    //rightmost leaf's biggest value

    }

    void remove_biggest(T& entry){    //remove the biggest child of tree->entry

    }

    void transfer_left(int i){

    }        //transfer one element LEFT from child i
    
    void transfer_right(int i){

    }       //transfer one element RIGHT from child i

    BPlusTree<T> *merge_with_next_subset(int i){ //merge subset i with  i+1
    
        return &BPlusTree<T>();
    }
};

#endif