#pragma once

#include <stdint.h>

#include <ZipsLib.h>

namespace uazips
{

    template<typename T>
    class OrderedList
    {
    private:
        T* array;
        size_t arr_capacity;
        size_t arr_size;

    public:
        inline OrderedList() : OrderedList(1)
        {
        }

        inline OrderedList(size_t cap) : arr_capacity(cap), arr_size(0)
        {
            array = new T[cap];
            if (!array)
                THROW("Out of memory.");
        }

        inline ~OrderedList()
        {
            delete[] array;
        }

        inline void Malloc(size_t capacity, bool empty = false)
        {
            if (empty)
            {
                delete[] array;
                array = new T[capacity];
                if (!array)
                    THROW("Out of memory.");
                return;
            }
            T* temp = new T[capacity];
            for (size_t i = 0; i < arr_size; i++)
                temp[i] = array[i];
            delete[] array;
            array = temp;
        }

        inline void Push(T data)
        {
            if (arr_capacity == arr_size)
            {
                T* temp = new T[arr_capacity++];
                if (!temp)
                    THROW("Out of memory.");
                for (int i = 0; i <= arr_capacity; i++)
                    temp[i] = array[i];
                delete[] array;
                array = temp;
            }
            array[arr_size] = data;
            arr_size++;
        }

        inline void Set(T data, size_t index)
        {
            if (arr_capacity <= index)
                Push(data);
            else
                array[index] = data;
        }

        inline T Get(size_t index) const
        {
            if (index < arr_size)
                return array[index];
            return reinterpret_cast<T>(-1);
        }

        /*
        * Returns the first index of the data that is inserted.
        * @param data The object to find the index of.
        */
        inline size_t FindFirstOf(T data) const
        {
            for (size_t i = 0; i < arr_size; i++)
            {
                if (array[i] == data)
                    return i;
            }
            return -1;
        }

        /*
        * Returns the last index of the data that is inserted.
        * @param data The object to find the index of.
        */
        inline size_t FindLastOf(T data) const
        {
            for (size_t i = arr_size; i > 0; i--)
            {
                if (array[i] == data)
                    return i;
            }
            return -1;
        }

        /*
        * Returns the indecies of the data that is inserted.
        * @param data The object to find the index of.
        */
        inline const OrderedList<size_t>& Find(T data) const
        {
            OrderedList<size_t> indecies;
            for (size_t i = 0; i < arr_size; i++)
            {
                if (array[i] == data)
                    indecies.Push(i);
            }
            return indecies;
        }

        /*
        * This function is used to return the data represented as a raw pointer array.
        * This is not safe! Data can be destroyed if list destructor is called, or
        * when copying the array, you must call `delete[]` when done using it!
        * @param create_new Choose whether to copy to a new array or use original.
        * @param by_capacity Choose to copy the entire capacity of the array instead
        * of the used entries.
        */
        inline T* ToArray(bool create_new = true, bool by_capacity = false) const
        {
            if (!create_new)
                return array;
            size_t ret_size = by_capacity ? arr_capacity : arr_size;
            T* ret = new T[ret_size];
            memcpy(ret, array, sizeof(T) * ret_size);
            return ret;
        }

        inline T operator[](size_t index)
        {
            return Get(index);
        }

        inline const T& Pop()
        {
            const T& ret = array[arr_size];
            arr_size--;
            return ret;
        }

        inline void Remove(T remove)
        {
            T* temp = new T[arr_capacity--];
            if (!temp)
                THROW("Out of memory.");
            size_t rewind = 0;
            for (int i = 0; i <= arr_capacity; i++)
            {
                if (array[i] == remove)
                {
                    rewind++;
                    continue;
                }
                temp[i - rewind] = array[i];
            }
            delete[] array;
            array = temp;
            arr_size -= rewind;
        }

        inline size_t GetCapacity() const
        {
            return arr_capacity;
        }

        inline size_t GetSize() const
        {
            return arr_size;
        }

    };

}