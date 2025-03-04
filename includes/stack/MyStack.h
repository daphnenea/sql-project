#ifndef MYSTACK_H
#define MYSTACK_H

#include "../linked_list_functions/linkedlistfunctionTemplated.h"
#include "../token/token.h"
#include <assert.h>

using namespace std;

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator(){_ptr = NULL;}            //default ctor
        Iterator(node<ITEM_TYPE>* p){
            _ptr = p;
        }      //Point Iterator to where
                                            //  p is pointing to
        ITEM_TYPE operator *(){
            return _ptr->_item;
        }    //dereference operator
        bool is_null(){return _ptr == NULL;}            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  //true if left != right
        {return left._ptr != right._ptr;}

        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  //true if left == right
        {
            if(!(left != right)) //left + right
                return true;
            else
                return false;
        }


        Iterator& operator++(){                        //member operator:++it
                                                       // or ++it = new_value
            return _ptr = _ptr->_next;
        }

        friend Iterator operator++(Iterator& it,
                                   int unused){        //friend operator: it++
            assert(it._ptr != nullptr && "Cannot it++ an empty iterator");
            Iterator hold(it._ptr);
            it._ptr = it._ptr->_next;
            return hold;
        }

    private:
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    };

    Stack()
    {
        _top = nullptr;
        _size = 0;
    }
    Stack(const Stack<ITEM_TYPE>& copyMe)
    {
        _top = nullptr;
        _size = copyMe.size();
    }
    ~Stack()
    {
        _top = nullptr;
        _size = 0;
    }
    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS)
    {
        if(this == &RHS){
            return *this;
        }
        _copy_list(_top, RHS._top);
        _size = RHS._size;
        
        return *this;
    }
    ITEM_TYPE top()
    {
        assert(!empty() && "top is empty");
        return _top->_item;
    }
    bool empty()
    {
        return !_top;  
    }
    void push(ITEM_TYPE item)
    {
        _size++;
        _insert_head(_top, item);
    }
    ITEM_TYPE pop()
    {
        _size--;
        return _delete_node(_top, _top);
    }
    // template<typename T>
    // friend ostream& operator<<(ostream& outs, const Stack<T>& printMe)
    // {
    //     //cout << "Stack:";
    //     _print_list(printMe._top);
    //     return outs; //Always return outs for the insertion operator (<<)
    // }

    friend ostream &operator<<(ostream &outs, const Stack &printMe)
    {
        // cout << "hey we are calling the overriden" << endl;
        //Iterator it;
        for (Stack::Iterator it = printMe.begin(); it != printMe.end(); it++)
        {
            outs << "[" << (*it) << "]->";
        }
        outs << "||| size: [" << printMe._size << "]";
        return outs;
    }


    Iterator begin() const                   //Iterator to the head node
    {
        return Iterator(_top);
    }
    Iterator end() const                     //Iterator to NULL
    {
        return Iterator(nullptr);
    }
    int size() const { return _size; }

private:
    node<ITEM_TYPE>* _top;
    int _size;
};

#endif
