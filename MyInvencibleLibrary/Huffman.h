//
// Created by malavassi on 27/05/19.
//

#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H
#include <iostream>
#include <fstream>
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
        void encode(string text, string dir){
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
            root = pq.top();

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

            //Creates txt and writes the formatted text
            ofstream outfile (dir);
            outfile <<txtFormatting(str, root)<< std::endl;

            //cout<<decode_aux(str, root)<<endl;
        }
        
        /**
        * Funcion que, a partir del arbol huffman, crea una lista de sus elementos
        * en preorden en formato  !char%frq (_ si char es nulo, 0 si freq es nula)
        * @param Nodo raiz con la referencia al arbol
        * @return string en formato definido
        */
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

        /**
        * Funcion que, a partir del arbol huffman, crea una lista de sus elementos
        * en orden en formato  !char%frq (_ si char es nulo, 0 si freq es nula)
        * @param Nodo raiz con la referencia al arbol
        * @return string en formato definido
        */
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

        /**
        * Funcion que arregla el codigo huffman, lista preorden y lista en orden
        * @param string codigo huffman
        * @param Nodo raiz con la referencia al arbol
        * @return string en formato $CODIGO#ARBOL_PREORDEN#ARBOL_INORDEN$
        */
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

        /**
        * Funcion que reconstruye el arbol huffman a partir de las listas
        * de sus elementos en preorden y orden
        * @param string elementos en preorden
        * @param string elementos en orden
        * @return Node con la referencia al arbol reconstruido
        */
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

        /**
        * Funcion que abre el archivo con el formato huffman y llama
        * a otra funcion auxiliar para decodificarlo
        * @return string mensaje decodificado
        */
        string decode(string dir){
            ifstream huf_file;
            huf_file.open(dir);
            if(!huf_file){
                cout<<"Error con la direccion del archivo"<<endl;
            }
            else{
                string format, preor, inord;
                getline(huf_file, format);
                preor = format;
                format.erase(0, 1);
                format = format.substr(0, format.find("#"));
                cout<<"El formato es: " + format<<endl;
                //Obtiene el arbol en preorden
                preor.erase(0, preor.find("#")+1);
                inord = preor.substr(preor.find("#")+1, preor.length()-preor.rfind("#")-2);
                preor = preor.substr(0, preor.find("#"));
                //Crea el nodo raiz a partir del codigo
                Node *raiz = buildTree(preor, inord);
                return decode_aux(format, raiz);
            }
        }

        /**
        * Funcion auxiliar que decodifica el mensaje
        * @param string codigo huffman
        * @param Nodo raiz con la referencia al arbol
        * @return string mensaje decodificado
        */
        string decode_aux(string code, Node* tree){
            if(code != "" && tree != nullptr){
                string result = "";
                Node* tmp_node = tree;      //Nodo con el que se recorre el arbol
                bool execute = true;
                while(execute){
                    //Si llega a un nodo hoja, anade la letra al resultado
                    if((tmp_node->left == nullptr) && (tmp_node->right == nullptr)){
                        result += tmp_node->ch;
                        tmp_node = tree;
                        //Si anade el ultimo caracter y se acaba el codigo
                        if(code == ""){
                            execute = false;
                        }
                    }
                    //Si es un nodo raiz
                    else{
                        //Si el codigo indica 0, toma el camino izquierdo
                        if(code.substr(0,1) == "0"){
                            tmp_node = tmp_node->left;
                        }
                        //Si el codigo indica 1, toma el camino derecho
                        else{
                            tmp_node = tmp_node->right;
                        }
                        code.erase(0, 1);   //Elimina el elemento que acaba de evaluar
                    }
                }
                cout<<"El mensaje es: " + result<<endl;
                return result;
            }
            else{
                cout<<"Error, ingrese un codigo y arbol valido"<<endl;
            }
        }
    };



#endif //HUFFMAN_HUFFMAN_H
