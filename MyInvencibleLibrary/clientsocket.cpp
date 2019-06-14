#include "clientsocket.h"

ClientSocket::ClientSocket(string tipo)
{
    if(tipo == "base" || tipo == "raid"){
        this->tipo=tipo;
        // Inicializando la conexion con server
    sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
    }
    receiveS();
    sendS(tipo);

    }else{
        cout<<"Error de tipo";
    }
}

void ClientSocket::sendS(string mensaje)
{
    send(sock, mensaje.c_str(), strlen(mensaje.c_str()), 0);
}

string ClientSocket::receiveS()
{
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    return buffer;
}

std::string ClientSocket::specialReceive() {
    // Implementar handshake
    string asa =receiveS();
    int length = std::atoi(asa.c_str());  // Primero recibe el tamanno
    char buffer[length+2];
    for(int i = 0; i<length+2;i++){  // Inicializa el buffer en 0s
        buffer[i] = 0;
    }
    read(sock, buffer, length);
    string bfs = buffer;
    while(length != bfs.size()){
        //sendS("false");  // Envia al remisor la sennal de que se recibio mal el mensaje
        //free(buffer);   // Elimina el buffer pasado
        char buffer[length+2];  // Inicia un nuevo buffer para guardar el nuevo mensaje
        for(int i = 0; i<length+2;i++){  // Inicializa el buffer en 0s
             buffer[i] = 0;
        }
        read(sock, buffer, length);  // Lee de nuevo
        bfs += buffer;
    }
    //sendS("true");
    return bfs;  // Deuvelve el mensaje cuando se recibe bien
}

void ClientSocket::specialSend(string mensaje) {
    sendS(to_string(mensaje.size()));
    string ver = receiveS();
    while(ver == "false"){  // Si se recibe mal
        sendS(mensaje);  // Manda el mensaje de nuevo
        ver = receiveS();  // Espera sennal
    }
}
