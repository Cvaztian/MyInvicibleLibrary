#include "clientmanager.h"

char* prb;
int tam;
vector<char> img;

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
    json rsponse = "";
    while(response != "off"){
        cout<< "Esperando peticion...\n";
        response = sockets->specialReceive();
        cout<< "Peticion recibida\n";
        Metadata metadataObj;
        try {
            rsponse =  json::parse(response);
            metadataObj = Metadata::jsonParse(rsponse); // Se cae cuando se desconecta el server
        }catch (...){
            cout <<"Señal del servidor perdida, apagando...";
            if(tipo == "base"){
                response = "off";
                delete baseDatos;
            }
            return;
        }
        switch(metadataObj.protocolo){
        case 0:
            cout<<"Protocolo 0: Get\n"<<flush;
            Get(rsponse);
            break;
        case 1:
            cout << "Protocolo 1: Actualizar\n"<<flush;
            actualizar(rsponse);
            break;
        case 2:
            cout << "Protocolo 2: Eliminar\n"<<flush;
            eliminar(rsponse);
            break;
        case 3:
            cout << "Protocolo 3: Crear\n"<<flush;
            crear(rsponse);
            break;
        case 4:
            cout<< "Protocolo 4: Get All\n" << flush;
            GetAll(rsponse);
            break;
        }
    }
}

json ClientManager::Get(json metadata)
{
    Metadata metaObj = Metadata::jsonParse(metadata);
    json response;
    //Metadata metaObj = Metadata::jsonParse(metadata);
    if(tipo == "base"){
        response = baseDatos->Select(metaObj.galeria, metaObj.nombre).dump();  // ejecuta el metodo select y lo guarda en response
        if(response == "404"){ // Si no se encontro la imagen
            metaObj.mensaje = "404";  // Reporta 404
            response = metaObj.getJson().dump();
        }
        sockets->sendS(response);  // Envia respuesta al server
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
        sockets->specialSend(response.dump());
    }
    return response;

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
        response = metaObj.getJson().dump();
        sockets->sendS(response);
    }else{
        // En raid no hay actualizar
    }

}

void ClientManager::eliminar(json metadata)
{
    json response = Metadata::getEmptyJson();
    string success;
    success = "Exito";
    Metadata metaObj = Metadata::jsonParse(metadata);
    if(tipo == "base"){
        success = baseDatos->Delete(metaObj.galeria, metaObj.nombre);
        if(success=="404"){
            metaObj.mensaje = "404";
        }else{
            metaObj.id = std::atoi(success.c_str());
        }
        response = metaObj.getJson();
        sockets->sendS(response.dump());
    }else{
        string directorio = "../RAID/";
        for(int i = 1;i<5;i++) {
            for (int j = 1; j < 4; j++) {
                const char *eliminar = (directorio +to_string(i)+ "/"+to_string(metaObj.id) + "." + to_string(j)).c_str();
                remove(eliminar);
                string eliminarP = (directorio + to_string(i)+"/"+to_string(metaObj.id) + ".paridad");
                cout << "Ay la par "<<eliminarP;
                remove(eliminarP.c_str());
            }
        }
        sockets->sendS(Metadata::getEmptyJson().dump());
    }

}

void ClientManager::crear(json metadata)
{
     json response = {};
     //const char* sendd = metadata.dump().c_str();
     string success = "406";
     Metadata metaObj;
    if(tipo == "base"){
        metaObj  = Metadata::jsonParseFile(metadata);
        success = baseDatos->Insert(metadata);
        if(success == "406"){
            metaObj.mensaje = "406";
        }else{
            metaObj.id = std::atoi(success.c_str());
        }
        response = metaObj.getJson();
        sockets->sendS(response.dump());
    }else{
        metaObj = Metadata::jsonParse(metadata);
        char c[metaObj.imagen.size()];
        std::copy(metaObj.imagen.begin(), metaObj.imagen.end(), c);
        RAID::crear_archivo(to_string(metaObj.id), c, metaObj.imagen.size());
        sockets->sendS(metaObj.getJsonFile().dump());
    }

}

json ClientManager::GetAll(json metadata) {
    json respuesta = baseDatos->SelectAll(metadata["galeria"]);  // Debe ser un json con array de jsons
    return nlohmann::json();
}
