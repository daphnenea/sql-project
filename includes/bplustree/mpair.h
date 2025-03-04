#ifndef MPAIR_H
#define MPAIR_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>

using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()){
        key = k;
        //vector<V> value_list;
    }
    
    MPair(const K& k, const V& v){
        key = k;

        value_list.push_back(v);
    }

    MPair(const K& k, const vector<V>& vlist){
        key = k;

        for(int i = 0; i < vlist.size(); i++){
            value_list.push_back(vlist(i));
        }

    }
    //--------------------------------------------------------------------------------

    friend ostream& operator <<(ostream& outs, const vector<V> value_list){

        for(int i = 0; i < value_list.size(); i++){
            outs << value_list[i] << " ";
        }

        return outs;
    }


    //You'll need to overload << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){

        outs << print_me.key << ": [" << print_me.value_list << "]";

        return outs;
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){

        if(lhs.key == rhs.key) {
            return true;
        } 
        return false;
    }

    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        if(lhs.key < rhs.key) {
            return true;
        }
        return false;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){

        if(lhs < rhs || lhs == rhs){
            return true;
        }

        return false;
    }

    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){

        if(lhs > rhs || lhs == rhs){
            return true;
        }

        return false;
    }

    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){

        if(lhs.key > rhs.key) {
            return true;
        }
        return false;
    }

    friend MPair<K, V> operator +(const MPair<K, V>& lhs, const MPair<K, V>& rhs){

        int size;
        MPair<K, V> sum = lhs;

        if(rhs.value_list.size() > lhs.value_list.size()){
            size = rhs.value_list.size();
        }
        else {
            size = lhs.value_list.size();
        }

        for(int i = 0; i < size; i++){
            sum.value_list.push_back(rhs.value_list[i]);
        }
        
        return sum;
    }
};

#endif