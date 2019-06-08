#include <iostream>

#include "include/stdafx.h"
#include "include/handler.h"
#include "servermanager.h"

std::unique_ptr<ServerManager> manager;

void on_initialize(const string_t& address)
{


    uri_builder uri(address);

    auto addr = uri.to_uri().to_string();
     manager = std::unique_ptr<ServerManager>(new ServerManager(addr));
     manager->open().wait();

    ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

    return;
}

void on_shutdown()
{
    manager->close().wait();
    return;
}

int main(){
  utility::string_t port = U("34568");

    utility::string_t address = U("http://127.0.0.1:");
    address.append(port);

    on_initialize(address);  // Inicia el server
    std::cout << "Press ENTER to exit." << std::endl;

    std::string line;
    std::getline(std::cin, line);  // No deja que muera

    on_shutdown();  // Destruye el server
    return 0;
}
