#ifndef RESTCLIENT_H
#define RESTCLIENT_H
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include "nlohmann/json.hpp"
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

/** @brief Clase encargada de manejas solicitudes al REST.
 * Esta clase utiliza Casablance para realizar solicitudes al server
 * y procesarlas, retorna los JSON, no retorna los objetos que incluyen
 * estos.
 * @author Elorim
 * @date 28/05/2018
*/
class RestClient
{
public:
    /** Constructor default, no utilizado. */
    RestClient(); 
    static std::string respuesta; /**Atributo encargado de portar la respuesta desde el servidor*/
    /**
      Metodo HTML GET.
      @param mensaje Mensaje a enviar
      @author Elorim
*/
    static void Get(std::string mensaje);
    /**
      Metodo HTML PUT.
      @param mensaje Mensaje a enviar
      @author Elorim
      @returns Respuesta del servidor en la variable respuesta
*/
    static void Put(std::string mensaje);
    /**
      Metodo HTML POST.
      @param mensaje Mensaje a enviar
      @author Elorim
      @returns Respuesta del servidor en la variable respuesta
*/
    static void Post(std::string mensaje);
    /**
      Metodo HTML DELETE.
      @param mensaje Mensaje a enviar
      @author Elorim
      @returns Respuesta del servidor en la variable respuesta
*/
    static void Delete(std::string mensaje);
};

#endif // RESTCLIENT_H
