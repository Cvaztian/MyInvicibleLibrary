#include "clientmanager.h"

char* prb;
int tam;
vector<char> img;

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

        metaObj.imagen = vector<char>();
        img = vector<char>();
        for(int i =0; i<tam; i++){
            metaObj.imagen.push_back(prb[i]);
            img.push_back(prb[i]);
        }

        //cout << "Imagen:" << metaObj.imagen <<endl;
        metaObj.size = tam;
        /*
        ofstream prueba;
        prueba.open("../prueba", ios::out | ios::trunc | ios::binary);
        prueba.write(prb,tam);
        prueba.close();*/

        response = metaObj.getJson();
    }
    return response;
}

json ClientManager::actualizar(json metadata)
{
        json response;
    if(tipo == "base"){
    }else{
        // En raid no hay actualizar
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
        char c[metaObj.imagen.size()];
        std::copy(metaObj.imagen.begin(), metaObj.imagen.end(), c);
        RAID::crear_archivo(to_string(metaObj.id), c, tam);
    }

    return response;
}
