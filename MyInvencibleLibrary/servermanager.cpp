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
/* Antes: Llega json, llamo a base de datos, recibo una sola metadata, luego llamo a RAID
 * para adjuntar la imagen a la metadata que recibi y devuelvo al cliente esta respuesta.
 * Ahora: LLega sintaxis SQL, interpreto, llamo a getAll de base de datos con parametro galeria
 * recibo un json con un array de jsons, todos en su forma de string, devuelvo este array al
 * cliente.
 * Llega ID de imagen, llamo a RAID para conseguir la imagen de ese ID, me retorna la imagen
 * y devuelvo al cliente.*/
void ServerManager::handle_get(http_request message)
{
    ucout << "Operacion get\n";

    // Parsing incoming message
    string returning;

    pplx::task<utility::string_t> body_json = message.extract_string();

    string contenido = body_json.get();
    string cs;

    if(!is_number(contenido)){
        pair<string,string> interpretacion = Interprete::Interpretar(contenido);
        string galeria = interpretacion.first;
        string cond = interpretacion.second;
        string response;

    // Consiguiendo metadata
    Metadata responseObj = Metadata();
    responseObj.protocolo = 4; // Protocolo 4 es get all
    responseObj.galeria = galeria;
    sockets->specialSend(responseObj.getJson().dump(), "base");  // Pido las metadatas a la base de datos
    cs = sockets->receiveS("base");



    if(!cond.empty()){  // si hay condiciones
        std::vector<nlohmann::json> jsnVecCnd = Interprete::Cond(nlohmann::json::parse(cs), cond);
        nlohmann::json jsnCnd = {{"array",jsnVecCnd}};
        cs = jsnCnd.dump();
    }


    }else{
        // Consiguiendo imagen
        Metadata responseObj = Metadata();
        responseObj.protocolo = 0;
        responseObj.id = std::atoi(contenido.c_str());
        sockets->specialSend(responseObj.getJson().dump(), "raid");  // Pido imagen
        cs = sockets->specialReceive("raid");
        if(responseObj.mensaje == "404") { // Check for exceptions
            cout << "Error interno: Hay imagen en base de datos, pero no en disco.\n";
            responseObj.mensaje = "Error, hay imagen en base de datos pero no en disco.\n";
            returning = responseObj.getJson().dump();
            message.reply(status_codes::InternalError, returning);
            return;
        }
    }


    //response = nlohmann::json::parse(cs); // A este punto se tiene un json con array de jsons
    /*
    responseObj = Metadata::jsonParseFile(response);
    if(responseObj.mensaje == "404"){ //Check for exceptions
        cout << "Not found\n";
        responseObj.mensaje = "La imagen no fue encontrada";
        returning = responseObj.getJson().dump();
        message.reply(status_codes::NotFound,returning);
        return;
    }




    */

    // Devolviendo, a este punto response tiene todo lo necesario
    ucout <<  "Operacion exitosa." << endl;
    message.reply(status_codes::OK,cs);
    //std::cout<<rep;
};

//
// A POST request
//
void ServerManager::handle_post(http_request message)
{
    ucout << "Operacion post\n";

    // Parsing incoming message
    pplx::task<utility::string_t> body_json = message.extract_string();
    string returning;
    string contenido = body_json.get();
    nlohmann::json response = nlohmann::json::parse(contenido); // Convierto a json
    Metadata responseObj = Metadata::jsonParse(response);  // Objeto metadata

    // Sobreescribiendo metadata
    responseObj.protocolo = 1; // Protocolo 1: Actualizar
    sockets->specialSend(responseObj.getJson().dump(), "base");
    response = nlohmann::json::parse(sockets->receiveS("base"));
    responseObj = Metadata::jsonParseFile(response);
    if(responseObj.mensaje == "404"){ //Check for exceptions
        cout << "Not found\n";
        responseObj.mensaje = "La imagen no fue encontrada";
        returning = responseObj.getJson().dump();
        message.reply(status_codes::NotFound,returning);
        return;
    }
    responseObj.mensaje = "Succesful";
    returning = responseObj.getJson().dump();
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
    pplx::task<utility::string_t> body_json = message.extract_string();
    string returning;
    string contenido = body_json.get();
    nlohmann::json response = nlohmann::json::parse(contenido); // Convierto a json
    Metadata responseObj = Metadata::jsonParse(response);  // Objeto metadata

    // Elimina metadata
    responseObj.protocolo = 2; // Protocolo 2: Eliminar
    sockets->specialSend(responseObj.getJson().dump(), "base");
    string ayy = sockets->receiveS("base");
    response = nlohmann::json::parse(ayy);
    cout << response["galeria"];
    responseObj = Metadata::jsonParse(response);
    if(responseObj.mensaje == "404"){ //Check for exceptions
        cout << "Not found\n";
        responseObj.mensaje = "La imagen no fue encontrada";
        returning = responseObj.getJson().dump();
        message.reply(status_codes::NotFound,returning);
        return;
    }
    response["protocolo"] = 2;
        // Elimina imagen
        sockets->specialSend(response.dump(), "raid");
        response = nlohmann::json::parse(sockets->receiveS("raid"));
        responseObj = Metadata::jsonParse(response);
        if(responseObj.mensaje == "404"){ // Check for exceptions
        cout << "Error interno: Hay imagen en base de datos, pero no en disco.\n";
        responseObj.mensaje = "Error, hay imagen en base de datos pero no en disco.\n";
        returning = responseObj.getJson().dump();
        message.reply(status_codes::InternalError, returning);
        return;
    }

    responseObj.mensaje = "Succesful";
    returning = responseObj.getJson().dump();
    ucout <<  "Eliminacion exitosa." << endl;
    message.reply(status_codes::OK,returning);
    return;
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
    string enviar = responseObj.getJson().dump();
    nlohmann::json jsEnviar = responseObj.getJsonFile();
    jsEnviar["protocolo"]=3;
    sockets->specialSend(jsEnviar.dump(), "base");
    response = nlohmann::json::parse(sockets->receiveS("base"));
    responseObj = Metadata::jsonParse(response);
    if(responseObj.mensaje == "406"){ //Check for exceptions
        cout << "Already in database\n";
        responseObj.mensaje = "La imagen ya existe";
        returning = responseObj.getJson().dump();
        message.reply(status_codes::NotAcceptable,returning);
        return;
    }

        // Escribe imagen
        response["protocolo"] = 3;
    response["imagen"] = orasi["imagen"];
    string ayj = response.dump();
        sockets->specialSend(ayj, "raid");
        response = nlohmann::json::parse(sockets->receiveS("raid"));
        responseObj = Metadata::jsonParse(response);
        if(responseObj.mensaje == "406"){ // Check for exceptions
        cout << "Error interno: Hay imagen en disco, pero no en base de datos.\n";
        responseObj.mensaje = "Error, hay imagen en disco pero no en base de datos.\n";
        returning = responseObj.getJson().dump();
        message.reply(status_codes::InternalError, returning);
        return;

    }
    responseObj.mensaje = "Succesful";
    returning = responseObj.getJson().dump();
    ucout <<  "Creacion exitosa." << endl;
    message.reply(status_codes::OK,returning);
    return ;

}

bool ServerManager::is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}