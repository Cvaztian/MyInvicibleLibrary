
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
json j = {};
//cliente.Get();
//a.write("../image.jpg","1");
//a.read("chunks");
}