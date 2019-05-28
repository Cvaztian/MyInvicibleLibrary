//
// Created by malavassi on 27/05/19.
//

#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;
/**
 * Clase que es utilizada mediante la instanciacion de un objeto estatico para realizar el proceso de compresion
 * y descompresion de strings mediante Huffman Coding
 */
    class Huffman {
    public:
/**
 * Estructura utilizada como nodo de un arbol
 */
        struct Node {
            char ch;
            int freq;
            Node *left, *right;
        };

/**
 * Funcion para colocar un nuevo nodo padre a partir de dos nodos hijos
 * @param ch char al que corresponde el nodo
 * @param freq  frecuencia con la que aparece el char en el string por decodificar
 * @param left nodo hijo izquiero
 * @param right nodo hijo derecho
 * @return nuevo nodo creado a partir de dos nodos hijos
 */
        Node *getNode(char ch, int freq, Node *left, Node *right) {
            Node *node = new Node();

            node->ch = ch;
            node->freq = freq;
            node->left = left;
            node->right = right;

            return node;
        }

/**
 * Parametro de comparacion para ordenar el heap segun frecuencia de aparicion
 */
        struct comp {
            bool operator()(Node *l, Node *r) {
                // A mayor prioridad menor frecuencia de aparicion
                return l->freq > r->freq;
            }
        };

        //Atributos
        /**
         * Map que guarda los codigos calculados correspondientes a cada uno de los char del ultimo string codificado
         */
        unordered_map<char, string> decoding_map;
        /**
         * String que guarda el ultimo string codificado
         */
        string encoded_string; //almacena el string codificado
        /**
         * String que guarda el ultimo string decodificado
         */
        string decoded_string; //almacena el string decodificado
        /**
         * Nodo raiz que guarda referencia al arbol de Huffman del ultimo string codificado
         */
        Node *root; //almacena el arbol a partir del nodo raiz

        /**
         * Recorre el arbol y almacena los codigos correspondientes en un mapa
         * @param root nodo raiz que guarda a partir de el, el resto del arbol
         * @param str string que se codifica
         * @param huffmanCode mapa que almacena los codigos con su respectivo char
         */
        void pre_encode(Node *root, string str,
                        unordered_map<char, string> &huffmanCode) {
            if (root == nullptr)
                return;

            // found a leaf node
            if (!root->left && !root->right) {
                huffmanCode[root->ch] = str;
            }

            pre_encode(root->left, str + "0", huffmanCode);
            pre_encode(root->right, str + "1", huffmanCode);
        }

/**
 * Recorre el arbol para decodificar el string deseado
 * @param root nodo raiz mediante el que se accede al arbol
 * @param index indice del string
 * @param str string por decodificar
 * @return char obtenido de la decodificacion de un segmento del string codificado
 */
        char pre_decode(Node *root, int &index, string str) {
            if (root == nullptr) {
                return NULL;
            }

            //Verifica si es nodo hoja
            if (!root->left && !root->right) {
                cout << root->ch;
                return root->ch;
            }

            index++;

            if (str[index] == '0')
                pre_decode(root->left, index, str);
            else
                pre_decode(root->right, index, str);
        }

        /**
         * Realiza el proceso de codificacion completo
         * @param text string que se desea codificar
         */
        void encode(string text){
            //Contabiliza la frecuencia de cada char y lo almacena en un map
            unordered_map<char, int> freq;
            for (char ch: text) {
                freq[ch]++;
            }

            //Crea una cola de prioridad para cada uno de los nodos del arbol
            priority_queue<Node *, vector<Node *>, comp> pq;

            //Crea un nodo hoja para cada uno de los char y lo anade a la cola de prioridad
            for (auto pair: freq) {
                pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
            }

            while (pq.size() != 1) { // mientras haya mas de un nodo en la cola de prioridad
                //Elimina los dos nodos de mayor prioridad (menor frecuencia) de la cola de prioridad
                Node *left = pq.top();
                pq.pop();
                Node *right = pq.top();
                pq.pop();

                /* Crea un nuevo nodo interno con dichos dos nodos como sus hijos y una frecuencia igual
                 * a la suma de las frecuencias de tales nodos hijos. Anade el nuevo nodo a la cola de
                 * prioridad */
                int sum = left->freq + right->freq;
                pq.push(getNode('\0', sum, left, right));
            }

            //El nodo raiz almacena una referencia al arbol
            Node *root = pq.top();

            // Recorre el arbol y guarda los codigos calculados en un map, y los imprime
            unordered_map<char, string> huffmanCode;
            pre_encode(root, "", huffmanCode);

            cout << "Huffman Codes are :\n" << '\n';
            for (auto pair: huffmanCode) {
                cout << pair.first << " " << pair.second << '\n';
            }
            string str = "";
            for (char ch: text) {
                str += huffmanCode[ch];
            }

            cout << "\nEncoded string is :\n" << str << '\n';
            this->encoded_string = str;
            this->decoded_string = text;
            this->decoding_map = huffmanCode;
        }
/**
 * Funcion que realiza todo el proceso de decodificacion
 * @param text string que se desea decodificar
 * @param huffmanCode map de codigos y char respectivos
 * @param root nodo raiz mediante el cual se obtiene el arbol
 * @return string decodificado
 */
        string decode(string text,unordered_map<char, string> huffmanCode, Node *root) {
            //Recorre el arbol para decodificar el string
            string str = "";
            int index = -1;
            while (index < (int) text.size() - 2) {
                str += pre_decode(root, index, text);
            }
            cout << "\nDecoded string is: " << endl;

        }
    }Compressor;



#endif //HUFFMAN_HUFFMAN_H
