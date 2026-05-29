#include <stdexcept>

#ifndef MYARRAY_H
#define MYARRAY_H

#pragma region Class Array

template <typename T>

class Array{
    private:
    T* data;
    int lenght;
    int size;
    void Resize(int newCapacity);
    void CopyFrom(const Array &other);

    public:
    using iterator = T*;
    using const_iterator = const T*;
    Array();
    explicit Array(int capacity);
    ~Array();
    Array(const Array<T> &other);
    Array<T>& operator=(const Array<T> &other);

    void PushBack(const T& value);
    bool PopBack();
    bool PopFront();
    auto Front() const { return data[0]; }
    auto Back() const { return data[lenght - 1]; }
    T& operator[](int index);
    const T& operator[](int index) const;
    bool IsEmpty() const;
    int Size() const;
    int Capacity() const;
    void Clear();
    void Reverse();
    iterator Insert(iterator index, const T& value);
    iterator Insert(iterator pos, iterator beg, iterator last);
    iterator begin() const { return data; }
    iterator end() const { return data + lenght; }
};

#pragma endregion

#pragma region Constructors

template <typename T>
Array<T>::Array(){
    this->data = nullptr;
    this->lenght = 0;
    this->size = 0;
};

template <typename T>
Array<T>::Array(int capacity){
    this->lenght = 0;
    this->size = capacity;
    this->data = new T[size];
};

template <typename T>
Array<T>::Array(const Array &other){
    this->data = nullptr;
    this->lenght = 0;
    this->size = 0;

    CopyFrom(other);
};

#pragma endregion

#pragma region Destructors

template <typename T>
Array<T>::~Array(){
    Clear();
    lenght = 0;
};

#pragma endregion

#pragma region Operators

template <typename T>
Array<T>& Array<T>::operator=(const Array<T> &other){
    if(this == &other) return *this;
    Clear();
    CopyFrom(other);
    return *this;
};

template <typename T>
T& Array<T>::operator[](int index){
    if(index >= lenght || index < 0){
        throw std::out_of_range("Index out of the range");
    }
    return data[index];
};

template <typename T>
const T& Array<T>::operator[](int index)const{
    if(index >= lenght || index < 0){
        throw std::out_of_range("Index out of the range");
    }
    return data[index];
};

#pragma endregion

#pragma region Helpers
template <typename T>
void Array<T>::Resize(int newCapacity){
    T* tmp = new T[newCapacity];

    for(int i = 0; i < lenght; i++){
        tmp[i] = data[i];
    }

    delete[] data;
    data = tmp;
    size = newCapacity;
};

template <typename T>
void Array<T>::Clear(){
    delete[] data;
    data = nullptr;
    size = 0;
    lenght = 0;
};

template <typename T>
void Array<T>::CopyFrom(const Array<T> &other){
    this->size = other.size;
    this->lenght = other.lenght;
    this->data = new T[size];

    for(int i = 0; i < lenght; i++){
        data[i] = other.data[i];
    }


};

template <typename T>
void Array<T>::PushBack(const T& value){
    if(lenght == size){
        int newCapacity = (size == 0) ? 1 : 2*size;
        Resize(newCapacity);
    }

    data[lenght++] = value;
};

template <typename T>
bool Array<T>::PopBack(){
    if(IsEmpty()) return false;

    lenght--;

    if(lenght > 0 && lenght <= size/4){
        Resize(size/2);
    }

    return true;

}
template<typename T>
bool Array<T>::PopFront(){
    if(IsEmpty()) return false;

    for(int i = 1; i < lenght; i++){
        data[i-1] = data[i];
    }

    lenght--;

    if(lenght > 0 && lenght <= size/4){
        Resize(size/2);
    }

    return true;
};

template <typename T>
bool Array<T>::IsEmpty()const{
    return lenght == 0;
};

template <typename T>
int Array<T>::Size()const{
    return lenght;
}

template <typename T>
int Array<T>::Capacity()const{
    return size;
}

template <typename T>
void Array<T>::Reverse(){
    for(int i = 0; i < lenght/2; i++){
        std::swap(data[i], data[lenght - 1 - i]);
    }
}

template <typename T>
typename Array<T>::iterator Array<T>::Insert(typename Array<T>::iterator index, const T& value){
    size_t idx = index - begin();
    if(idx > (size_t)lenght){
        throw std::out_of_range("Index out of the range");
    }

    if(lenght == size){
        int newCapacity = (size == 0) ? 1 : 2*size;
        Resize(newCapacity);
        index = begin() + idx;
    }

    for(int i = lenght; i > (int)idx; i--){
        data[i] = data[i-1];
    }

    data[idx] = value;
    lenght++;

    return begin() + idx;
}

template <typename T>
typename Array<T>::iterator Array<T>::Insert(typename Array<T>::iterator pos, typename Array<T>::iterator beg, typename Array<T>::iterator last){
    size_t idx = pos - begin();
    size_t count = last - beg;

    if(pos < begin() || pos > end()){
        throw std::out_of_range("Index out of the range");
    }

    while(lenght + count > size){
        int newCapacity = (size == 0) ? 1 : 2*size;
        Resize(newCapacity);
        pos = begin() + idx;
    }

    for(int i = lenght - 1; i >= (int)idx; i--){
        data[i + count] = data[i];
    }

    for(size_t i = 0; i < count; i++){
        data[idx + i] = beg[i];
    }

    lenght += count;

    return begin() + idx;
}
#pragma endregion

#endif