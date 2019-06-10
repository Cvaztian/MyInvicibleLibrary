// basic file operations
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include "dbmanager.h"
#include "clientmanager.h"

using namespace std;

int main () {
    /*
    // Creating a directory
        if (mkdir("a", 0777) == -1)
            cerr << "Error :  " << strerror(errno) << endl;  // Si ya existe
        else
            cout << "Directory created";  // Si no existe

        // Verificando directorio
        struct stat info;
        char* pathname = "a";
        if( stat( pathname, &info ) != 0 )
            printf( "cannot access %s\n", pathname );
        else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows
            printf( "%s is a directory\n", pathname );
        else
            printf( "%s is no directory\n", pathname );

        // Verificando si archivo existe
        ifstream f("a/example.txt");
        f.good();
        cout << f.good() << endl;  // 1 de existir, 0 si no.

        // Escribiendo en archivo o creando.
        nlohmann::json j;
        j = {{"id",1},{"Imagen","123"}};  // Al escribir las cosas en json no hay espacios.
        ofstream myfile;
        myfile.open ("./example.json");  // Se puede especificar una carpeta, si existe, crea el archivo dentro, si no, no hace nada.
        myfile << j;  // Esto crea un nuevo archivo y reescribe todo lo que hay en el
        myfile.close();

        // Leyendo
        string data;
        ifstream infile;
           infile.open("a/example.txt");

           cout << "Reading from the file" << endl;
           infile >> data;  // Lee linea por linea

           // write the data at the screen.
           cout << data << endl;

           // Extrayendo json
           nlohmann::json js = nlohmann::json::parse(data);
           cout << js["id"] <<endl;  // json funciona

           // close the opened file.
           infile.close();

        // Eliminando archivo
        if (remove("example.txt") == 0)
           printf("Deleted successfully"); // Existe
        else
           printf("Unable to delete the file");  // No existe
           */
    /*
    DBManager* manager = new DBManager();
    json j = {
        {"nombre","ala"},
        {"galeria","misrecuerdos"},
        {"autor","Kevin"},
        {"descripcion","Quesuenno!!"},
        {"year","2019"},
        {"id",-1},
        {"size",-1},
        {"imagen",""},
        {"mensaje",""},
        {"protocolo",-1}
    };
   // cout << manager->Insert(j);
    cout << manager->Delete("misrecuerdos","ala");
    delete manager;*/
    
    ClientManager("base");

  return 0;
}
