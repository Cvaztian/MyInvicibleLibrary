#include "clientmanager.h"

ClientManager::ClientManager(string tipo)
{
    cout << "Intentando conectar con servidor...\n";
    sockets = new ClientSocket(tipo);
    cout << "Conectado\n";
    this->tipo = tipo;
    if(tipo == "raid"){

    }else if(tipo == "base"){
        baseDatos = new DBManager();
    }else{
        cout<<"No wey";
    }
    mainloop();

}

void ClientManager::mainloop()
{
    string response ="";
    while(response != "off"){
        cout<< "Esperando peticion...\n";
        response = sockets->receiveS();
        cout<< "Peticion recibida\n";
        cout<<response<<flush;
        json rsponse = json::parse(response);
        Metadata metadataObj = Metadata::jsonParse(rsponse);
        switch(metadataObj.protocolo){
        case 0:
            cout<<"Protocolo 0: Get\n";
            Get(rsponse);
            break;
        case 1:
            cout << "Protocolo 1: Actualizar\n";
            actualizar(rsponse);
            break;
        case 2:
            cout << "Protocolo 2: Eliminar\n";
            eliminar(rsponse);
            break;
        case 3:
            cout << "Protocolo 3: Crear\n";
            crear(rsponse);
            break;
        }
    }
}

void ClientManager::Get(json metadata)
{
    json response;
    Metadata metaObj = Metadata::jsonParse(metadata);
    if(tipo == "base"){
        response = baseDatos->Select(metaObj.galeria, metaObj.nombre);  // ejecuta el metodo select y lo guarda en response
        if(response == "404"){ // Si no se encontro la imagen
            metaObj.mensaje = "404";  // Reporta 404
            response = metaObj.getJson();
        }
        sockets->sendS(response);  // Envia respuesta al server
    }else{
    }

}

void ClientManager::actualizar(json metadata)
{
    json response;
    string success;
    Metadata metaObj = Metadata::jsonParse(metadata);
    if(tipo == "base"){
        success = baseDatos->Update(metadata);
        if(success == "404"){
            metaObj.mensaje = "404";
        }
        response = metaObj.getJson();
        sockets->sendS(response);
    }else{
    }

}

void ClientManager::eliminar(json metadata)
{
    json response;
    string success;
    Metadata metaObj = Metadata::jsonParse(metadata);
    if(tipo == "base"){
        success = baseDatos->Delete(metaObj.galeria, metaObj.nombre);
        if(success=="404"){
            metaObj.mensaje = "404";
        }
        response = metaObj.getJson();
        sockets->sendS(response);
    }else{
    }

}

void ClientManager::crear(json metadata)
{
     json response;
     const char* sendd = metadata.dump().c_str();
     string success;
     Metadata metaObj = Metadata::jsonParse(metadata);
    if(tipo == "base"){
        success = baseDatos->Insert(metadata);
        if(success == "406"){
            metaObj.mensaje = "406";
        }
        response = metaObj.getJson();
        sockets->sendS(response);
    }else{
    }

}
