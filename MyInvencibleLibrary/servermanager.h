#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <iostream>
#include "serversocket.h"
#include "include/stdafx.h"
#include "include/handler.h"

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


/** @brief Administrador del server.
 * Clase encargada de administrar todas las comunicaciones entre las diferentes
 * partes del proyecto, el corazon de ServerLib. Tiene dos partes principales:
 * el servidor REST y el servidor de sockets. El REST lo utiliza para comunicarse
 * con el cliente, se activa al recibir una peticion y envia una sola respuesta
 * al cliente. El servidor de sockets es más dinámico, permite una mejor comunicacion
 * con las diferentes partes del proyecto.
 * @author Elorim
 * @date 08/07/2019
*/
class ServerManager
{
public:
    /** @brief Constructor
     * Constructor default, inicializa los servers.
*/
    ServerManager(utility::string_t url);

    /** @brief Inicializa el server REST
     * Inicializa el servidor REST
*/
    pplx::task<void>open(){return m_listener.open();}
    /** @brief Cierra el server REST
     * Se encarga de cerrar el servidor REST.
*/
    pplx::task<void>close(){return m_listener.close();}


private:
    ServerSocket* sockets;  /** Puntero a la clase que maneja sockets*/

    // Metodos HTTP
    void handle_get(http_request message);
    void handle_put(http_request message);
    void handle_post(http_request message);
    void handle_delete(http_request message);
    void handle_error(pplx::task<void>& t);
    http_listener m_listener; /** El listener de las solicitudes http*/
};

#endif // SERVERMANAGER_H
