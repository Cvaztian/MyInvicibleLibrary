
#include <fstream>
#include <iostream>
#include <cstring>
#include "RAID.h"
#include "nlohmann/json.hpp"
#include "clientmanager.h"

using namespace std;
using namespace nlohmann;

int main( int argc, char* argv[] )
{
RAID a;
ClientManager cliente = ClientManager("raid");
    json j = {
            {"nombre","ala"},
            {"galeria","misrecuerdos"},
            {"autor","Kevin"},
            {"descripcion","Quesuenno!!"},
            {"year","2019"},
            {"id",1},
            {"size",-1},
            {"imagen",""},
            {"mensaje",""},
            {"protocolo",-1}
    };
cliente.Get(j);
j["id"] = 2;
cliente.crear(j);
//a.write("../image.jpg","1");
//a.read("chunks");
}