#ifndef DYNAMIC_ARRAY_HEADER
#define DYNAMIC_ARRAY_HEADER
#include <cstdlib>
#include <new>
#include <stdexcept>
#include "utilities.h"
using namespace std;

template <typename T>
struct dynamic_array
{
    T *data; // pointer to the place where holds the array
    unsigned int size;
    unsigned int capacity;
    T default_value;

    dynamic_array(unsigned int initial_capacity, T default_val)
    {
        data = (T *)malloc(sizeof(T) * initial_capacity);
        size = 0;
        default_value = default_val;

        if (data == nullptr)
        {
            capacity = 0;
            return;
        }

        capacity = initial_capacity;

        // Initialize the elements with a default constructor
        for (unsigned int i = 0; i < capacity; i++)
        {
            new (&data[i]) T();
        }
    }

    ~dynamic_array() // call the destructor
    {
        if (data != nullptr)
        {
            // Call destructor for all initialized elements
            for (unsigned int i = 0; i < size; i++) // 一个一个内存格子去free
            {
                data[i].~T();
            }
            free(data);
        }
    }

    // resize the capacity
    // if the new capacity is smaller than the current size, the currentsize will be adjusted to fit the new capacity
    bool resize(unsigned int new_capacity)
    {
        if (new_capacity < capacity)
        {
            // Destroy elements that will be removed
            for (unsigned int i = new_capacity; i < capacity; i++)
            {
                data[i].~T();
            }
        }

        // Resize using realloc
        T *new_data = (T *)realloc(data, sizeof(T) * new_capacity);
        // check if realloc success
        if (new_data == nullptr)
        {
            return false; // Failed to resize
                          // failed, so we keep the old data and capacity. cannot resize the array
        }

        // Initialize new elements if capacity increased
        if (new_capacity > capacity)
        {
            for (unsigned int i = capacity; i < new_capacity; i++)
            {
                new (&new_data[i]) T();
            }
        }

        // Update array properties
        data = new_data;
        capacity = new_capacity;
        // Update size if new capacity is smaller than the current size
        if (size > capacity)
        {
            size = capacity;
        }
        return true;
    }

    // Add element to end of array
    bool add(T value)                                      // T value add to the end of the array
    {                                                      // size >= capacity -->if the array is full如果数组满了（没有空位可以添加新元素）
        if (size >= capacity && !resize(capacity * 2 + 1)) // capacity * 2 + 1 → the strategy of resize, reduce the number of resizes
        {
            return false; // Failed to resize
        }

        data[size] = value;
        size++;
        return true;
    }

    // Get element at index
    T get(unsigned int index, T default_val)
    {
        if (index >= size)
        {
            return default_val;
        }
        return data[index];
    }

    // Set element at index
    bool set(unsigned int index, T value)
    {
        if (index >= size)
        {
            return false;
        }
        data[index] = value;
        return true;
    }

    // Overload array access operator
    T &operator[](unsigned int index)
    {
        if (index >= size)
        {
            // Return reference to default_value instead of throwing
            return default_value;
        }
        return data[index];
    }

    const T &operator[](unsigned int index) const
    {
        if (index >= size)
        {
            // Return reference to default_value instead of throwing
            return default_value;
        }
        return data[index];
    }

    bool remove(unsigned int index)
    {
        if (index >= size) // this is a bounds checking
        {
            return false; // This index is invalid
        }
        // call destructor
        data[index].~T();
        for (unsigned int i = index; i < size - 1; i++)
        {
            data[i] = data[i + 1];
        }
        size--;
        return true;
    }
};
#endif // DYNAMIC_ARRAY_HEADER