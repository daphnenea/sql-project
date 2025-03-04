#ifndef MAP_H
#define MAP_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include "pair.h"
#include "bplustree.h"

using namespace std;

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator(){

            _it = nullptr;
        }

        Iterator(typename map_base::Iterator it){

            _it = it;
        }

        Iterator operator ++(){
            
            return Iterator(_it++);
        }


        Iterator operator ++(int unused){

            return Iterator(++_it);
        }

        Pair<K, V> operator *(){

            return *_it;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){

            if(lhs._it == rhs._it){
                return true;
            }

            //if you are the same index of the same node return true
            return false;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            
            return !(lhs == rhs);
        }
        

    private:
        typename map_base::Iterator _it;
    };

    Map():map(true){
        
    }

//  Iterators
    Iterator begin(){
    
        return Iterator(map.begin());
    }

    Iterator end(){

        return Iterator(map.end());
    }

//  Capacity
    int size() const{
        
        return map.size();
    }

    bool empty() const{

        return map.empty();
    }

//  Element Access
    V& operator[](const K& key){

       return map.get(key).value;
    }

    V& at(const K& key){
        
        return map.get(key).value;
    }

    const V& at(const K& key) const{
       
        return map.get(key).value;
    }


//  Modifiers
    void insert(const K& k, const V& v){

        Pair<K, V> insert_pair(k,v);

        map.insert(insert_pair);
    }

    void erase(const K& key){
        // remove func
    }

    void clear(){

        map.clear_tree();
    }

    V get(const K& key){

        return map.get(key).value;
    }

//  Operations:
    Iterator find(const K& key){

        return Iterator(map.find(key));
    }

    bool contains(const Pair<K, V>& target) const{

        if(map.contains(target)){
            return true;
        }
        return false;
    }

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
    
    Iterator lower_bound(const K& key){
        //return the first element that's greater than or equal to key
        for(typename map_base::Iterator it = map.begin(); it != map.end(); it++)
        {
            if(*it >= key)
                return Iterator(it);
        }
        return Iterator();
    }

    Iterator upper_bound(const K& key){
        //return the first element that's greater than key
        for(typename map_base::Iterator it = map.begin(); it != map.end(); it++)
        {
            if(*it > key)
                return Iterator(it);
        }
        return Iterator();
    }

    Iterator equal_range(const K& key){

        return Iterator();
    }

    bool is_valid(){

        return map.is_valid();
    }

    friend ostream& operator <<(ostream& outs, const Map<K, V>& print_me){

        outs << print_me.map << endl;
        return outs;
    }


    //for parser
    void print_map_table_form (){

        string line = "           -----------------";
        
        cout << line << endl;
        cout << setw(20) << "| Keyword" << "| " << "Enum |" << endl;
        cout << line << endl;
        for(typename map_base::Iterator it = map.begin(); it != map.end(); it++){
            
            cout << setw(20) << *it << endl;

        }
    }

private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};


#endif