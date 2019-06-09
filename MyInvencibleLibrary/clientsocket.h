#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <string>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#define PORT 8080

using namespace std;
/** @brief Cliente de sockets.
 * Clase encargada de comunicarse con el servidor mediante sockets.
 * Se encarga de realizar correctamente la conexion con el server y de
 * proveer una interfaz para comunicarse con el mismo.
 * @author Elorim
 * @date 09/07/2019
*/
class ClientSocket
{
public:
    /** @brief Constructor default
     * Se encarga de realizar la conexion con el server y de identificar
     * que tipo de cliente se es.
     * @param tipo Que tipo de cliente es, puede ser base o raid
*/
    ClientSocket(string tipo);

    /** @brief Envia un mensaje al server
     * Funcion que se encarga de todo lo necesario para enviar un mensaje
     * @param mensaje Mensaje a enviar al server
*/
    void sendS(string mensaje);

    /** @brief Recibe un mensaje del server
     * Funcion que se encarga de todo los necesario para recibir un mensaje
     * del server
     * @returns Mensaje recibido
*/
    string receiveS();
private:
    string tipo; /** Guarda el tipo de cliente que es*/
    int sock; /** Socket del servidor*/
};

#endif // CLIENTSOCKET_H
