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
        void pre_encode(Node *root, string str, unordered_map<char, string> &huffmanCode) {
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
                cout<<pair.second<<endl;
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
            string test = txtFormatting(text, root);
            buildTree(test.substr(test.find("#")+1, test.rfind("#")-test.find("#")-1), test.substr(test.rfind("#")+1, test.length()));
        }


/**
 * Funcion que realiza todo el proceso de decodificacion
 * @param text string que se desea decodificar
 * @param huffmanCode map de codigos y char respectivos
 * @param root nodo raiz mediante el cual se obtiene el arbol
 * @return string decodificado
 */
        string decode(string text,unordered_map<char, string> huffmanCode) {    //Recorre el mapa para decodificar el string
            //string decodificado final
            string resultado = "";
            //letra actual encontrada en text
            string current_letter = "";
            //Codigo con el que se busca la letra en el mapa
            char key_fndr = '\0';
            //Contador de truncado a text
            int counter;
            while(text!=""){
                cout<<key_fndr<<endl;
                if(huffmanCode.find(key_fndr)!=huffmanCode.end()){
                    current_letter = huffmanCode.find(key_fndr)->second;
                    resultado+=current_letter;
                    text.erase(0, counter);
                    key_fndr = '\0';
                    counter = 0;
                }
                else{
                    counter++;
                    string tmp = text.substr(0, counter);
                    key_fndr = *tmp.c_str();
                }
            }
            cout<<resultado<<endl;
            return resultado;

        }

        //Se van a escribir las listas en preorden y orden, strings
        //Con el siguiente formato: $MENSAJE_CODIFICADO#ARBOL_PREORDEN#ARBOL_INORDEN$
        //La escritura de los nodos raices tiene el siguiente formato: !_%frq
        //La escritura de los nodos hojas tienen el siguiente formato: !char%frq
        string preorderTree(Node* raiz){
            if(raiz != nullptr){
                //String final armado recursivamente
                string result = "";
                //If found a leaf node
                if(!raiz->left && !raiz->right){
                    string tmp = "!";
                    tmp.push_back(raiz->ch);
                    tmp += "%";
                    tmp += "0";
                    result = tmp;
                    return result;
                }
                else{
                    //Anade la raiz
                    string tmp = "!_%";
                    tmp += to_string(raiz->freq);
                    result += tmp;
                    //Anade el hijo izquierdo en preordden
                    result += preorderTree(raiz->left);
                    //Anade el hijo derecho en preorden
                    result += preorderTree(raiz->right);
                }
                return result;
            }
            else{
                cout<<"Arbol nullptr"<<endl;
                return "";
            }
        }

        string inorderTree(Node* raiz){
            if(raiz != nullptr){
                //String final armado recursivamente
                string result = "";
                //If found a leaf node
                if(!raiz->left && !raiz->right){
                    string tmp = "!";
                    tmp.push_back(raiz->ch);
                    tmp += "%";
                    tmp += "0";
                    result = tmp;
                    return result;
                }
                else{
                    result += inorderTree(raiz->left);
                    string tmp = "!_%";
                    tmp += to_string(raiz->freq);
                    result += tmp;
                    result += inorderTree(raiz->right);
                }
                return result;
            }
            else{
                cout<<"Arbol nullptr"<<endl;
                return "";
            }
        }

        string txtFormatting(string text_code, Node* raiz) {
            if (text_code != "" && raiz != nullptr) {
                string result = "$";
                result += (text_code + "#");
                result += preorderTree(raiz);
                result += "#";
                result += inorderTree(raiz);
                cout<<result<<endl;
                return result;
            }
            else{
                cout<<"Error, no hay mensaje que codificar"<<endl;
            }
      }

        Node* buildTree(string preordr, string inordr){
            if(preordr != "" && inordr != ""){
                //Takes first element of preorder as root
                char charac = preordr[1];       //!0,char1
                preordr.erase(0,3);     //freq1!char%freq2
                int freq = stoi(preordr.substr(0, preordr.find("!")));      //desde inicio hasta llegar a siguiente nodo
                Node *root = getNode(charac, freq, nullptr, nullptr);       //crea un nodo con el char y freq
                preordr.erase(0,preordr.find("!"));     //elimina totalmente los datos del primer nodo en preorder
                if(preordr != "") {
                    //Finds the position of the root element in inorder
                    string tmp_root = "!";
                    tmp_root.push_back(charac);
                    tmp_root += ("%" + to_string(freq));
                    int pos_inordr = inordr.find(
                            tmp_root);     //Guarda el valor de la posicion donde esta el nodo raiz en inorder
                    //Left subtree
                    string prev_2_root = inordr.substr(0, pos_inordr);      //Obtiene el string de la lista inorder antes de la raiz
                    int size_of_prev = prev_2_root.length() - prev_2_root.rfind("!");       //Obtiene el tamano que tiene el ultimo elemento de inorder, el mismo ultimo delemento para preorder
                    string tmp_preordr = preordr.substr(0, preordr.find(prev_2_root.substr(prev_2_root.rfind("!"), size_of_prev))+size_of_prev);  //tmp va desde el comienzo hasta el ultimo elemento de inorder antes de la raiz actual
                    string tmp_inordr = inordr.substr(0, pos_inordr);       //Lista de nodos inorder antes de raiz
                    root->left = buildTree(tmp_preordr, tmp_inordr);
                    //Right subtree
                    preordr.erase(0, tmp_preordr.length());  //Borra los elementos innecesarios
                    inordr.erase(0, inordr.find(tmp_root)+tmp_root.length());       //Borra todos los elementos de la raiz para atras
                    root->right = buildTree(preordr, inordr);
                }
                return root;
                //
            }
            else{
                return nullptr;
            }
        }
    }Compressor;



#endif //HUFFMAN_HUFFMAN_H
