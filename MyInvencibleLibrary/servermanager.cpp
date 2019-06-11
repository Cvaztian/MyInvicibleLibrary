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
    nlohmann::json response = nlohmann::json::parse(contenido); // Convierto a json
    Metadata responseObj = Metadata::jsonParse(response);  // Objeto metadata

    // Consiguiendo metadata
    responseObj.protocolo = 0; // Protocolo 0 es get
    sockets->sendS(responseObj.getJson(), "base");  // Pido la metadata a la base de datos
    response = nlohmann::json::parse(sockets->receiveS("base")); // La recibo
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
    response = nlohmann::json::parse(sockets->receiveS("raid")); // La recibo
    responseObj = Metadata::jsonParse(response);
    if(responseObj.mensaje == "404"){ // Check for exceptions
        cout << "Error interno: Hay imagen en base de datos, pero no en disco.\n";
        responseObj.mensaje = "Error, hay imagen en base de datos pero no en disco.\n";
        returning = responseObj.getJson();
        message.reply(status_codes::InternalError, returning);
        return;
    }

    // Devolviendo, a este punto response tiene todo lo necesario
    ucout <<  "Operacion exitosa." << endl;
    returning = response;
    message.reply(status_codes::OK,returning);
    //std::cout<<rep;
};

//
// A POST request
//
void ServerManager::handle_post(http_request message)
{
    ucout << "Operacion post\n";

    // Parsing incoming message
    string returning;
    string contenido = message.to_string();
    nlohmann::json response = nlohmann::json::parse(contenido); // Convierto a json
    Metadata responseObj = Metadata::jsonParse(response);  // Objeto metadata

    // Sobreescribiendo metadata
    responseObj.protocolo = 1; // Protocolo 1: Actualizar
    sockets->sendS(responseObj.getJson(), "base");
    response = nlohmann::json::parse(sockets->receiveS("base"));
    responseObj = Metadata::jsonParse(response);
    if(responseObj.mensaje == "404"){ //Check for exceptions
        cout << "Not found\n";
        responseObj.mensaje = "La imagen no fue encontrada";
        returning = responseObj.getJson();
        message.reply(status_codes::NotFound,returning);
        return;
    }

    if(responseObj.imagen != ""){  // Si el usuario quiere sobreescribir imagen...
        // Sobreescribe imagen
        sockets->sendS(response, "raid");
        response = nlohmann::json::parse(sockets->receiveS("base"));
        responseObj = Metadata::jsonParse(response);
        if(responseObj.mensaje == "404"){ // Check for exceptions
        cout << "Error interno: Hay imagen en base de datos, pero no en disco.\n";
        responseObj.mensaje = "Error, hay imagen en base de datos pero no en disco.\n";
        returning = responseObj.getJson();
        message.reply(status_codes::InternalError, returning);
        return;
    }
    }
    responseObj.mensaje = "Succesful";
    returning = responseObj.getJson();
    ucout <<  "Actualizacion exitosa." << endl;
    message.reply(status_codes::OK,returning);
    return ;
};

//
// A DELETE request
//
void ServerManager::handle_delete(http_request message)
{
    ucout<<"Operacion delete\n";

    // Parsing incoming message
    string returning;
    string contenido = message.to_string();
    nlohmann::json response = nlohmann::json::parse(contenido); // Convierto a json
    Metadata responseObj = Metadata::jsonParse(response);  // Objeto metadata

    // Elimina metadata
    responseObj.protocolo = 2; // Protocolo 2: Eliminar
    sockets->sendS(responseObj.getJson(), "base");
    response = nlohmann::json::parse(sockets->receiveS("base"));
    responseObj = Metadata::jsonParse(response);
    if(responseObj.mensaje == "404"){ //Check for exceptions
        cout << "Not found\n";
        responseObj.mensaje = "La imagen no fue encontrada";
        returning = responseObj.getJson();
        message.reply(status_codes::NotFound,returning);
        return;
    }

        // Elimina imagen
        sockets->sendS(response, "raid");
        response = nlohmann::json::parse(sockets->receiveS("base"));
        responseObj = Metadata::jsonParse(response);
        if(responseObj.mensaje == "404"){ // Check for exceptions
        cout << "Error interno: Hay imagen en base de datos, pero no en disco.\n";
        responseObj.mensaje = "Error, hay imagen en base de datos pero no en disco.\n";
        returning = responseObj.getJson();
        message.reply(status_codes::InternalError, returning);
        return;
    }

    responseObj.mensaje = "Succesful";
    returning = responseObj.getJson();
    ucout <<  "Eliminacion exitosa." << endl;
    message.reply(status_codes::OK,returning);
    return ;
};


//
// A PUT request
//
void ServerManager::handle_put(http_request message)
{
    ucout << "Operacion put\n";
     ucout << message.to_string();

     // Parsing mensaje entrante
    pplx::task<utility::string_t> body_json = message.extract_string();
    std::string jsonstr=utility::conversions::to_utf8string(body_json.get());

    nlohmann::json orasi = nlohmann::json::parse(jsonstr);

    cout << orasi["id"] << endl;

    string returning;
    //string contenido = message.to_string();
    nlohmann::json response = orasi; // Convierto a json
    Metadata responseObj = Metadata::jsonParse(response);  // Objeto metadata

    // Escribe metadata
    responseObj.protocolo = 3; // Protocolo 3: Crear
    sockets->sendS(responseObj.getJson(), "base");
    response = nlohmann::json::parse(sockets->receiveS("base"));
    responseObj = Metadata::jsonParse(response);
    if(responseObj.mensaje == "406"){ //Check for exceptions
        cout << "Already in database\n";
        responseObj.mensaje = "La imagen ya existe";
        returning = responseObj.getJson();
        message.reply(status_codes::NotAcceptable,returning);
        return;
    }

        // Escribe imagen
        sockets->sendS(response, "raid");
        response = nlohmann::json::parse(sockets->receiveS("base"));
        responseObj = Metadata::jsonParse(response);
        if(responseObj.mensaje == "406"){ // Check for exceptions
        cout << "Error interno: Hay imagen en disco, pero no en base de datos.\n";
        responseObj.mensaje = "Error, hay imagen en disco pero no en base de datos.\n";
        returning = responseObj.getJson();
        message.reply(status_codes::InternalError, returning);
        return;

    }
    responseObj.mensaje = "Succesful";
    returning = responseObj.getJson();
    ucout <<  "Creacion exitosa." << endl;
    message.reply(status_codes::OK,returning);
    return ;

}
