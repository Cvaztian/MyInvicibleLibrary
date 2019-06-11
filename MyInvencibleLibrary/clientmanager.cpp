#include "clientmanager.h"

char* prb;
int tam;

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
        pair<int, char*> aux = RAID::read(a);
        tam = aux.first;
        prb = aux.second;

        metaObj.size = tam;

        ofstream prueba;
        prueba.open("../prueba", ios::out | ios::trunc | ios::binary);
        prueba.write(prb,tam);
        prueba.close();
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
    Metadata metaObj = Metadata::jsonParse(metadata);
    json response;
    if(tipo == "base"){
    }else{
        RAID::crear_archivo(to_string(metaObj.id), prb, tam);
    }

    return response;
}
