#ifndef BTREE_ARRAY_FUNC_H
#define BTREE_ARRAY_FUNC_H

#include <iostream>
#include <vector>

using namespace std;

template <class T>
T maximal(const T& a, const T& b){

    if(a > b) {
        return a;
    }
    else if(a == b) {
        cout << "equal" << endl;
    }

    return b;
}                      //return the larger of the two items

template <class T>
void swap(T& a, T& b){

}  //swap the two items

template <class T>
int index_of_maximal(T data[ ], int size){
    


    return size;
}                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[ ], int& size, T entry){

}        //insert entry into the sorted array
                                                        //data with length size

template <class T>
int first_ge(const T data[ ], int size, const T& entry){
    
    for(int i = 0; i < size; i++){
        if(data[i] >= entry){
            return i;
        }
    }
    return size;
}   //return the first element in data that is
                                                        //not less than entry                                                  

template <class T>
void attach_item(T data[ ], int& size, const T& entry){
    int prev_size = size;
    size++;

    data[prev_size] = entry;
}    //append entry to the right of data

template <class T>
void insert_item(T data[ ], int i, int& size, T entry){

   //assert(i < size && "size cannot be less than index");
    if(i == size || size == 0) {
        attach_item(data, size, entry);
    }
    else {
        //cout<<"else from insert_item" <<endl;
        size++;
        //last index of incremented array
        int last_index = size - 1;
        
        //for loop to iterate through the array and shift all of the indicies after index i, right
        for(int j = size - 1; j > i; j--) {

            //if last_index is 0 tries to access 0 -1 
            data[last_index] = data[last_index -1];

            last_index--;
        }
        //j should equal i after the loop

        //assign entry value to data at given index
        data[i] = entry;
    }


}    //insert entry at index i in data

template <class T>
void prepend_item(T data[], int& size, const T& entry){
    
    insert_item(data, 0, size, entry);
}

template <class T>
void delete_item(T data[ ], int i, int& size, T& entry){

    //(i < size && "size cannot be less than index");
    entry = data[i];
    //cout << "deleting: " << entry << endl;
    
    //for loop to iterate through the array and shift all of the indicies after index i, right
    for(int j = i; j < size; j++) {
        data[j] = data[j + 1];
    }
    size--;

}   //delete item at index i and place it in entry

template <class T>
void detach_item(T data[ ], int& size, T& entry){

    entry = data[size -1];
    
    size--;

}          //remove the last element in data and place
                                                        //it in entry

template <class T>
void merge(T data1[ ], int& size1, T data2[ ], int& size2){
    int begin_merge = size1;
    size1 = size1 + size2;
    int j = 0;
    for(int i = begin_merge; i < size1; i++) {
        data1[i] = data2[j];
        j++; 
    }
}   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& size1, T data2[ ], int& size2){

    int split = size1 /2;    //integer division
    int prev_size1 = size1;
    size2 = split;
    int arr1_size = prev_size1 - split; 

    for(int i = 0; i < size2; i++) {
//         cout << "data2[i]: " << data2[i] <<endl;
// cout << "data1[]: " << data1[end_arr1] << endl;

        data2[i] = data1[arr1_size];
        arr1_size++;
    }

    size1 = prev_size1 - split; 


}   //move n/2 elements from the right of data1
                                                        //and move to data2

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size){
    //force the size if they arent equal
    dest_size = src_size;            
    
    for(int i = 0; i < src_size; i++) {
        dest[i] = src[i];
    }

}              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int size, int pos = -1){

    for(int i = 0; i < size; i++){
        cout << data[i] << " ";
    }

}  //print array data

template <class T>
bool is_gt(const T data[], int size, const T& item){

    for(int i = 0; i < size; i++) {
        if(item < data[i]){
            return false;
        }
    }
    return true;
}
       //item > all data[i]

template <class T>
bool is_le(const T data[], int size, const T& item){

    for(int i = 0; i < size; i++) {
        if(item > data[i]){
            return false;
        }
    }
    return true;
}       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list){

    for(int i = 0; i < list.size(); i++){
        outs << list[i] << " ";
    }

    return outs;
} //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){
    
    list.push_back(addme);

    return list;
} 

//list.push_back addme

#endif