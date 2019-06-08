#include "servermanager.h"

ServerManager::ServerManager(utility::string_t url):m_listener(url)
{
    // Conectando modulos
    sockets = new ServerSocket();

    // Conectando http a metodos
    m_listener.support(methods::GET, std::bind(&ServerManager::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&ServerManager::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&ServerManager::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&ServerManager::handle_delete, this, std::placeholders::_1));
}

void ServerManager::handle_error(pplx::task<void>& t)
{
    try
    {
        t.get();
    }
    catch(...)
    {
        // Ignore the error, Log it if a logger is available
    }
}


//
// Get Request
//
void ServerManager::handle_get(http_request message)
{
    ucout << "Operacion get\n";

    // Parsing incoming message
    string returning;
    string contenido = message.to_string();
    nlohmann::json response = nlohmann::json(contenido); // Convierto a json
    Metadata responseObj = Metadata::jsonParse(response);  // Objeto metadata

    // Consiguiendo metadata
    responseObj.protocolo = 0; // Protocolo 0 es get
    sockets->sendS(responseObj.getJson(), "base");  // Pido la metadata a la base de datos
    response = nlohmann::json(sockets->receiveS("base")); // La recibo
    responseObj = Metadata::jsonParse(response);
    if(responseObj.mensaje == "404"){ //Check for exceptions
        cout << "Not found\n";
        responseObj.mensaje = "La imagen no fue encontrada";
        returning = responseObj.getJson();
        message.reply(status_codes::NotFound,returning);
        return;
    }

    // Consiguiendo imagen
    sockets->sendS(response, "raid");  // Pido imagen
    response = nlohmann::json(sockets->receiveS("raid")); // La recibo
    responseObj = Metadata::jsonParse(response);
    if(responseObj.mensaje == "404"){ // Check for exceptions
        cout << "Error interno: Hay imagen en base de datos, pero no en disco.\n";
        responseObj.mensaje = "Error, hay imagen en base de datos pero no en disco.\n";
        returning = responseObj.getJson();
        message.reply(status_codes::Conflict, returning);
        return;
    }

    // Devolviendo, a este punto response tiene todo lo necesario
    ucout <<  message.to_string() << endl;
    string rep = U("WRITE YOUR OWN GET OPERATION");
    returning = response;
    message.reply(status_codes::OK,returning);
    //std::cout<<rep;
};

//
// A POST request
//
void ServerManager::handle_post(http_request message)
{
    ucout <<  message.to_string() << endl;
     message.reply(status_codes::OK,message.to_string());
    return ;
};

//
// A DELETE request
//
void ServerManager::handle_delete(http_request message)
{
    ucout <<  message.to_string() << endl;
    string rep = U("WRITE YOUR OWN DELETE OPERATION");
    message.reply(status_codes::OK,rep);
    return;
};


//
// A PUT request
//
void ServerManager::handle_put(http_request message)
{
    ucout <<  message.to_string() << endl;
    message.extract_string(true).wait();
    string pet = message.extract_string().get();
    string rep = U("WRITE YOUR OWN PUT OPERATION");
    message.reply(status_codes::OK,rep);
    return;
}
