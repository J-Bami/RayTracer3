#ifndef DYNAMIC_CONTAINER_H
#define DYNAMIC_CONTAINER_H

#include <cstring>
#include <cassert>

/*
A growing and shrinking container, like std::vector 
*/

template<typename ObjectType>
class DynamicContainer
{
private:
    size_t capacity;
    size_t size;
    ObjectType *objects;

    inline void reallocate()
    {
        if (!capacity)
        {
            delete[] objects;
            objects = nullptr;
        }
        else
        {
            ObjectType* ptr = new ObjectType[capacity];
            for (size_t i = 0; i < size; i++)
            {
                ptr[i] = objects[i];
            }
            delete[] objects;
            objects = ptr;
        }
    }

protected:

    static constexpr float growth_factor = 2;
    static constexpr float shrink_factor = 0.5;
    static constexpr size_t default_capacity = 0; // I hope this doesn't break anything.

    inline void grow()
    {
        if(size >= capacity)
        {
            capacity = capacity? size_t(capacity*growth_factor) : 2;
            reallocate();
        }
        size++;
    }

    inline void shrink()
    {
        size--;
        if((size < capacity*shrink_factor) && (capacity > default_capacity))
        {
            const size_t value = size_t(capacity*shrink_factor + 1);
            capacity = default_capacity > value ? default_capacity: value;
            reallocate();
        }
    }

    inline void copy_data(const ObjectType* const source, const size_t length)
    {
        for (size_t i = 0; i < length; i++)
        {
            objects[i] = source[i];
        }
    }

public:
    inline const ObjectType* get_objects()const{return objects;}

    DynamicContainer():
    capacity(default_capacity),
    size(0),
    objects(capacity ? new ObjectType [capacity]: nullptr)
    {}

    DynamicContainer(ObjectType *const objects_, const size_t n_objects):
    capacity(default_capacity > n_objects? default_capacity: n_objects),
    size(n_objects),
    objects(capacity ? new ObjectType[capacity] : nullptr)
    {
        copy_data(objects, n_objects);
    }

    DynamicContainer(const DynamicContainer &other):
    capacity(other.capacity),
    size(other.size),
    objects(capacity ? new ObjectType[capacity] : nullptr)
    {
        copy_data(other.get_objects(), other.size);
    }

    DynamicContainer &operator=(const DynamicContainer &other)
    {        
        size = other.size;

        if(capacity!=other.capacity)
        {
            capacity = other.capacity;
            delete[] objects;
            objects = new ObjectType [capacity];
        }
        copy_data(other.get_objects(), other.size);
        return *this;
    }

    ~DynamicContainer()
    {
        delete[] objects;
    }

    inline const size_t &get_size()const{return size;}

    inline const size_t &get_capacity()const{return capacity;}

    inline void shrink_to_fit()
    {
        if(capacity == size){return;}
        capacity = size;
        reallocate();
    }

    inline void empty()
    {
        if(capacity == default_capacity && !size){return;}
        size = 0;
        capacity = default_capacity;
        reallocate();
    }

    inline void reserve(const size_t new_capacity)
    {
        if (capacity > new_capacity) { return; }
        capacity = new_capacity;
        reallocate();
    }


    inline const ObjectType &get_object(const size_t index)const
    {
        assert(index < size);
        return objects[index];
    }

    inline void set_object(const ObjectType &object, const size_t index)
    {
        assert(index < size);
        objects[index] = object;
    }

    inline ObjectType &operator[](const size_t index)const
    {
        assert(index < size);
        return objects[index];
    }

    inline operator bool()const{return size;}

    inline operator ObjectType*()
    {
        return objects;
    }
};


//pointer specialisation.
template<typename ObjectType>
class DynamicContainer<ObjectType*>
{
private:
    size_t capacity;
    size_t size;
    const ObjectType **objects; //we never want to modify the actual object, just the ptrs

    inline void reallocate()
    {
        if (!capacity)
        {
            delete[] objects;
            objects = nullptr;
        }
        else
        {
            const ObjectType** ptr = new const ObjectType *[capacity];
            for (size_t i = 0; i < size; i++)
            {
                ptr[i] = objects[i];
            }
            delete[] objects;
            objects = ptr;
        }
    }

protected:

    static constexpr float growth_factor = 2;
    static constexpr float shrink_factor = 0.5;
    static constexpr size_t default_capacity = 0; // I hope this doesn't break anything.

    inline void grow()
    {
        if(size >= capacity)
        {
            capacity = capacity? size_t(capacity*growth_factor) : 2;
            reallocate();
        }
        size++;
    }

    inline void shrink()
    {
        size--;
        if((size < capacity*shrink_factor) && (capacity > default_capacity))
        {
            const size_t value = size_t(capacity*shrink_factor + 1);
            capacity = default_capacity > value ? default_capacity: value;
            reallocate();
        }
    }

    inline void copy_data(const ObjectType *const *const source, const size_t length)
    {
        for (size_t i = 0; i < length; i++)
        {
            objects[i] = source[i];
        }
    }

public:
    inline const ObjectType** get_objects()const{return objects;}

    DynamicContainer():
    capacity(default_capacity),
    size(0),
    objects(capacity ? new const ObjectType* [capacity]: nullptr)
    {}

    DynamicContainer(const ObjectType *const *const objects_, const size_t n_objects):
    capacity(default_capacity > n_objects? default_capacity: n_objects),
    size(n_objects),
    objects(capacity ? new const ObjectType*[capacity] : nullptr)
    {
        copy_data(objects, n_objects);
    }

    DynamicContainer(const DynamicContainer &other):
    capacity(other.capacity),
    size(other.size),
    objects(capacity ? new const ObjectType*[capacity] : nullptr)
    {
        copy_data(other.get_objects(), other.size);
    }

    DynamicContainer &operator=(const DynamicContainer &other)
    {        
        size = other.size;

        if(capacity!=other.capacity)
        {
            capacity = other.capacity;
            delete[] objects;
            objects = new const ObjectType* [capacity];
        }
        copy_data(other.get_objects(), other.size);
        return *this;
    }

    ~DynamicContainer()
    {
        delete[] objects;
    }

    inline const size_t &get_size()const{return size;}

    inline const size_t &get_capacity()const{return capacity;}

    inline void shrink_to_fit()
    {
        if(capacity == size){return;}
        capacity = size;
        reallocate();
    }

    inline void empty()
    {
        if(capacity == default_capacity && !size){return;}
        size = 0;
        capacity = default_capacity;
        reallocate();
    }

    inline void reserve(const size_t new_capacity)
    {
        if (capacity > new_capacity) { return; }
        capacity = new_capacity;
        reallocate();
    }

    //keep an eye on this
    inline const ObjectType *const get_object(const size_t index)const
    {
        assert(index < size);
        return objects[index];
    }

    inline void set_object(const ObjectType *const object, const size_t index)
    {
        assert(index < size);
        objects[index] = object;
    }

    //and this
    inline const ObjectType *operator[](const size_t index)const
    {
        assert(index < size);
        return objects[index];
    }

    inline operator bool()const{return size;}

    inline operator const ObjectType**()
    {
        return objects;
    }

    inline operator ObjectType**()
    {
        return objects;
    }
};

#endif
