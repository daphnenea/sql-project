#ifndef MULTIMAP_H
#define MULTIMAP_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include "mpair.h"
#include "btree_array_funcs.h"
#include "bplustree.h"

using namespace std;

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(){
            _it = nullptr;
        }

        Iterator(typename map_base::Iterator it){
            _it = it;
        }

        Iterator operator ++(int unused){

            return Iterator(_it++);
        }

        Iterator operator ++(){

            return Iterator(++_it);
        }

        MPair<K, V> operator *(){
            return *_it;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
        
            if(lhs._it == rhs._it){
                return true;
            }

            return false;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){

            return !(lhs == rhs);
        }



    private:
        typename map_base::Iterator _it;
    };

    MMap() : mmap(true){

    }

    //multimap function to print mmap as a table
    void print_lookup(){
        
        //string line = "             -----------------";
        
        //cout << line << endl;
        //cout << setw(20) << "| Type " << "| " << "Keyword |" << endl;
        //cout << line << endl;
        if(!mmap.empty())
        {
            //cout<<"entered multimap print lookup loop: "<<endl;
            for(Iterator it = mmap.begin(); it != mmap.end(); it++){
            
                cout << setw(25) <<*it << endl;

            }
            cout << endl;
        }

    
    }   


//  Iterators
    Iterator begin(){

        return Iterator(mmap.begin());
    }

    Iterator end(){

        return Iterator(mmap.end());
    }

//  Capacity
    int size() const{
        
        return mmap.size();
    }

    bool empty() const{

        return mmap.empty();
    }

//  Element Access
    const vector<V>& operator[](const K& key) const{
    
        return mmap.get(key).value_list;

    }

    vector<V>& operator[](const K& key){

        return mmap.get(key).value_list;
    }

//  Modifiers
    void insert(const K& k, const V& v){

        MPair<K, V> insert_pair(k,v);

        mmap.insert(insert_pair);
    }

    void erase(const K& key){
        //call remove func

    }

    void clear(){

        mmap.clear_tree();
    }

//  Operations:
    bool contains(const K& key) const {

        if(mmap.contains(key)){
            return true;
        }
        return false;    
    }

    vector<V> &get(const K& key){

        return mmap.get(key).value_list;    
    }

    vector<V>& at(const K& key){

        return mmap.get(key).value_list;    
    }


    Iterator find(const K& key){


        return Iterator(mmap.find(key));
    }

    int count(const K& key){

        return 0;
    }
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key){

    //    }
    //    lower_bound
    //    upper_bound
    //    equal_range:

    Iterator lower_bound(const K& key){


        for(typename map_base::Iterator _it = mmap.begin(); _it != mmap.end(); _it++){

            if(*_it >= key){
                return Iterator(_it);
            }
        }
        return Iterator();

        // return mmap.lower_bound(key);
    }

    Iterator upper_bound(const K& key){

        for(typename map_base::Iterator _it = mmap.begin(); _it != mmap.end(); _it++){
            if(*_it > key){
                return Iterator(_it);
            }
        }

        return Iterator();

        // return mmap.upper_bound(key);

    }
    
    Iterator equal_range(const K& key){

        return Iterator();
    }


    bool is_valid(){

        return mmap.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){

        outs << print_me.mmap << endl;
        
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};

#endif