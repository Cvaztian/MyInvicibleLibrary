//
// Created by kevin on 15/06/19.
//

#ifndef MYINVENCIBLELIBRARY_LINKEDLIST_H
#define MYINVENCIBLELIBRARY_LINKEDLIST_H
//
// Created by cvaz on 22/04/19.
//

#include "Node.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
template <typename T>

class LinkedList {
public:
    LinkedList(){
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void push_front(T value){
        Node<T>* nuevoNod = new Node<T>(value);
        if(head== nullptr){
            head=nuevoNod;
            head->setNext(tail);
            tail=nuevoNod;
        } else {
            Node<T>* Tmp = head;
            head = nuevoNod;
            head->setNext(Tmp);
        }
        size+=1;
    };

    void addPos(int i, T value){
        Node<T>* nuevoNod = new Node<T>(value);
        if(i==0){  // head
            nuevoNod->setNext(head);
            head = nuevoNod;
        }else if(i==size-1){  // tail
            tail->setNext(nuevoNod);
            nuevoNod->setNext(nullptr);
        }else{  // middle
            Node<T>* temp = head;
            int j = 0;
            while(temp->getNext()!=nullptr && j+1!=i){
                temp = temp->getNext();
                j++;
            }
                nuevoNod->setNext(temp->getNext());
                temp->setNext(nuevoNod);
        }
        size++;
    }

    void push_back(T value){
        Node<T>* nuevoNod = new Node<T>(value);
        if(head==nullptr){
            head=nuevoNod;
            tail=head;
        }
        else if (head->getNext()==nullptr) {
            tail=nuevoNod;
            head->setNext(tail);
        }
        else{
            Node<T>* tmp = tail;
            tail = nuevoNod;
            tmp->setNext(tail);
        }
        size++;
    }

    Node<T>* pop_front(){
        Node<T>* returnin = head;
        if(head==tail){
            head=nullptr;
            tail=head;
        } else{
            head=head->getNext();
        }
        size--;
        if(returnin){
        return returnin;
        }else{
            return nullptr;
        }
    }

    void randomize(){
        for(int i=0; i<size; i++) {
            int pos = (int) (rand()%size);
            while(pos<0 && pos>=size){
                pos = (int) (rand()%size);
            }
            if(!getElemento(pos)){
                push_back(pop_element(pos)->getData());
            }

        }
    }

    Node<T>* pop_element(int pos){
        Node<T>* returnin = head;
        Node<T>* tmp = head;
        if(pos==0){
            return pop_front();
        }else{
            int c = 0;
            while(tmp->getNext()!= nullptr && c+1!=pos){
                tmp=tmp->getNext();
                c++;
            }
            returnin = tmp->getNext();
            if(tmp->getNext()!=nullptr){
                tmp->setNext(tmp->getNext()->getNext());
            }
        }
        size--;
        return returnin;
    }

    Node<T>* getElemento(int pos) const {
        Node<T>* tmp = head;

        while(pos>0 && tmp!= nullptr){
            tmp=tmp->getNext();
            pos--;
        }
        return tmp;
    }

    void clearList(){
        int c = 0;
        while(c < this->getSize()){
            this->pop_front();
        }
    }

    T front() const
    {
        return head->getData();
    }

    int getSize() const
    {
    return size;
    }

    void setSize(int value)
    {
    size = value;
    }

    Node<T>* getHead() const
    {
    return head;
    }

    void setHead(Node<T> *value)
    {
    head = value;
    }

    Node<T>* getTail() const
    {
    return tail;
    }

    void setTail(Node<T> *value)
    {
    tail = value;
    }

private:
    Node<T>* head;
    Node<T>* tail;
    int size;
};

#endif //MYINVENCIBLELIBRARY_LINKEDLIST_H
