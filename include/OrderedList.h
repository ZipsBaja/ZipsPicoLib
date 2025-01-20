#pragma once

#include <stdint.h>

#include <ZipsLib.h>

namespace uazips
{
    /*
    * A stack-based dynamic array structure. This class is not responsible
    * for handling your pointers!
    */
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

        class Iterator
        {
        private:
            T* current_elem;
        public:
            inline Iterator(T* ptr) : current_elem(ptr)
            {
            }

            inline T& operator*() const
            {
                return *current_elem;
            }

            inline Iterator& operator++()
            {
                current_elem++;
                return *this;
            }

            inline Iterator& operator--()
            {
                current_elem--;
                return *this;
            }

            inline bool operator==(const Iterator& other) const
            {
                return current_elem == other.current_elem;
            }

            inline bool operator!=(const Iterator& other) const
            {
                return !(*this == other);
            }
        };

        inline Iterator begin()
        {
            return Iterator(array[0]);
        }

        inline Iterator end()
        {
            return Iterator(array[arr_size]);
        }

        /*
        * Allocates slots for entries.
        * @param capacity The number of entries you wish to allocate.
        * Minimum value accepted is 1.
        * The total byte count is done automatically.
        * @param empty False by default. Clears the list and sets the
        * capacity to what is specified.
        */
        inline void Malloc(size_t capacity, bool empty = false)
        {
            capacity = capacity < 1 ? 1 : capacity;
            arr_size = arr_size < capacity ? capacity : arr_size;
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
            for (size_t i = arr_size - 1; i >= 0; i--)
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
                if (array[i] == data)
                    indecies.Push(i);
            return indecies;
        }

        /*
        * Removes duplicates from the existing list.
        */
        inline void RemoveDuplicates()
        {
            if (arr_size < 2)
                return;
            
            size_t new_size = arr_size;
            for (size_t i = 0; i < new_size - 1; i++)
            {
                for (size_t j = 0; j < new_size;)
                {
                    if (array[i] == array[j])
                    {
                        for (size_t k = j; k < new_size - 1; k++)
                            array[k] = array[k + 1];
                        new_size--;
                    }
                    else
                        j++;
                }
            }

            // Resize array and move data.
            T* temp = new T[new_size];
            memcpy(temp, array, new_size * sizeof(T));

            delete[] array;
            array = temp;
            arr_size = new_size;
            arr_capacity = new_size;
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

        inline T operator[](size_t index) const
        {
            return Get(index);
        }

        inline operator T() const
        {
            return array[0];
        }

        /*
        * Removes the last entry in the list.
        * Does not change the allocated capacity.
        */
        inline const T& Pop()
        {
            const T& ret = array[arr_size];
            arr_size--;
            return ret;
        }

        /*
        * Checks the list to see if an entry exists.
        */
        inline bool Exists(T data) const
        {
            for (size_t i = 0; i < arr_size; i++)
                if (array[i] == data)
                    return true;
            return false;
        }

        /*
        * Removes all entries from the list and resizes the list.
        */
        inline void Remove(T remove)
        {
            if (!Exists(remove))
                return;
            size_t rewind = 0;
            for (size_t i = 0; i < arr_size; i++)
                if (array[i] == remove)
                    rewind++;
            T* temp = new T[arr_capacity -= rewind];
            if (!temp)
                THROW("Out of memory.");
            rewind = 0;
            for (size_t i = 0; i < arr_capacity + rewind; i++)
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

        /*
        * Removes the entry at the given position. Resizes the list.
        */
        inline void RemoveAt(size_t index)
        {
            if (arr_capacity < index)
                return;
            T* temp = new T[arr_capacity--];
            if (!temp)
                THROW("Out of memory.");
            size_t found = 0;
            for (size_t i = 0; i <= arr_capacity; i++)
            {
                if (index == i)
                {
                    found++;
                    continue;
                }
                temp[i - found] = array[i];
            }
            delete[] array;
            array = temp;
            arr_size--;
        }

        /*
        * Clears the list and keeps the capacity.
        */
        inline void Clear()
        {
            arr_size = 0;
        }

        /*
        * Calls `OrderedList::Malloc(1, true)`.
        * This function exists out of user-friendlyness.
        */
        inline void Reset()
        {
            Malloc(1, 1);
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