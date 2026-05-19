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
    Array();
    explicit Array(int capacity);
    ~Array();
    Array(const Array<T> &other);
    Array<T>& operator=(const Array<T> &other);

    void PushBack(const T& value);
    bool PopBack();
    T& operator[](int index);
    const T& operator[](int index) const;
    bool IsEmpty() const;
    int Size() const;
    int Capacity() const;
    void Clear();
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

    for(int i = 0; i < size; i++){
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

#pragma endregion

#endif