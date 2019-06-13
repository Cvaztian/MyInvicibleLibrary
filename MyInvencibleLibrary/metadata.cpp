#include "metadata.h"

Metadata::Metadata()
{
    imagen = vector<char>();
}

json Metadata::getJson()
{
   json result;
   result = {
       {"id", id},
       {"nombre",nombre},
       {"galeria",galeria},
       {"autor",autor},
       {"size",size},
       {"descripcion",descripcion},
       {"year",year},
       {"imagen", imagen},
       {"mensaje", mensaje},
       {"protocolo", protocolo}
   };
   return result;
}

Metadata Metadata::jsonParse(json jsonToParse)
{
    Metadata newObj = Metadata();
    newObj.galeria = jsonToParse["galeria"];
    newObj.id = jsonToParse["id"];
    newObj.autor = jsonToParse["autor"];
    newObj.size = jsonToParse["size"];
    newObj.descripcion = jsonToParse["descripcion"];
    newObj.year = jsonToParse["year"];
    vector<char> im = jsonToParse["imagen"];
    //const char* img = im.c_str();
    newObj.imagen = vector<char>();
    for(int i =0 ; i<im.size(); i++){
        newObj.imagen.push_back(im[i]);
    }
    newObj.protocolo = jsonToParse["protocolo"];
    newObj.mensaje = jsonToParse["mensaje"];
    newObj.nombre = jsonToParse["nombre"];
    return newObj;
}

nlohmann::json Metadata::getJsonFile() {
    json result;
   result = {
       {"id", id},
       {"nombre",nombre},
       {"galeria",galeria},
       {"autor",autor},
       {"size",size},
       {"descripcion",descripcion},
       {"year",year},
       {"imagen", vector<char>()},
       {"mensaje", mensaje},
       {"protocolo", -1}
   };
   return result;
}

Metadata Metadata::jsonParseFile(nlohmann::json jsonToParse) {
    Metadata newObj = Metadata();
    newObj.galeria = jsonToParse["galeria"];
    newObj.id = jsonToParse["id"];
    newObj.autor = jsonToParse["autor"];
    newObj.size = jsonToParse["size"];
    newObj.descripcion = jsonToParse["descripcion"];
    newObj.year = jsonToParse["year"];
    vector<char> im = vector<char>();
    newObj.mensaje = jsonToParse["mensaje"];
    newObj.protocolo = -1;
    newObj.nombre = jsonToParse["nombre"];
    return newObj;
}

nlohmann::json Metadata::getEmptyJson() {
        json result;

   result = {
       {"id", -1},
       {"nombre",""},
       {"galeria",""},
       {"autor",""},
       {"size",-1},
       {"descripcion",""},
       {"year",""},
       {"imagen", vector<char>()},
       {"mensaje", ""},
       {"protocolo", -1}
   };
   return result;
}
