#include "metadata.h"

Metadata::Metadata()
{

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
    newObj.nombre = jsonToParse["nombre"];
    newObj.galeria = jsonToParse["galeria"];
    newObj.id = jsonToParse["id"];
    newObj.autor = jsonToParse["autor"];
    newObj.size = jsonToParse["size"];
    newObj.descripcion = jsonToParse["descripcion"];
    newObj.year = jsonToParse["year"];
    newObj.imagen = jsonToParse["imagen"];
    newObj.protocolo = jsonToParse["protocolo"];
    newObj.mensaje = jsonToParse["mensaje"];
    return newObj;
}
