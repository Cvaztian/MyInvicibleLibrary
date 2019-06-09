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
