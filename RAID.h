//
// Created by malavassi on 02/06/19.
//

#ifndef MYINVICIBLELIBRARY_RAID_H
#define MYINVICIBLELIBRARY_RAID_H
#include <iostream>;
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <sys/stat.h>

using namespace std;
/**
 * Clase utilizada para realizar la simulacion del funcionamiento de un RAID 5
 */
class RAID {
public:
// Chunks a file by breaking it up into chunks of "chunkSize" bytes.
/**
 * Parte un archivo en 3 trozos y los almacena aleatoriamente en los discos del RAID (carpetas), calcula la paridad basada
 * en los 3 archivos existentes y el disco restante se utiliza para almacenar el archivo de paridad generado
 * @param fullFilePath ruta del archivo que se quiere almacenar
 * @param chunkName nombre del archivo con el que se almacenaran los trozos
 */
    void write(char *fullFilePath, char *chunkName) {
        ifstream fileStream;
        fileStream.open(fullFilePath, ios::in | ios::binary);
        // File open a success
        if (fileStream.is_open()) {
            int chunk_size = (getFileSize(&fileStream) / 3) + 1;
            vector<char *> particiones;
            vector<int> aleatorio = {1, 2, 3, 4};
            int elemento;
            int trozo = 4;
            ofstream output;
            int counter = 1;
            string fullChunkName;
            cout << "El tamano del archivo es: " << getFileSize(&fileStream) << endl;
            cout << "El tamano de cada archivo parte sera: " << chunk_size << endl;

            // Create a buffer to hold each chunk
            char *buffer = new char[chunk_size];
            // Keep reading until end of file
            while (!fileStream.eof()) {
                // Build the chunk file name. Usually drive:\\chunkName.ext.N
                // N represents the Nth chunk
                srand(time(NULL) * time(NULL) * time(NULL) * 10);
                elemento = rand() % trozo;
                fullChunkName.clear();
                fullChunkName.append(chunkName);
                fullChunkName.append(".");
                // Convert counter integer into string and append to name.
                char intBuf[10];
                itoa(counter, intBuf, 10);
                fullChunkName.append(intBuf);
                // Open new chunk file name for output
                output.open((string) "../RAID/" + to_string(aleatorio[elemento]) + (string) "/" +
                            (string) fullChunkName.c_str(), ios::out | ios::trunc | ios::binary);
                cout << "Elemento aleatorio: " << aleatorio[elemento] << endl;
                cout << "Chunk number " << counter << " will be saved in"
                     << (string) "../" + "RAID/" + to_string(aleatorio[elemento]) + (string) "/" +
                        (string) fullChunkName.c_str() << endl;
                // If chunk file opened successfully, read from input and
                // write to output chunk. Then close.
                if (output.is_open()) {
                    fileStream.read(buffer, chunk_size);
                    particiones.push_back(buffer);
                    // gcount() returns number of bytes read from stream.
                    output.write(buffer, fileStream.gcount());
                    output.close();
                    aleatorio.erase(std::find(aleatorio.begin(), aleatorio.end(), aleatorio[elemento]));
                    counter++;
                    trozo--;
                }
            }
            // Cleanup buffer
            delete (buffer);
            // Close input file stream.
            fileStream.close();
            cout << "Chunking complete! " << counter - 1 << " files created." << endl;
            char *paridad = new char[chunk_size];
            for (int i = 0; i <= chunk_size; i++) {
                paridad[i] = XOR(particiones[0][i], particiones[1][i], particiones[2][i]);
            }
            ofstream archivo_paridad;
            archivo_paridad.open(
                    (string) "../RAID/" + to_string(aleatorio.front()) + (string) "/" + (string) chunkName + ".paridad",
                    ios::out | ios::trunc | ios::binary);
            cout << "Estoy guardando paridad en: "
                 << (string) "../RAID/" + to_string(aleatorio.front()) + (string) "/" + (string) chunkName + ".paridad"
                 << endl;
            if (archivo_paridad.is_open()) {
                archivo_paridad.write(paridad, chunk_size);
            } else {
                cout << "No se logro abrir archivo para la paridad" << endl;
            }
            archivo_paridad.close();
            delete (paridad);
        } else { cout << "Error opening file!" << endl; }
    }

    /**
     * Busca los trozos de un archivo y los une para generar el archivo original que fue almacenado, si esto no es
     * posible por la perdida de un trozo del archivo entonces lo reconstruye y se llama a si misma para completar el
     * proceso. Si se pierde la paridad la recalcula.
     * @param chunkName nombre del archivo que se quiere generar (leer)
     */
    void read(char *chunkName) {
        string fileName;
        char *fileOutput = chunkName;
        // Create our output file
        ofstream outputfile;
        outputfile.open("../" + (string) fileOutput, ios::out | ios::binary);

        // If successful, loop through chunks matching chunkName
        if (outputfile.is_open()) {
            bool filefound = true;
            int counter = 1;
            int fileSize = 0;
            int contador = 1;
            vector<char *> particiones;
            vector<int> carpetas_por_encontrar = {1, 2, 3, 4};
            vector<int> archivos_por_encontrar = {1, 2, 3, 4};
            while (filefound) {
                // Build the filename
                fileName.clear();
                fileName.append(chunkName);
                fileName.append(".");

                char intBuf[10];
                itoa(counter, intBuf, 10);
                fileName.append(intBuf);

                // Open chunk to read
                ifstream fileInput;
                // If chunk opened successfully, read it and write it to
                // output file.
                if (counter == 4) {
                    fileInput.open("../RAID/" + to_string(carpetas_por_encontrar[0]) + "/" + (string) chunkName +".paridad", ios::in | ios::binary);
                    cout << "../RAID/" + to_string(carpetas_por_encontrar[0]) + "/" + (string) chunkName +".paridad" << endl;
                    //-----------------------
                    if (fileInput.is_open()) {
                        //-----------------------
                        cout << "Abri: "<< "../RAID/" + to_string(carpetas_por_encontrar[0]) + "/" + (string) chunkName +".paridad" << endl;
                        carpetas_por_encontrar.pop_back();
                        archivos_por_encontrar.erase(std::find(archivos_por_encontrar.begin(), archivos_por_encontrar.end(), counter));
                        fileSize = getFileSize(&fileInput);
                        char *inputBuffer = new char[fileSize];
                        fileInput.read(inputBuffer, fileSize);
                        outputfile.write(inputBuffer, fileSize);
                        particiones.push_back(inputBuffer);
                        delete (inputBuffer);
                        cout << "Se finalizo la busqueda de trozos" << endl;
                        outputfile.close();
                        fileInput.close();
                        if (archivos_por_encontrar.size() > 1) {
                            cout << "Imposible unir archivos debido al fallo simultaneo de 2 discos. Problema desconocido" << endl;
                            break;
                        }else if(archivos_por_encontrar.size() == 1){
                            cout << "No fue posible encontrar todos los trozos, el disco caido es el disco numero: "<< carpetas_por_encontrar[0] << " y el archivo perdido es: " << archivos_por_encontrar[0]  << endl;
                            reconstruir_archivo(to_string(archivos_por_encontrar[0]), carpetas_por_encontrar[0], chunkName, fileSize,particiones);
                            read(chunkName);
                            break;
                        }else{
                            cout << "Quedan " << archivos_por_encontrar.size() << " archivos por encontrar" << endl;
                            cout << "Reconstruccion completa" << endl;
                            // Close output file.
                            outputfile.close();
                            break;
                        }
                        //este es el else del filenput.isopen()
                    }else{
                        if (archivos_por_encontrar.size() > 1) {
                            cout << "Imposible unir archivos debido al fallo simultaneo de 2 discos. Problema desconocido" << endl;
                            break;
                        }else{
                            cout << "No fue posible encontrar todos los trozos, el disco caido es el disco numero: "<< carpetas_por_encontrar[0] << " y el archivo perdido es la paridad"  << endl;
                            reconstruir_archivo("paridad", carpetas_por_encontrar[0], chunkName, fileSize,particiones);
                            cout << "Paridad recalculada" << endl;
                            read(chunkName);
                            break;
                        }
                    }
                    //este es el else del counter == 4
                } else {
                    fileInput.open("../RAID/" + to_string(contador) + "/" + (string) fileName.c_str(),
                                   ios::in | ios::binary);
                    cout << "Estoy intentando abrir: " << "../RAID/" + to_string(contador) + "/" + (string) fileName.c_str()
                         << endl;
                    if (fileInput.is_open()) {
                        cout << "Abri: " << "../RAID/" + to_string(contador) + "/" + (string) fileName.c_str() << endl;
                        carpetas_por_encontrar.erase(std::find(carpetas_por_encontrar.begin(), carpetas_por_encontrar.end(), contador));
                        archivos_por_encontrar.erase(std::find(archivos_por_encontrar.begin(), archivos_por_encontrar.end(), counter));
                        filefound = true;
                        fileSize = getFileSize(&fileInput);
                        char *inputBuffer = new char[fileSize];

                        fileInput.read(inputBuffer, fileSize);
                        outputfile.write(inputBuffer, fileSize);
                        particiones.push_back(inputBuffer);
                        delete (inputBuffer);

                        fileInput.close();
                        counter++;
                        cout << "El tamano de carpetos_por_encontrar es: " << carpetas_por_encontrar.size() << endl;
                        cout << "El tamano de particiones es: " << particiones.size() << endl;
                        contador = 1;
                    } else {
                        filefound = true;
                        if (contador == 4) {
                            cout << "No se encontro el archivo: " << counter << endl;
                            counter++;
                            contador = 1;
                        } else {
                            contador++;
                        }
                    }
                }
            }
            cout << "File assembly complete!" << endl;


        }else { cout << "Error: Unable to open file for output." << endl; }
    }

    // Finds chunks by "chunkName" and creates file specified in fileOutput
    /**
     *
     * @param chunkName
     */
    void seek(char *chunkName) {
        string fileName;

        // If successful, loop through chunks matching chunkName
        bool filefound = true;
        int counter = 1;
        int fileSize = 0;
        int contador = 1;
        vector<char *> particiones;
        vector<int> carpetas_por_encontrar = {1, 2, 3, 4};
        vector<int> archivos_por_encontrar = {1, 2, 3, 4}; //aqui 4 se le llamara a la paridad
        while (filefound) {
            // Build the filename
            fileName.clear();
            fileName.append(chunkName);
            fileName.append(".");

            char intBuf[10];
            itoa(counter, intBuf, 10);
            fileName.append(intBuf);

            // Open chunk to read
            ifstream fileInput;
            // If chunk opened successfully, read it and write it to
            // output file.
            if (counter == 4) {
                fileInput.open(
                        "../RAID/" + to_string(carpetas_por_encontrar[0]) + "/" + (string) chunkName + ".paridad",
                        ios::in | ios::binary);
                cout << "../RAID/" + to_string(carpetas_por_encontrar[0]) + "/" + (string) chunkName + ".paridad"
                     << endl;
                //-----------------------
                if (fileInput.is_open()) {
                    //-----------------------
                    cout << "Abri: "
                         << "../RAID/" + to_string(carpetas_por_encontrar[0]) + "/" + (string) chunkName + ".paridad"
                         << endl;
                    carpetas_por_encontrar.pop_back();
                    archivos_por_encontrar.erase(
                            std::find(archivos_por_encontrar.begin(), archivos_por_encontrar.end(), counter));
                    fileSize = getFileSize(&fileInput);
                    char *inputBuffer = new char[fileSize];
                    fileInput.read(inputBuffer, fileSize);
                    particiones.push_back(inputBuffer);
                    delete (inputBuffer);
                    cout << "Se finalizo la busqueda de trozos" << endl;
                    fileInput.close();
                    if (archivos_por_encontrar.size() > 1) {
                        cout << "Imposible unir archivos debido al fallo simultaneo de 2 discos. Problema desconocido"
                             << endl;
                        break;
                    } else if (archivos_por_encontrar.size() == 1) {
                        cout << "No fue posible encontrar todos los trozos, el disco caido es el disco numero: "
                             << carpetas_por_encontrar[0] << " y el archivo perdido es: " << archivos_por_encontrar[0]
                             << endl;
                        reconstruir_archivo(to_string(archivos_por_encontrar[0]), carpetas_por_encontrar[0], chunkName,
                                            fileSize, particiones);
                        break;
                    } else {
                        cout << "Quedan " << archivos_por_encontrar.size() << " archivos por encontrar" << endl;
                        cout << "Reconstruccion completa" << endl;
                        // Close output file.
                        break;
                    }
                    //este es el else del filenput.isopen()
                } else {
                    if (archivos_por_encontrar.size() > 1) {
                        cout << "Imposible unir archivos debido al fallo simultaneo de 2 discos. Problema desconocido"
                             << endl;
                        break;
                    } else {
                        cout << "No fue posible encontrar todos los trozos, el disco caido es el disco numero: "
                             << carpetas_por_encontrar[0] << " y el archivo perdido es la paridad" << endl;
                        reconstruir_archivo("paridad", carpetas_por_encontrar[0], chunkName, fileSize, particiones);
                        cout << "Paridad recalculada" << endl;
                        break;
                    }
                }
                //este es el else del counter == 4
            } else {
                fileInput.open("../RAID/" + to_string(contador) + "/" + (string) fileName.c_str(),
                               ios::in | ios::binary);
                cout << "Estoy intentando abrir: " << "../RAID/" + to_string(contador) + "/" + (string) fileName.c_str()
                     << endl;
                if (fileInput.is_open()) {
                    cout << "Abri: " << "../RAID/" + to_string(contador) + "/" + (string) fileName.c_str() << endl;
                    carpetas_por_encontrar.erase(
                            std::find(carpetas_por_encontrar.begin(), carpetas_por_encontrar.end(), contador));
                    archivos_por_encontrar.erase(
                            std::find(archivos_por_encontrar.begin(), archivos_por_encontrar.end(), counter));
                    filefound = true;
                    fileSize = getFileSize(&fileInput);
                    char *inputBuffer = new char[fileSize];
                    fileInput.read(inputBuffer, fileSize);
                    particiones.push_back(inputBuffer);
                    delete (inputBuffer);

                    fileInput.close();
                    counter++;
                    cout << "El tamano de carpetos_por_encontrar es: " << carpetas_por_encontrar.size() << endl;
                    cout << "El tamano de particiones es: " << particiones.size() << endl;
                    contador = 1;
                } else {
                    filefound = true;
                    if (contador == 4) {
                        cout << "No se encontro el archivo: " << counter << endl;
                        counter++;
                        contador = 1;
                    } else {
                        contador++;
                    }
                }
            }
        }
        cout << "File assembly complete!" << endl;
    }




/* A C++ program to implement itoa() */

/* A utility function to reverse a string  */
/**
 *
 * @param str
 * @param length
 */
    void reverse(char str[], int length) {
        int start = 0;
        int end = length - 1;
        while (start < end) {
            swap(*(str + start), *(str + end));
            start++;
            end--;
        }
    }

// Implementation of itoa()
/**
 *
 * @param num
 * @param str
 * @param base
 * @return
 */
    char *itoa(int num, char *str, int base) {
        int i = 0;
        bool isNegative = false;

        /* Handle 0 explicitely, otherwise empty string is printed for 0 */
        if (num == 0) {
            str[i++] = '0';
            str[i] = '\0';
            return str;
        }

        // In standard itoa(), negative numbers are handled only with
        // base 10. Otherwise numbers are considered unsigned.
        if (num < 0 && base == 10) {
            isNegative = true;
            num = -num;
        }

        // Process individual digits
        while (num != 0) {
            int rem = num % base;
            str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
            num = num / base;
        }

        // If number is negative, append '-'
        if (isNegative)
            str[i++] = '-';

        str[i] = '\0'; // Append string terminator

        // Reverse the string
        reverse(str, i);

        return str;
    }


    // Simply gets the file size of file.
/**
 *
 * @param file
 * @return
 */
    int getFileSize(ifstream *file) {
        file->seekg(0,ios::end);
        int filesize = file->tellg();
        file->seekg(ios::beg);
        return filesize;
    }
/**
 *
 * @param a
 * @param b
 * @param c
 * @return
 */
    char XOR(char a, char b, char c){
    char d = a ^ b ^ c;
    return d;
    }
/**
 *
 * @param trozo
 * @param carpeta
 * @param chunkName
 * @param chunk_size
 * @param particiones
 */
    void reconstruir_archivo(string trozo, int carpeta, char* chunkName, int chunk_size, vector<char*> particiones){
        cout << "Se llamo a reconstruir_archivo para el trozo: "+(string)trozo+" en la carpeta (disco): "<< carpeta << "; particiones tiene un tamano de: " << particiones.size() << endl;
        char* paridad = new char[chunk_size];
        ofstream archivo;
        for(int i = 0; i <= chunk_size; i++){
            paridad[i] = XOR(particiones[0][i],particiones[1][i],particiones[2][i]);
        }
        archivo.open((string)"../RAID/"+to_string(carpeta)+"/"+(string)chunkName+"."+trozo,ios::out | ios::trunc | ios::binary);
        cout << "Estoy guardando archivo reconstruido en: " << (string)"../RAID/"+to_string(carpeta)+"/"+(string)chunkName+"."+trozo << endl;
        if(archivo.is_open()) {
            archivo.write(paridad, chunk_size);
            cout << "Reconstruccion completada" << endl;
        }else{
            cout << "No se logro abrir archivo para la reconstruccion" << endl;
        }
        archivo.close();
        delete(paridad);
    }


};


#endif //MYINVICIBLELIBRARY_RAID_H
