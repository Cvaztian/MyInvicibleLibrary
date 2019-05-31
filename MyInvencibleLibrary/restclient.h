#ifndef RESTCLIENT_H
#define RESTCLIENT_H
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

/**
Clase encargada de manejar las solicitudes al REST server.
@author elorim
@date 28/05/2018
*/
class RestClient
{
public:
    RestClient();
    static std::string respuesta; /**Atributo encargado de portar la respuesta desde el servidor*/
    static void Get();
    static void Put(std::string mensaje);
    static void Post(std::string mensaje);
    static void Delete(std::string mensaje);
};

#endif // RESTCLIENT_H
