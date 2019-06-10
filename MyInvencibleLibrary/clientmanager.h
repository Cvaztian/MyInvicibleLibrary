#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
#include <string>
#include <iostream>
#include "clientsocket.h"
#include "nlohmann/json.hpp"
#include "metadata.h"
#include "dbmanager.h"

using namespace std;
/** @brief Administra la conexion entre server y cliente mediante sockets
 * Esta funcion se encarga de administrar la conexion de los diferentes
 * modulos del proyecto desde cada modulo hacia el server. Interpreta
 * los protocolos y los conecta con sus contrapartes en RAID o metadatadb,
 * tambien se encarga de todas las formalidades para comunicarse con el server
 * @author Elorim
 * @date 09/07/2019
*/
class ClientManager
{
public:
    /** @brief Constructor estandar
     * @param tipo El tipo de cliente que es.
*/
     ClientManager(string tipo);
     ClientManager(){}
private:
    ClientSocket* sockets; /** Puntero a la clase encargada de la comunicacion*/
    /** @brief Mainloop
     * Mainloop de cada cliente, donde ocurre toda la logica.
*/
    void mainloop();

    /** @brief Ejecuta get
     * Interpretacion de protocolo 0. Ejecuta get ya sea en la base de datos
     * o en el raid.
     * @returns La respuesta de la base de datos o el raid
*/
    void Get(json metadata);

    /** @brief Ejecuta Update
     * Interpretacion de protocolo 1. Ejecuta Update o lo analogo en la base
     * de datos o el raid.
     * @returns La respuesta de la base de datos o el raid
*/
    void actualizar(json metadata);

    /** @brief Ejecuta Delete
     * Interpretacion de protocolo 1. Ejecuta Delete o lo analogo en la base
     * de datos o el raid.
     * @returns La respuesta de la base de datos o el raid
*/
    void eliminar(json metadata);

   /** @brief Ejecuta Insert
     * Interpretacion de protocolo 1. Ejecuta Insert o lo analogo en la base
     * de datos o el raid.
     * @returns La respuesta de la base de datos o el raid
*/
    void crear(json metadata);

    string tipo; /** Tipo de cliente*/
    DBManager* baseDatos;  /** La base de datos*/

};

#endif // CLIENTMANAGER_H
