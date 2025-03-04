#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <cassert>

#include "node.h"

using namespace std;

//Linked List General Functions:
// template <typename T>                       //template allows for different data types
// void _print_list(node<T>* head){

//     node<T>* current = nullptr;

//     cout << "Head->";
//     for (current = head; current!= nullptr; current = current->_next)      //iterates through and prints
//     {
//         cout << "[" << current->_item << "]-> ";
//     }
//     if(current == nullptr) {
//         cout << "|||" << endl;
//     }
//     //cout << endl;

// }

template <typename T>                       //template allows for different data types
void _print_list(node<T>* head){

    node<T>* current = nullptr;

    cout << "Head->";
    for (current = head; current!= nullptr; current = current->_next)      //iterates through and prints
    {
        cout << "[" << current->_item << "]-> ";
    }
    if(current == nullptr) {
        cout << "|||" << endl;
    }
    //cout << endl;

}

// //recursive fun! :)
// template <typename T>
// void _print_list_backwards(node<T> *head);

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key){
    if (head == nullptr) {
        return nullptr;
    }
      
    for(node<T>* current = head; current != nullptr; current = current->_next) {        //if data in current ptr is equal to value of key, then returns ptr

        if(current->_item == key) {
        //    cout << "found " << current; (print for testing)
            return current;
        }

    }

    return nullptr;
}


template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this){

    node<T>* new_node = new node<T>(insert_this); //create new node
    
    (new_node->_next) = head;                      //point new node to current head

    head = new_node;                            //point head ptr to new node
    //cout <<"*head"<<*head<<"\n";              //print test
    return head;

}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this){

    if (head == nullptr || after_this == nullptr) {       //if a node is null in a list, the entire list is null (can't exist a nullptr within a list bc no address)  
        //cout << "null" << endl; (print to test)
        if(after_this == nullptr)
            //cout << "After this is null when head is not null"<<endl;
        // else {
        //     cout << "after this not null when head" << endl;
        // }
        return _insert_head(head, insert_this);
        //cout << head; (print testing)

    }

    node<T>* new_node = new node<T>(insert_this);   //create new node
    new_node->_next = after_this->_next;            //connect new node to node after after_this
    after_this->_next = new_node;                   //connect after this to new node

    //_print_list(head); (printing to test)
    return new_node;
                                
}

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this) {           

    if (head == nullptr || before_this == head || before_this == nullptr) {       //if a node is null in a list, the entire list is null (can't exist a nullptr within a list bc no address)  
        //cout << "null" << endl; (print to test)
        _insert_head(head, insert_this);
        //cout << head; (print testing)
    }

    node<T>* new_node = new node<T>(insert_this);       //create new node
    node<T>* prev_node = nullptr;                       //initialize prev_node 
    node<T>* current = nullptr;                         //initialize current node

    for(current = head; (current != nullptr) && (current != before_this); current = current -> _next) {
        prev_node = current;                                //loop through list until before_this is found
    }
    new_node->_next = before_this;                          //connect new node to before_this
    prev_node->_next = new_node;                            //connect previous node to new

    return new_node;
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* head, node<T>* prev_to_this){

    //cout << "prev_to_this: " << prev_to_this->_item;

    if (prev_to_this == nullptr || head == nullptr || prev_to_this == head) {
        //cout << "null"; (test print)
        return nullptr;
    }

    node<T>* current = nullptr;

    for(current = head; current->_next != prev_to_this; current = current->_next) {
        return current;
    }

    //cout << "prev: " << current->_item;

}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this){

    assert(delete_this != nullptr && "delete_this is null");
    if(head == nullptr) {
        cout << "list null";
        return T();
    }

    T deleteVal;
    node<T>* current = nullptr;
    node<T>* temp = nullptr;
    
    if(delete_this == head) {
        temp = head;
        deleteVal = temp->_item;
        head = head->_next;
        delete temp;
        return deleteVal;
    }

    for(current = head; current != nullptr; ) {
        current = current->_next;
        if(current->_next == delete_this) {
            temp = current->_next;
            current->_next = temp->_next;
            deleteVal = temp->_item;          //create variable to save data value in order to return after deleted node
            delete temp;

            return deleteVal;
        }
    }
    return T();
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head){

    node<T>* dest = nullptr;

    _copy_list(dest, head);

    return dest;
}

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src){

    _clear_list(dest);

    if (src == nullptr) {
        return nullptr;
    }
    node<T>* current = nullptr;

    _insert_head(dest, src->_item);

    node<T>* prev_node = dest;

    for(current = src->_next; current != nullptr; current = current->_next) {
        prev_node = _insert_after(dest, prev_node, current->_item);  //head, after, insert
    }
    //cout << "was in copy list\n";

//    cout << prev_node->_item;
    return prev_node;
}
//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head){
    //cout << "out" << endl;
    while(head != nullptr){
        _delete_node(head, head);
        if(head != nullptr){
            head = head->_next;
        }
      //  cout << "current: " << current->_item;
    }
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos){

    assert((pos >= 0) && "pos cant be negative");   //assert to check if pos is negative

    node<T>* current = head;

    for(int i = 0; i < pos; i++) {              //loops through to find pos, use int i bc pos is an int value (use index to iterate)
        current = current->_next;
    }

    assert((current != nullptr) && "position out of range");        //assert to check if the node is within range of list

   // cout << current->_item; (test print)

    return current->_item;

}

#endif
