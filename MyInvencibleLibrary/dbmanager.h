#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <string>
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include "metadata.h"
#include "dirent.h"


using namespace nlohmann;
using namespace std;
/** @brief Manager de la base de datos.
 * Clase encargada de administrar la base de datos y proveer la interfaz
 * necesaria para trabajar con la misma. Los metodos que provee son:
 * SELECT, UPDATE, INSERT, DELETE, cada metodo es bastante claro por si solo.
 * Cabe destacar que no pueden haber dos galerias con el mismo nombre o dos
 * metadatas con el mismo nombre dentro de la misma galeria, sin embargo
 * si pueden haber dos metadatas con el mismo nombre, siempre y cuando esten
 * en diferentes galerias.
 * @author Elorim
 * @date 09/07/2019
*/
class DBManager
{
public:
    /** @brief Constructor estandar*/
    DBManager();
    /** @brief Destructor estandar.*/
    ~DBManager();
    /** @brief Devuelve la metadata necesaria
     * Este metodo es el analogo a un select de SQL, dados un nombre y una
     * galeria se encarga de devolver la metada en formato json asociados
     * al nombre y la galeria. De no existir la galeria o el nombre tira 404.
     * @param galeria Nombre de la carpeta(o galeria) donde se ubica la imagen
     * @param nombre Nombre de la imagen en la carpeta.
     * @returns json Con la metadata asociada al nombre y la galeria
     * @exceptions 404 de no existir galeria o imagen.
*/
    json Select(string galeria, string nombre);

    /** @brief Devuelve las metadatas de toda una galeria
     * Metodo necesario para la correcta implentacion del SELECT SQL.
     * @param galeria
     * @return Un json con un array de jsons en su forma de string.
     * */
     json SelectAll(string galeria);

    /** @brief Actualiza la metadata especificada
     * Metodo analogo a update de SQL, dada una metadata, extrae el nombre
     * y la galeria y sobreescribe el archivo encontrado. De no existir el
     * nombre o la galeria tira 404.
     * @param metadata Json con la metadata a sobreescribir
     * @returns mensaje de exito
     * @exceptions 404 de no existir galeria o imagen.
*/
    string Update(json metadata);

    /** @brief Crea una nueva metadata
     * Metodo analogo a insert de SQL, dada una metadata, crea el archivo
     * con el nombre en la galeria especificada, o, de no existir galeria,
     * crea tanto galeria como metadata.
     *
     * Si existe galeria, pero no imagen,
     * crea la imagen en la galeria especificada.
     *
     * Si existe galera e imagen, tira 406.
     * @param metadata Json con la nueva metadata a crear.
     * @returns mensaje de exito
     * @exceptions 406 de existir tanto galeria como imagen.
*/
    string Insert(json metadata);

    /** @brief Borra una metadata dada
     * Metodo analogo a delete de SQL, dada una metadata, borra el archivo
     * con el nombre en la galeria especificada. De no existir la galeria o
     * el nombre tira 404.
     * @param galeria Galeria de donde se quiere eliminar
     * @param nombre Nombre del archivo a eliminar
     * @returns Mensaje de exito
     * @exceptions 404 de no existir galeria o nombre
*/
    string Delete(string galeria, string nombre);
private:
    static int id; /** Contador estatico del id de los archivos*/


    /** @brief Verifica si existe galeria.
     * Version auxiliar que verifica la existencia de una galeria
     * @param galeria Nombre de la galeria
     * @returns Si existe la libreria o no.
*/
    bool checkGalery(string galeria);

    /** @brief Verifica si existe el archivo
     * Dadas un nombre de archivo y una galeria, verifica si existe
     * el archivo.
     * @param nombre Nombre del archivo
     * @param galeria Nombre de la galeria
     * @returns Si existe el archivo en la galeria o no.
*/
    bool checkFile(string nombre, string galeria);
};

#endif // DBMANAGER_H
