#ifndef LIST_H
#define LIST_H

#include "DynamicContainer.h"
#include <iostream>
#include <cassert>

/*
defines a collection of items that can be indexed.
A number of methods can be applied to this:

public:
- insert(object, index)     -- replaces item at index with new object, and grows the list, keeping order
- remove(object, index)     -- removes item at index, and shrinks the list, keeping everything in order
- push/append(object)       -- adds object to the end of the list
- pop(object)               -- removes and returns the item at the end of the list
- empty()                   -- empties the list
- replace(object, index)    -- replaces item at index with object (operator[])
- find(object)              -- finds the index of the first occurence of this item
- count(object)             -- finds the number of occurences of this item in list

*/

enum ListErrors
{
    ItemNotFound,
};

template<typename type>
class List;

template<typename type>
List<type> operator+(const List<type> &l1, const List<type> &l2);
//now as a child of DynamicContainer

template<typename ObjectType>
class List: public DynamicContainer<ObjectType>
{
typedef DynamicContainer<ObjectType> Container;

private:
    //shifts all elements along by one from the end to index
    inline void forward_shift(const size_t index)
    {
        for (size_t i = Container::get_size() - 1; i > index; i--)
        {
            Container::set_object(Container::get_object(i-1), i);
        }
    }

    //shifts all elements backward from index to end
    inline void backward_shift(const size_t index)
    {
        for (size_t i = index; i < Container::get_size()-1; i++)
        {
            Container::set_object(Container::get_object(i+1), i);
        }
    }

public:
    List(){}

    List(ObjectType *const objects_, const size_t n_objects)
    {
        for (size_t i = 0; i < n_objects; i++)
        {
            append(objects_[i]);
        }   
    }

    List(const List &other)
    {
        for (size_t i = 0; i < other.get_size(); i++)
        {
            append(other[i]);
        } 
    }

    List &operator=(const List &other)
    {
        Container::empty();
        for (size_t i = 0; i < other.get_size(); i++)
        {
            append(other.get_object(i));
        }
        return *this;
    }

    ~List(){}

    void insert(const ObjectType &object, const size_t index)
    {
        assert(index < Container::get_size());
        Container::grow();
        forward_shift(index);
        Container::set_object(object, index);
        return;
    }

    const ObjectType remove(const size_t index)
    {
        assert(index < Container::get_size());
        ObjectType out = Container::get_object(index);
        //(Container::get_object(index)).~ObjectType();
        backward_shift(index);
        Container::shrink();
        return out;
    }

    inline void push(const ObjectType item)
    {
        Container::grow();
        Container::set_object(item, Container::get_size()-1);
    }
    
    inline void append(const ObjectType item)
    {
        return push(item);
    }

    inline void empty()
    {
        Container::empty();
    }

    const ObjectType pop()
    {
        ObjectType out = Container::get_object(Container::get_size()-1);
        Container::shrink();
        return out;
    }

    const size_t find(const ObjectType object)const
    {
        for (size_t i = 0; i < Container::get_size(); i++)
        {
            if(Container::get_object(i) == object){return i;}
        }
        //could return the size_ of the container if not found.
        return Container::get_size();
    }

    const size_t count(const ObjectType object)const
    {
        size_t counter = 0;
        for (size_t i = 0; i < Container::get_size(); i++)
        {
            if(Container::get_object(i) == object){counter ++ ;}
        }
        return counter;
    }

    List operator+=(const List &other)
    {
        *this = (*this + other);
        return *this;
    }

    List get_sublist(const size_t start, const size_t end=Container::get_size(), const int step=1)
    {
        List out;
        for (size_t i = start; i < end; i+=step)
        {
            out.append(Container::get_object(i));
        }
        return out;
    }
};

template<typename ObjectType>
List<ObjectType> operator+(const List<ObjectType> &l1, const List<ObjectType> &l2)
{
    List<ObjectType> out = l1;
    for (size_t i = 0; i < l2.get_size(); i++)
    {
        out.push(l2[i]);
    }
    return out;
}

template<typename ObjectType>
bool operator==(const List<ObjectType> &l1, const List<ObjectType> &l2)
{
    if(l1.get_size() != l2.get_size()){return false;}
    for (size_t i = 0; i < l1.get_size(); i++)
    {
        if(l1[i] != l2[i]){return false;}
    }
    return true;
}

template<typename ObjectType>
std::ostream &operator<<(std::ostream& out, const List<ObjectType> &list)
{
    out << "[";
    if(!list.get_size()){out << "]"; return out;}
    if(list.get_size() > 1)
    {
        for (size_t i = 0; i < list.get_size()-1; i++)
        {
            out << list[i] << ", ";
        }
    }
    out << list[list.get_size()-1] << "]";
    return out;
}



#endif