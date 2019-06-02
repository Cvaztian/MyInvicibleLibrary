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
       {"imagen", imagen}
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
    return newObj;
}
