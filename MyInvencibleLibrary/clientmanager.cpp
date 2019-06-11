#include "clientmanager.h"

ClientManager::ClientManager(string tipo)
{
    //sockets = new ClientSocket(tipo);
    this->tipo = tipo;
}

void ClientManager::mainloop()
{
    string response ="";
    while(response != "off"){
        response = sockets->receiveS();
        Metadata metadataObj = Metadata::jsonParse(response);
        switch(metadataObj.protocolo){
        case 0:
            Get(response);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        }
    }
}

json ClientManager::Get(json metadata)
{
    Metadata metaObj = Metadata::jsonParse(metadata);
    json response;
    if(tipo == "base"){
    }else{
        char* a = (char*)to_string(metaObj.id).c_str();   // Peligro
        cout << a <<endl;
        cout << RAID::read(a) << endl;
    }
    return response;
}

json ClientManager::actualizar(json metadata)
{
        json response;
    if(tipo == "base"){
    }else{
    }

    return response;
}

json ClientManager::eliminar(json metadata)
{
        json response;
    if(tipo == "base"){
    }else{
    }

    return response;
}

json ClientManager::crear(json metadata)
{
        json response;
    if(tipo == "base"){
    }else{
    }

    return response;
}
