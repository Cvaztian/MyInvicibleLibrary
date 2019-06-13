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
    int length = 0;
    string bfs = "1";
    char buffer[1000000] = {0};
    while(length != bfs.size()){
        length = std::atoi(receiveS().c_str());  // Lo primero que recibe es un
        free(buffer);
        char buffer[1000000] = {0};
        read(sock, buffer, 1000000);
        bfs = buffer;
        if(length == bfs.size()){
            sendS("true");
        }else{
            sendS("false");
        }
    }
    return bfs;
}

void ClientSocket::specialSend(string mensaje) {
    string ver = "false";
    while(ver == "false"){
        sendS(to_string(mensaje.size()));
        sendS(mensaje);
        ver = receiveS();
    }
}
