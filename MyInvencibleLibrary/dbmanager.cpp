#include "dbmanager.h"

int DBManager::id =0;
DBManager::DBManager()
{
    // recordar que al iniciar un nuevo programa que ejecuta la base de datos
    // se necesita, de alguna manera, saber cual es el numero del ultimo id
    // y guardarlo en DBManager::id.

    // Primeramente lee el id del file
    if(checkFile("lastid.txt",".")){
        string data;
        ifstream infile;
           infile.open("lastid.txt");

           infile >> data;  // Lee linea por linea
           DBManager::id = atoi(data.c_str());  // Establece el id
           // close the opened file.
           infile.close();
    }else{  // Si no hay registro de id anterior
        DBManager::id = 0;
    }
}

DBManager::~DBManager()
{
    // Tiene que guardar el id en un file.
        ofstream myfile;
        myfile.open ("lastid.txt");  // Se puede especificar una carpeta, si existe, crea el archivo dentro, si no, no hace nada.
        myfile << DBManager::id;  // Esto crea un nuevo archivo y reescribe todo lo que hay en el
        myfile.close();
}

json DBManager::Select(string galeria, string nombre)
{
    if(checkGalery(galeria) && checkFile(nombre, galeria)){ // Si existen
        json response;
        string data;
        ifstream infile;
        string spath = galeria + "/" + nombre+".json";
        const char* path = spath.c_str();
        infile.open(path);
        infile >> data;
        response = json::parse(data);
        infile.close();
        return response;
    }else{  // Si no existe alguno
        return "404";
    }
}

string DBManager::Update(json metadata)
{
    Metadata metadataObj = Metadata::jsonParse(metadata);
    if(checkGalery(metadataObj.galeria) && checkFile(metadataObj.nombre,metadataObj.galeria)){ // Check existence
        string spath = metadataObj.galeria + "/" + metadataObj.nombre+".json";
        const char* path = spath.c_str();
        ofstream myfile;
        myfile.open (path);  // Se puede especificar una carpeta, si existe, crea el archivo dentro, si no, no hace nada.
        myfile << metadata;  // Esto crea un nuevo archivo y reescribe todo lo que hay en el
        myfile.close();
        return "Success";
    }else{
        return "404";
    }
}

string DBManager::Insert(json metadata)
{
    Metadata metadataObj = Metadata::jsonParse(metadata);
    if(!(checkGalery(metadataObj.galeria) && checkFile(metadataObj.nombre,metadataObj.galeria))){  // Check for non existence
        //Crea galeria
        if (mkdir(metadataObj.galeria.c_str(), 0777) == -1)
            cerr << "Error :  " << strerror(errno) << endl;  // Si ya existe
        else
            cout << "Directory created";  // Si no existe

        // Crea file
        int nid = DBManager::id;  // Id del archivo
        DBManager::id++;
        metadataObj.id = nid;  // Asigna el id
        ofstream myfile;
        myfile.open ("./example.json");  // Se puede especificar una carpeta, si existe, crea el archivo dentro, si no, no hace nada.
        myfile << metadataObj.getJson();  // Esto crea un nuevo archivo y reescribe todo lo que hay en el
        myfile.close();
        return "Success";
    }else if(checkGalery(metadataObj.galeria)){
        // Crea file
        int nid = DBManager::id;  // Id del archivo
        DBManager::id++;
        metadataObj.id = nid;  // Asigna el id
        ofstream myfile;
        myfile.open ("./example.json");  // Se puede especificar una carpeta, si existe, crea el archivo dentro, si no, no hace nada.
        myfile << metadataObj.getJson();  // Esto crea un nuevo archivo y reescribe todo lo que hay en el
        myfile.close();
        return "Success";
    }else{
        return "406";
    }
}

string DBManager::Delete(string galeria, string nombre)
{
    if(checkGalery(galeria) && checkFile(nombre, galeria)){
        string spath = galeria + "/" + nombre+".json";
        const char* path = spath.c_str();
        remove(path);
        return "Success";
    }else{
        return "404";
    }
}

bool DBManager::checkGalery(string galeria)
{
    struct stat info;
        const char* pathname = galeria.c_str();
        if( stat( pathname, &info ) != 0 ){
            printf( "cannot access %s\n", pathname );  // No existe
            return false;
        }
        else if( info.st_mode & S_IFDIR ){  // S_ISDIR() doesn't exist on my windows
            printf( "%s is a directory\n", pathname );  // Existe
            return true;
        }
        else{
            printf( "%s is no directory\n", pathname );  // No es directorio
            return false;
        }
}

bool DBManager::checkFile(string nombre, string galeria)
{
    string spath = galeria + "/" + nombre;
    const char* path = spath.c_str();
    ifstream f(path);
    return f.good();
}
