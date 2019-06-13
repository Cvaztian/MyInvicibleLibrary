#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#define PORT 8080
/** @brief Clase encargada de la conexion por sockets
 * Clase encargada de la conexion mediante sockets. La progra esta distribuida en 4
 * librerias, 3 principales y una de conexion, las tres principales se comunican
 * con las demas mediante la de conexion por medio de sockets.
 * @author Elorim
 * @date 07/07/19
*/
class ServerSocket
{
public:
    /**
      Constructor default, encargado de inicializar el servidor.
      @author Elorim
*/
    ServerSocket();

    /** @brief Envia mensajes.
     * Funcion encargada de enviar mensajes a un cliente especifico. Solo acepta
     * los clientes que se establecen en el server.
     * @param mensaje Mensaje a enviar, tipo std::string.
     * @param socket Socket a enviar, tipo std::string. Puede ser base o raid, solamente.
     * @author Elorim
*/
    void sendS(std::string mensaje, std::string socket);

    /** @brief Recibe mensajes.
     * Funcion encargada de recibir mensajes de un cliente especifico. Solo acepta
     * a los clientes que se establecen en el server.
     * @param socket Sockeat a enviar, tipo std::string. Puede ser base o raid, solamente.
     * @author Elorim
*/
    std::string receiveS(std::string socket);

    /** @brief receiveS with extra space
     * Este metodo se utiliza para enviar y recibir metadata con su respectiva imagen,
     * esto porque las imagenes ocupan mucho mas espacio que el metodo recibir normal.
     * @param socket Sockect a enviar, tipo std::string. Puede ser base o raid, solamente.
     * @author Elorim
     * */
    std::string specialReceive(std::string socket);

    void specialSend(std::string mensaje, std::string socket);

private:
    int base;  /** Socket de la base de datos*/
    int raid; /** Socket del raid*/
};

#endif // SERVERSOCKET_H
