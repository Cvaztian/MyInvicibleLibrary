//
// Created by malavassi on 02/06/19.
//

#ifndef MYINVICIBLELIBRARY_AUXILIAR_H
#define MYINVICIBLELIBRARY_AUXILIAR_H
#include <iostream>;
#include <fstream>
#include <cstring>
#include <vector>
#include<algorithm>
using namespace std;
class auxiliar {
public:
// Chunks a file by breaking it up into chunks of "chunkSize" bytes.
    void chunkFile(char *fullFilePath, char *chunkName) {
        ifstream fileStream;
        fileStream.open(fullFilePath, ios::in | ios::binary);
        // File open a success
        if (fileStream.is_open()) {
            fileStream.seekg(0, ios::end); // To get file size
            int tam = static_cast <int> (fileStream.tellg());
            fileStream.close();
            fileStream.open(fullFilePath, ios::in | ios::binary);
            int chunk_size = (tam/3)+1;
            cout << tam << endl;
            cout << chunk_size << endl;
            ofstream output;
            int counter = 1;
            string fullChunkName;

            //Variables necesarias para la aleatoriedad del almacenamiento en las carpetas del RAID
            vector<int> aleatorio = {1,2,3,4};
            int elemento;
            int cont = 4;

            // Create a buffer to hold each chunk
            char *buffer = new char[chunk_size];
            // Keep reading until end of file
            while (!fileStream.eof()) {
                // Build the chunk file name. Usually drive:\\chunkName.ext.N

                // N represents the Nth chunk
                srand(time(NULL)*time(NULL)*time(NULL)*10);
                elemento = rand() % cont;
                fullChunkName.clear();
                fullChunkName.append(chunkName);
                fullChunkName.append(".");
                // Convert counter integer into string and append to name.
                char intBuf[10];
                itoa(counter,intBuf,10);
                fullChunkName.append(intBuf);
                // Open new chunk file name for output
                output.open((string)"../"+"RAID/"+to_string(aleatorio[elemento])+(string)"/"+(string)fullChunkName.c_str(),ios::out | ios::trunc | ios::binary);
                cout << "Elemento aleatorio: " << aleatorio[elemento] << endl;
                cout << "Chunk number " << counter << " will be saved in"<< (string)"../"+"RAID/"+to_string(aleatorio[elemento])+(string)"/"+(string)fullChunkName.c_str() << endl;
                // If chunk file opened successfully, read from input and
                // write to output chunk. Then close.
                if (output.is_open()) {
                    fileStream.read(buffer, chunk_size);
                    // gcount() returns number of bytes read from stream.
                    output.write(buffer,fileStream.gcount());
                    output.close();
                    counter++;
                    cont--;
                    aleatorio.erase(std::find(aleatorio.begin(),aleatorio.end(),aleatorio[elemento]));
                }
            }
            // Cleanup buffer
            delete(buffer);
            // Close input file stream.
            fileStream.close();
            cout << "Chunking complete! " << counter - 1 << " files created." << endl;
        }
        else { cout << "Error opening file!" << endl; }
    }
/* A C++ program to implement itoa() */

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}

// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}
// Finds chunks by "chunkName" and creates file specified in fileOutput
    void joinFile(char *chunkName, char *fileOutput) {
        string fileName;

        // Create our output file
        ofstream outputfile;
        outputfile.open(fileOutput, ios::out | ios::binary);

        // If successful, loop through chunks matching chunkName
        if (outputfile.is_open()) {
            bool filefound = true;
            int counter = 1;
            int fileSize = 0;

            while (filefound) {
                filefound = false;

                // Build the filename
                fileName.clear();
                fileName.append(chunkName);
                fileName.append(".");

                char intBuf[10];
                itoa(counter,intBuf,10);
                fileName.append(intBuf);

                // Open chunk to read
                ifstream fileInput;
                fileInput.open(fileName.c_str(), ios::in | ios::binary);

                // If chunk opened successfully, read it and write it to
                // output file.
                if (fileInput.is_open()) {
                    filefound = true;
                    fileSize = getFileSize(&fileInput);
                    char *inputBuffer = new char[fileSize];

                    fileInput.read(inputBuffer,fileSize);
                    outputfile.write(inputBuffer,fileSize);
                    delete(inputBuffer);

                    fileInput.close();
                }
                counter++;
            }

            // Close output file.
            outputfile.close();

            cout << "File assembly complete!" << endl;
        }
        else { cout << "Error: Unable to open file for output." << endl; }

    }

// Simply gets the file size of file.
    int getFileSize(ifstream *file) {
        file->seekg(0,ios::end);
        int filesize = file->tellg();
        file->seekg(ios::beg);
        return filesize;
    }
};


#endif //MYINVICIBLELIBRARY_AUXILIAR_H
