#ifndef METADATA_H
#define METADATA_H
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;
/** @brief Clase encargada de guardar datos de MetadataDB
Esta clase tiene la finalidad de ser usada como un contenedor para JSON, es de las
pocas clases que estan habilitadas para convertirse en JSOn y viceversa. Sus
atributos son publicos para facilitar el parsing utilizando la libreria Json
for modern c++. Esta clase tiene todos los atributos que es necesario que un entry
de la base de datos tenga: id, nombre, autor, creacion, size, descripcion y un
atributo adicional para guardar la imagen, de ser necesario por el cliente.
@author Elorim
@date 02/06/2019
*/
class Metadata
{
public:
    // Metodos
    /**
      Default constructor
*/
    Metadata();

    /**
      Este metodo toma los atributos de la clase actual y los convierte en un json
      @returns Json que representa los datos de esta clase.
      @author Elorim
*/
    json getJson();

    /**
      Este metodo estatico puede llamarse de cualquier lugar y retorna un objeto
      Metadata a partir de un json dado.
      @param jsonToParse Json con los datos necesarios para crear el objeto
      @returns Objeto Metadata con los datos que tiene el json que se pasa como parametro
      @author Elorim
*/
    static Metadata jsonParse(json jsonToParse);
    // Atributos
   string nombre; /** El nombre de la imagen*/
   string galeria;  /** La galeria a la que pertenece la imagen*/
   int id;  /** El id para ir a buscar la imagen a los discos virtuales*/
   string autor; /** El autor de la imagen*/
   int size;  /** El tamaño de la imagen en bytes*/
   string descripcion;  /** La descripcion que se le da a la imagen*/
   string year;  /** El año de creacion de la imagen*/
   string imagen;  /** La imagen en binario*/

}

#endif // METADATA_H
