
#include <fstream>
#include <iostream>
#include <cstring>
#include "RAID.h"
#include "nlohmann/json.hpp"
#include "clientmanager.h"

using namespace std;

int main( int argc, char* argv[] )
{
RAID a;
a.write("../image.jpg","1");
ClientManager cliente = ClientManager("raid");
vector<char> arr = {'a','l','c'};
    nlohmann::json j = {
            {"nombre","ala"},
            {"galeria","misrecuerdos"},
            {"autor","Kevin"},
            {"descripcion","Quesuenno!!"},
            {"year","2019"},
            {"id",1},
            {"size",-1},
            {"imagen",arr},
            {"mensaje",""},
            {"protocolo",-1}
    };
 //   cout << j["id"] <<endl;
//j = cliente.Get(j);
//j["id"] = 4;
//cliente.eliminar(j);
//cout<<j.dump();
//cliente.crear(j);

//a.write("../image.jpg","1");
//a.read("chunks");
}