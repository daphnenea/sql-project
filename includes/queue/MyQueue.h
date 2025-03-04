#ifndef MYQUEUE_H
#define MYQUEUE_H

#include "../linked_list_functions/linkedlistfunctionTemplated.h"
#include "../token/token.h"
 
using namespace std;

template <typename T>
class Queue
{
public:
    class Iterator{
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator(){_ptr = NULL;}                          //default ctor
        Iterator(node<T>* p){
            _ptr = p;
        }                            //Point Iterator to where
                                                          //  p is pointing to
        T operator *(){

            return _ptr->_item;
        }    //dereference operator
        bool is_null(){
            if (_ptr == nullptr){
                return true;
            }
            return false;
        }         //true if _ptr is NULL
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  //true if left != right
        {

            return left._ptr != right._ptr;
        }

        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  //true if left == right
        {
            return left._ptr == right._ptr;
        }


        Iterator& operator++(){                         //member operator:++it
                                                        // or ++it = new_value
            return _ptr = _ptr->_next;
        }

        friend Iterator operator++(Iterator& it,
                                   int unused){         //friend operator: it++
       
            assert(it._ptr != nullptr && "Cannot it++ an empty iterator");
            Iterator hold(it._ptr);
            it._ptr = it._ptr->_next;
            return hold;

        }

    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Queue(){
        _front = nullptr;
        _rear = nullptr;
        _size = 0;
    }

    Queue(const Queue<T>& copyMe){
        //cout << "inside queue copy constructor" << endl;
        _front = nullptr;
        _rear = _copy_list(_front, copyMe._front);
        _size = copyMe._size;
    }

    ~Queue()
    {
        _clear_list(_front);
        _front = nullptr;
        _rear = nullptr;
        _size = 0;
    }

    Queue& operator=(const Queue<T>& RHS) //right hand side(RHS)
    {   
        //cout << "inside assignment operator" << endl;
        
        if(&RHS == this) {
            return *this;
        }
        _rear = _copy_list(_front, RHS._front);
        _size = RHS._size;
    
        return *this;
    }

    bool empty()
    {
        if(_front == nullptr) {
            assert(_rear == nullptr && "Rear should be nullptr when front is nullptr");
            return true;
        }
        return false;
    }
    T front()
    {
        return _front->_item;
    }
    T back()
    {
        return _rear->_item;
    }

    void push(T item)
    {
        const bool debug = false;
        if(debug)
        {
            if(debug)cout << "Queue push"<<endl;
            _print_list(_front);
        }
        if(_front == nullptr)
        {
            if(debug) {
                if(debug)cout << "Empty list"<<endl;
            }
            assert(_rear == nullptr && "_rear should be null when _front is null");
            _rear = _insert_head(_front, item);
        }
        else {
            _rear = _insert_after(_front, _rear, item);
        }
        _size++;

        //cout << "exit q_push" << endl;
    }
    T pop()
    {
        _size--;
        if(_front == _rear){
            _rear = nullptr;
        }
        return _delete_node(_front, _front);
    }

    Iterator begin() const                                     //Iterator to the head node
    {
        return Iterator(_front);
    }
    Iterator end() const                                       //Iterator to NULL
    {
        return Iterator(nullptr);
    }
    void print_pointers()
    {
        cout <<"_front: "<< _front << "\n";
        cout <<"_rear: "<< _rear << "\n";
    }
    int size() const { return _size; }
    
    // friend ostream& operator << (ostream& outs, const Queue<TT>& printMe)
    // {
    //     //cout << "Queue:";
    //     _print_list(printMe._front);
    //     return outs;
    // }

    friend ostream &operator<<(ostream &outs, const Queue &printMe)
    {
        //Iterator it;
        for (Queue::Iterator it = printMe.begin(); it != nullptr; it++)
        {
            outs << "[" << *it << "]->";
        }
        outs << "||| size: [" << printMe._size << "]";
        return outs;
    }

    // friend ostream &operator<<(ostream &outs, const Queue<Token*> &printMe)
    // {
    //     Iterator it;
        
    //     for (it = printMe.begin(); it != nullptr; it++)
    //     {
    //         Token* ptr = *it;
    //         outs << "[" << *ptr << "]->";
    //     }
    //     outs << "|||";
    //     return outs;
    // }
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

#endif