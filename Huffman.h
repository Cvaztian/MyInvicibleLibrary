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
 * y descompresion de strings. Para usarla
 */
    class Huffman {
    public:

// A Tree node
        struct Node {
            char ch;
            int freq;
            Node *left, *right;
        };

// Function to allocate a new tree node
        Node *getNode(char ch, int freq, Node *left, Node *right) {
            Node *node = new Node();

            node->ch = ch;
            node->freq = freq;
            node->left = left;
            node->right = right;

            return node;
        }

// Comparison object to be used to order the heap
        struct comp {
            bool operator()(Node *l, Node *r) {
                // highest priority item has lowest frequency
                return l->freq > r->freq;
            }
        };

//Atributos
        unordered_map<char, string> decoding_map;
        string encoded_string;
        string decoded_string;
        Node *root;

// traverse the Huffman Tree and store Huffman Codes
// in a map.
        void simple_encode(Node *root, string str,
                    unordered_map<char, string> &huffmanCode) {
            if (root == nullptr)
                return;

            // found a leaf node
            if (!root->left && !root->right) {
                huffmanCode[root->ch] = str;
            }

            simple_encode(root->left, str + "0", huffmanCode);
            simple_encode(root->right, str + "1", huffmanCode);
        }

// traverse the Huffman Tree and simple_decode the encoded string
        char simple_decode(Node *root, int &index, string str) {
            if (root == nullptr) {
                return NULL;
            }

            // found a leaf node
            if (!root->left && !root->right) {
                cout << root->ch;
                return root->ch;
            }

            index++;

            if (str[index] == '0')
                simple_decode(root->left, index, str);
            else
                simple_decode(root->right, index, str);
        }

// Builds Huffman Tree and decode given input text
        void buildHuffmanTree(string text) {
            // count frequency of appearance of each character
            // and store it in a map
            unordered_map<char, int> freq;
            for (char ch: text) {
                freq[ch]++;
            }

            // Create a priority queue to store live nodes of
            // Huffman tree;
            priority_queue<Node *, vector<Node *>, comp> pq;

            // Create a leaf node for each character and add it
            // to the priority queue.
            for (auto pair: freq) {
                pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
            }

            // do till there is more than one node in the queue
            while (pq.size() != 1) {
                // Remove the two nodes of highest priority
                // (lowest frequency) from the queue
                Node *left = pq.top();
                pq.pop();
                Node *right = pq.top();
                pq.pop();

                // Create a new internal node with these two nodes
                // as children and with frequency equal to the sum
                // of the two nodes' frequencies. Add the new node
                // to the priority queue.
                int sum = left->freq + right->freq;
                pq.push(getNode('\0', sum, left, right));
            }

            // root stores pointer to root of Huffman Tree
            Node *root = pq.top();

            // traverse the Huffman Tree and store Huffman Codes
            // in a map. Also prints them
            unordered_map<char, string> huffmanCode;
            simple_encode(root, "", huffmanCode);

            cout << "Huffman Codes are :\n" << '\n';
            for (auto pair: huffmanCode) {
                cout << pair.first << " " << pair.second << '\n';
            }

            cout << "\nOriginal string was :\n" << text << '\n';

            // print encoded string
            string str = "";
            for (char ch: text) {
                str += huffmanCode[ch];
            }

            cout << "\nEncoded string is :\n" << str << '\n';

            // traverse the Huffman Tree again and this time
            // decode the encoded string
            int index = -1;
            cout << "\nDecoded string is: \n";
            while (index < (int) str.size() - 2) {
                simple_decode(root, index, str);
            }
            this->encoded_string = str;
            this->decoded_string = text;
            this->decoding_map = huffmanCode;

        }

        void encode(string text){
            // count frequency of appearance of each character
            // and store it in a map
            unordered_map<char, int> freq;
            for (char ch: text) {
                freq[ch]++;
            }

            // Create a priority queue to store live nodes of
            // Huffman tree;
            priority_queue<Node *, vector<Node *>, comp> pq;

            // Create a leaf node for each character and add it
            // to the priority queue.
            for (auto pair: freq) {
                pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
            }

            // do till there is more than one node in the queue
            while (pq.size() != 1) {
                // Remove the two nodes of highest priority
                // (lowest frequency) from the queue
                Node *left = pq.top();
                pq.pop();
                Node *right = pq.top();
                pq.pop();

                // Create a new internal node with these two nodes
                // as children and with frequency equal to the sum
                // of the two nodes' frequencies. Add the new node
                // to the priority queue.
                int sum = left->freq + right->freq;
                pq.push(getNode('\0', sum, left, right));
            }

            // root stores pointer to root of Huffman Tree
            Node *root = pq.top();

            // traverse the Huffman Tree and store Huffman Codes
            // in a map. Also prints them
            unordered_map<char, string> huffmanCode;
            simple_encode(root, "", huffmanCode);

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

        string decode(string text,unordered_map<char, string> huffmanCode, Node *root){
            // traverse the Huffman Tree again and this time
            // decode the encoded string
            string str = "";
            int index = -1;
            while (index < (int) text.size() - 2) {
                str+=simple_decode(root, index, text);
            }
            cout << "\nDecoded string is: " <<;
        }

    }Compressor;

#endif //HUFFMAN_HUFFMAN_H
