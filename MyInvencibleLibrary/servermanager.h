#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <iostream>
#include "serversocket.h"
#include "include/stdafx.h"
#include "include/handler.h"
#include "metadata.h"
#include "nlohmann/json.hpp"
#include "Interprete.h"

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
    /** @brief Inicia un socket hacia get.
     * Para asegurarse que diferentes
*/
    // Metodos HTTP
    /** @brief Retorna una imagen al cliente que la solicita.
     * Funcion de respuesta por html, responde ante una solicitud de get, devuelve
     * al cliente tanto la metadata de la imagen, como la imagen en si.
     * @author Elorim
     * @param message Proporcionado por el cliente, es un json de tipo
     * Metadata, para este metodo proporciona el nombre dela imagen y la galeria
     * de la cual se quiere obtener la imagen.
     * @exception Si la imagen no existe en la base de datos, tira error.
     * @returns Retorna al cliente la metadata y la imagen solicitadas.
*/
    void handle_get(http_request message);
    /** @brief Crea una imagen con su respectiva metadata en la base de datos.
     * Funcion de respuesta por html, responde ante una solicitud de put,
     * se encarga de crear una nueva imagen en la base de datos.
     * @param message Proporcionado por el cliente, es un json de tipo
     * Metadata, para este metodo propociona el nombre de la imagen y en la
     * cual se quiere crear la nueva imagen.
     * @returns Retorna al cliente el exito de la operacion.
     * @exception Si la imagen ya existe en la base de datos, tira error.
     * @author Elorim
*/
    void handle_put(http_request message);
    /** @brief actualiza una imagen en la base de datos.
     * Funcion de respuesta por html, responde ante una solicitud de put,
     * se encarga de actualizar una imagen ya existente en la base de datos.
     * @param message Proporcionado por el cliente, es un json de tipo
     * Metadata, para este metodo propociona el nombre de la imagen, la
     * galeria y todos los campos que se desean actualizar.
     * @returns Retorna al cliente mediante html el exito de la operacion
     * @exception Si la imagen no existe en la base de datos tira error
     * @author Elorim
*/
    void handle_post(http_request message);
    /** @brief Llama a la base de datos para eliminar la imagen especificada.
     * Funcion de respuesta por html, responde ante una solicitud de delete,
     * elimina la imagen de la base de datos.
     * @param message Proporcionado por el cliente, es un json de tipo
     * Metadata, para este metodo proporciona el nombre de la imagen y la galeria
     * de la cual se quiere eliminar la imagen.
     * @returns Retorna al cliente mediante html el exito de la operacion
     * @exception Si la imagen no existe en la base de datos tira error.
     * @author Elorim
*/
    void handle_delete(http_request message);

    /** @brief Maneja errores.
     * Maneja errores ocasionados por el html en algun momento de la ejecucion.
*/
    void handle_error(pplx::task<void>& t);

    http_listener m_listener; /** El listener de las solicitudes http*/
};

#endif // SERVERMANAGER_H
