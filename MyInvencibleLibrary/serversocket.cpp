#include "serversocket.h"

ServerSocket::ServerSocket()
{
    // Inicializando
    int server_fd, new_socket, valread;
    raid = 0;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Acepta un socket, se pega
    std::cout<< "Esperando conexion de modulos\n"<<std::flush;
    int i =1;  // i=0
    while(i!=2){
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                    (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    send(new_socket, "who", strlen("who"), 0);
    read(new_socket, buffer, 1024);
    if(strncmp(buffer, "raid",4)==0){
        raid = new_socket;
        std::cout << "Modulo de raid conectado\n"<<std::flush;
        i++;
    }else if(strncmp(buffer, "base",4)==0){
        base = new_socket;
        std::cout << "Modulo de base de datos conectado\n"<<std::flush;
        i++;
    }else{
        std::cout<<"Respuesta indefinida, conexion de modulo fallida, reintentando";
        i++; // Comentar
    }
    }
    std::cout<<"Todos los modulos conectados\n";
}

void ServerSocket::sendS(std::string mensaje, std::string socket)
{
    int socketnum = 0;
    if(socket == "base"){
        socketnum = base;
    }else if (socket == "raid") {
        socketnum = raid;
    }else{
        std::cout << "Error";
        return;
    }
    const char* mensajeChar = mensaje.c_str();
    send(socketnum, mensajeChar, strlen(mensajeChar),0);
}

std::string ServerSocket::receiveS(std::string socket)
{
    int socketnum =0;
    if(socket == "raid"){
        socketnum = raid;
    }else if(socket == "base"){
        socketnum = base;
    }
    char buffer[1024] = {0};
    read(socketnum, buffer, 1024);
    return buffer;
}
