#ifndef SET_H
#define SET_H


/*
Defines my Set Class, which is a class template for a dynamic container which
contains an unordered set of values in which there are no copies; therefore for
custom types, the == or != operator must be defined.

functions:
    - contains(value) sees if value is in set
    - add
    - remove

operators
    - subtract
    - union
    look up the rest of these
*/

#include "DynamicContainer.h"
#include <iostream>
#include <cassert>


template<typename type>
class Set: public DynamicContainer<type>
{
    typedef DynamicContainer<type> Container;

public:
    Set(){}

    ~Set(){}

    Set(const type* array, const size_t length)
    {
        for (size_t i = 0; i < length; i++)
        {
            add(array[i]);
        }
    }

    Set(const Set& other): Container(other){}

    Set& operator=(const Set& other)
    {
        Container::operator=(other);
        return *this;
    }

    bool contains(const type value)const
    {
        const size_t count = Container::get_size();
        for (size_t i = 0; i < count; i++)
        {
            if(Container::get_object(i) == value)
            {
                return true;
            }
        }
        return false;
    }

    const size_t add(const type& value) // returns the index 
    {
        //check if we have it first
        const size_t count = Container::get_size();
        for (size_t i = 0; i < count; i++)
        {
            if (Container::get_object(i) == value)
            {
                return i;
            }
        }
        Container::grow();
        const size_t index = Container::get_size() - 1;
        Container::set_object(value, index);
        return index;
    }

    void remove(const type value)
    {
        const size_t count = Container::get_size();
        for (size_t i = 0; i < count; i++)
        {
            if(Container::get_object(i) == value)
            {
                type& a = Container::operator[](i);
                type& b = Container::operator[](Container::get_size() - 1);
                const type temp = b;
                a = b;
                b = temp;
                Container::shrink();
                return;
            }
        }
    }

    const type remove_at_index(const size_t index)
    {
        assert(index < Container::get_size());
        const type out = (*this)[index];

        type& a = Container::operator[](index);
        type& b = Container::operator[](Container::get_size() - 1);
        const type temp = b;
        a = b;
        b = temp;
        Container::shrink();
        return out;
    }

    //union
    Set& operator|=(const Set& other)
    {
        const size_t count = other.get_size();
        for (size_t i = 0; i < count; i++)
        {
            add(other.get_object(i));
        }
        return *this;
    }

    //difference
    Set& operator-=(const Set& other)
    {
        const size_t count = other.get_size();
        for (size_t i = 0; i < count; i++)
        {
            const type object = other.get_object(i);
            if(contains(object))
            {
                remove(object);
            }
        }
        return *this;
    }

    //symmetric difference
    Set& operator^=(const Set& other)
    {
        const size_t count = other.get_size();
        for (size_t i = 0; i < count; i++)
        {
            const type object = other.get_object(i);
            contains(object) ? remove(object) : add(object);
        }
        return *this;
    }

    Set& operator&=(const Set& other)
    {
        const size_t count = Container::get_size();
        for (size_t i = count; i >= 0; i--)
        {
            const type object = Container::get_object(i);
            if(!other.contains(object))
            {
                remove(object);
            }
        }
        return *this;
    }
};

template<typename type>
class Set<type *>: public DynamicContainer<type*>
{
    typedef DynamicContainer<type*> Container;

public:
    Set(){}

    ~Set(){}

    Set(const type *const *const array, const size_t length)
    {
        for (size_t i = 0; i < length; i++)
        {
            add(array[i]);
        }
    }

    Set(const Set& other): Container(other){}

    Set& operator=(const Set& other)
    {
        Container::operator=(other);
        return *this;
    }

    bool contains(const type *const value)const
    {
        const size_t count = Container::get_size();
        for (size_t i = 0; i < count; i++)
        {
            if(Container::get_object(i) == value)
            {
                return true;
            }
        }
        return false;
    }

    const size_t add(const type *const value) // returns the index 
    {
        //check if we have it first
        const size_t count = Container::get_size();
        for (size_t i = 0; i < count; i++)
        {
            if (Container::get_object(i) == value)
            {
                return i;
            }
        }
        Container::grow();
        const size_t index = Container::get_size() - 1;
        Container::set_object(value, index);
        return index;
    }

    void remove(const type *const value)
    {
        const size_t count = Container::get_size();
        for (size_t i = 0; i < count; i++)
        {
            if(Container::get_object(i) == value)
            {
                const type* a = Container::operator[](i);
                const type* b = Container::operator[](Container::get_size() - 1);
                const type* temp = b;
                a = b;
                b = temp;
                Container::shrink();
                return;
            }
        }
    }

    const type* remove_at_index(const size_t index)
    {
        assert(index < Container::get_size());
        const type* out = (*this)[index];

        type* a = Container::operator[](index);
        type* b = Container::operator[](Container::get_size() - 1);
        const type* temp = b;
        a = b;
        b = temp;
        Container::shrink();
        return out;
    }

    //union
    Set& operator|=(const Set& other)
    {
        const size_t count = other.get_size();
        for (size_t i = 0; i < count; i++)
        {
            add(other.get_object(i));
        }
        return *this;
    }

    //difference
    Set& operator-=(const Set& other)
    {
        const size_t count = other.get_size();
        for (size_t i = 0; i < count; i++)
        {
            const type* object = other.get_object(i);
            if(contains(object))
            {
                remove(object);
            }
        }
        return *this;
    }

    //symmetric difference
    Set& operator^=(const Set& other)
    {
        const size_t count = other.get_size();
        for (size_t i = 0; i < count; i++)
        {
            const type* object = other.get_object(i);
            contains(object) ? remove(object) : add(object);
        }
        return *this;
    }

    Set& operator&=(const Set& other)
    {
        const size_t count = Container::get_size();
        for (size_t i = count; i >= 0; i--)
        {
            const type* object = Container::get_object(i);
            if(!other.contains(object))
            {
                remove(object);
            }
        }
        return *this;
    }
};



//union
template<typename dtype>
Set<dtype> operator|(const Set<dtype>& s1, const Set<dtype>& s2)
{
    Set<dtype> out = s1;
    const size_t count = s2.get_size();
    for (size_t i = 0; i < count; i++)
    {
        out.add(s2.get_object(i));
    }
    return out;
}

//difference
template<typename dtype>
Set<dtype> operator-(const Set<dtype>& s1, const Set<dtype>& s2)
{
    Set<dtype> out = s1;
    const size_t count = s2.get_size();
    for (size_t i = 0; i < count; i++)
    {
        out.remove(s2.get_object(i));
    }
    return out;
}

//symmetric difference
template<typename dtype>
Set<dtype> operator^(const Set<dtype>& s1, const Set<dtype>& s2)
{
    Set<dtype> out;
    const size_t count1 = s1.get_size();
    for (size_t i = 0; i < count1; i++)
    {
        const dtype object = s1.get_object(i);
        if(!s2.contains(object))
        {
            out.add(object);
        }
    }

    const size_t count2 = s2.get_size();
    for (size_t i = 0; i < count2; i++)
    {
        const dtype object = s2.get_object(i);
        if(!s1.contains(object))
        {
            out.add(object);
        }
    }
    return out;
}

//intersection
template<typename dtype>
Set<dtype> operator&(const Set<dtype>& s1, const Set<dtype>& s2)
{
    Set<dtype> out;
    const size_t count = s1.get_size();
    for (size_t i = 0; i < count; i++)
    {
        const dtype object = s1.get_object(i);
        if(s2.contains(object))
        {
            out.add(object);
        }
    }
    return out;
}

//is s1 a superset of s2?
template<typename dtype>
const bool operator>(const Set<dtype>& s1, const Set<dtype>& s2)
{
    if(s1.get_size() <= s2.get_size()){return false;}

    for (size_t i = 0; i < s2.get_size(); i++)
    {
        if(!s1.contains(s2[i])){return false;}
    }
    return true;
}

//is s1 a subset of s2?
template<typename dtype>
const bool operator<(const Set<dtype>& s1, const Set<dtype>& s2)
{
    return s2 > s1;
}

//are these sets equivalent?
template<typename dtype>
const bool operator==(const Set<dtype>& s1, const Set<dtype>& s2)
{
    if (s2.get_size() != s1.get_size){return false;}
    for (size_t i = 0; i < s1.get_size(); i++)
    {
        const dtype object = s1.get_object(i);
        if(!s2.contains(object)){return false;}
    }
    return true;
}

template<typename ObjectType>
std::ostream &operator<<(std::ostream& out, const Set<ObjectType> &set)
{
    out << "Set: {";
    if(!set.get_size()) {out << "}"; return out;}
    if(set.get_size() > 1)
    {
        for (size_t i = 0; i < set.get_size()-1; i++)
        {
            out << set[i] << ", ";
        }
    }
    out << set[set.get_size()-1] << "}";
    return out;
}

#endif