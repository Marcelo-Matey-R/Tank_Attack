#include <stdexcept>

#ifndef QUEUE_H
#define QUEUE_H

#pragma region Queue Class

template <typename T>

struct NodeQueue{
    T data;
    NodeQueue<T>* next;
};

template <typename T>

class Queue{
    private:
    NodeQueue<T> *head;
    NodeQueue<T> *tail;
    int length;
    void CopyFrom(const Queue<T>& other);
    public:
    Queue(){
        this->head = nullptr;
        this->tail = nullptr;
        this->length = 0;
    }
    ~Queue(){
        Clear();
    }
    Queue(const Queue<T>& other){
        this->head = nullptr;
        this->tail = nullptr;
        this->length = 0;
        CopyFrom(other);
    }
    Queue<T>& operator=(const Queue<T> &other){
        if(this == &other) return *this;
        Clear();
        CopyFrom(other);
        return *this;
    }
    void Enqueue(const T &valor);
    bool Dequeue();
    const T& Peek() const;
    bool IsEmpty() const {return head == nullptr;}
    int Size() const {return length;}
    void Clear();
};

#pragma endregion


#pragma region Queue Implementation

template <typename T>
void Queue<T>::Enqueue(const T &val){
    NodeQueue<T>* newNode = new NodeQueue<T>{val, nullptr};

    if(IsEmpty()){
        head = newNode;
        tail = newNode;
    }
    else{
        tail->next = newNode;
        tail = newNode;
    }

    length++;

};

template <typename T>
void Queue<T>::CopyFrom(const Queue<T>& other){
    NodeQueue<T>* current = other.head;

    while(current != nullptr){
        Enqueue(current->data);
        current = current->next;
    }
}

template <typename T>
bool Queue<T>::Dequeue(){
    if(IsEmpty()){
        return false; 
    }

    NodeQueue<T>* tmp = head;
    head = head->next;
    delete tmp;
    if(head == nullptr){
        tail = nullptr;
    }

    length--;

    return true;
};

template <typename T>
const T& Queue<T>::Peek() const{
    if(IsEmpty()){
        throw std::underflow_error("La cola esta vacia");
    }

    return head->data;
}

template <typename T>
void Queue<T>::Clear(){
    while(!IsEmpty()){
        Dequeue();
    }
}

#pragma endregion

#pragma region Class Priority_Queue

template <typename T, typename P = int>
struct NodePriorityQueue {
    T data;
    P priority;
    NodePriorityQueue<T, P>* next;
};

template <typename T, typename P = int>
class MiPriorityQueue {
private:
    NodePriorityQueue<T, P>* head;
    int length;

    void CopyFrom(const MiPriorityQueue<T, P>& other);

public:
    MiPriorityQueue();

    ~MiPriorityQueue();

    MiPriorityQueue(const MiPriorityQueue<T, P>& other);

    MiPriorityQueue<T, P>&
    operator=(const MiPriorityQueue<T, P>& other);

    void Enqueue(const T& value, const P& priority);

    bool Dequeue();

    const T& Peek() const;

    P PeekPriority() const;

    bool IsEmpty() const;

    int Size() const;

    void Clear();
};

#pragma endregion

#pragma region Priority_Queue Implementation

template <typename T, typename P>
MiPriorityQueue<T, P>::MiPriorityQueue(){
    head = nullptr;
    length = 0;
};
 
template <typename T, typename P>
MiPriorityQueue<T, P>::~MiPriorityQueue(){
    Clear();
};
 
template <typename T, typename P>
MiPriorityQueue<T, P>::MiPriorityQueue(const MiPriorityQueue<T, P>& other){
    head = nullptr;
    length = 0;

    CopyFrom(other);
};
 
template <typename T, typename P>
MiPriorityQueue<T, P>& MiPriorityQueue<T, P>::operator=(const MiPriorityQueue<T, P>& other){

    if (this == &other) {
        return *this;
    }

    Clear();
    CopyFrom(other);

    return *this;
};

template <typename T, typename P>
void MiPriorityQueue<T, P>::CopyFrom(const MiPriorityQueue<T, P>& other){
    if (other.head == nullptr) return;

    // Copiar el primer nodo
    head = new NodePriorityQueue<T, P>{other.head->data, other.head->priority, nullptr};

    NodePriorityQueue<T, P>* currentOther = other.head->next;
    NodePriorityQueue<T, P>* currentThis  = head;

    // Copiar el resto enlazando directamente
    while (currentOther != nullptr) {
        currentThis->next = new NodePriorityQueue<T, P>{currentOther->data, currentOther->priority, nullptr};

        currentThis  = currentThis->next;
        currentOther = currentOther->next;
    }

    length = other.length;
};

template <typename T, typename P>
void MiPriorityQueue<T, P>::Enqueue(const T& value, const P& priority){

    NodePriorityQueue<T, P>* newNode = new NodePriorityQueue<T, P>{value, priority, nullptr};

    // insertar al inicio si la prioridad es MENOR (MIN-heap: menor costo = mayor urgencia)
    if (IsEmpty() || priority < head->priority) {

        newNode->next = head;
        head = newNode;
    }
    else {

        NodePriorityQueue<T, P>* current = head;

        while(current->next != nullptr && current->next->priority <= priority){
            current = current->next;
        }

        newNode->next = current->next;
        current->next = newNode;
    }

    length++;
};

template <typename T, typename P>
bool MiPriorityQueue<T, P>::Dequeue(){

    if (IsEmpty()) {
        return false;
    }

    NodePriorityQueue<T, P>* temp = head;

    head = head->next;

    delete temp;

    length--;

    return true;
};

template <typename T, typename P>
const T& MiPriorityQueue<T, P>::Peek() const {

    if (IsEmpty()) {
        throw std::underflow_error("La cola de prioridad esta vacia");
    }

    return head->data;
};

template <typename T, typename P>
P MiPriorityQueue<T, P>::PeekPriority() const {

    if (IsEmpty()) {
        throw std::underflow_error("La cola de prioridad esta vacia");
    }

    return head->priority;
};

template <typename T, typename P>
bool MiPriorityQueue<T, P>::IsEmpty() const {
    return head == nullptr;
};

template <typename T, typename P>
int MiPriorityQueue<T, P>::Size() const {
    return length;
};

template <typename T, typename P>
void MiPriorityQueue<T, P>::Clear() {

    while (!IsEmpty()) {
        Dequeue();
    }
};

#pragma endregion

#endif