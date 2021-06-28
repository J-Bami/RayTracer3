#ifndef MANAGER_H
#define MANAGER_H


#include "DynamicContainer.h"
/*
defines a class to register and track objects, to be added as a static member in the class.
uses pointers
*/

namespace swappy
{
    template <typename T>
    inline void swap(T& a, T& b)
    {
        const T temp = b;
        b = a;
        a = temp;
    }

    template <typename T>
    inline void swap(const T* a, const T* b)
    {
        const T* temp = b;
        b = a;
        a = temp;
    }
}

template<typename ObjectType>
class Manager: public DynamicContainer<ObjectType*>
{
typedef DynamicContainer<ObjectType*> Container;

public:
    Manager(): DynamicContainer<ObjectType*>(){}

    void register_object(ObjectType *const object)
    {
        Container::grow();
        Container::set_object(object, Container::get_size()-1);
    }

    void remove_object(ObjectType *const object)
    {
        for (size_t i = 0; i < Container::get_size(); i++)
        {
            if(object == Container::get_object(i))
            {
                //we move the object at this position to the end of the array
                swappy::swap(Container::operator[](Container::get_size() - 1), Container::operator[](i));
                break;
            }
        }
        //and shrink the array to ignore this object pointer
        Container::shrink();
    }

    inline ObjectType* operator[](const size_t index)const
    {
        return Container::operator[](index);
    }

    operator Container()const
    {
        return static_cast<Container>(*this);
    }
};


#endif