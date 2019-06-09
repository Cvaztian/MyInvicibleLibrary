#include "dbmanager.h"

int DBManager::id =0;
DBManager::DBManager()
{
    // recordar que al iniciar un nuevo programa que ejecuta la base de datos
    // se necesita, de alguna manera, saber cual es el numero del ultimo id
    // y guardarlo en DBManager::id.
}

json DBManager::Select(string galeria, string nombre)
{
    if(checkGalery(galeria) && checkFile(nombre, galeria)){ // Si existen
        json response;
        string data;
        ifstream infile;
        string spath = galeria + "/" + nombre;
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

}

string DBManager::Insert(json metadata)
{

}

string DBManager::Delete(string galeria, string nombre)
{

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
