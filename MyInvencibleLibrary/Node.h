//
// Created by kevin on 15/06/19.
//

#ifndef MYINVENCIBLELIBRARY_NODE_H
#define MYINVENCIBLELIBRARY_NODE_H

using namespace std;
template <typename T>

class Node {
public:
    Node(){
        data= nullptr;
        next= nullptr;
    };

    /**
     * Contructor
     * @param valor Valor del nodo
     * */
    Node(T valor){
        data=valor;
        next= nullptr;
    };

    T getData() const {
        return data;
    }

    void setData(T data) {
        Node::data = data;
    }

    Node<T> *getNext() const {
        return next;
    }

    void setNext(Node<T> *next) {
        Node::next = next;
    }


private:
    T data;  /** Dato del nodo*/
    Node<T>* next;  /** Puntero al siguiente nodo */

};


#endif //MYINVENCIBLELIBRARY_NODE_H
