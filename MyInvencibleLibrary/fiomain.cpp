// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  ofstream myfile;
  myfile.open ("example.txt");
  myfile << "Writing this to a filesss.\n";  // Esto crea un nuevo archivo y reescribe todo lo que hay en el
  myfile.close();
  return 0;
}
