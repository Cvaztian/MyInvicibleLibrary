#include <iostream>
#include "Huffman.h"

using namespace std;
int main() {
    Compressor.encode("Hola soy Cvaz y este es el codigo hufmman en su maximo esplendor");
    Compressor.decode();
    //Compressor.decode("1110110010100011111111010110110011010100000", Compressor.decoding_map);
}