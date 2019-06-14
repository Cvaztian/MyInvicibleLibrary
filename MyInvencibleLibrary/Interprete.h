//
// Created by kevin on 14/06/19.
//

#ifndef MYINVENCIBLELIBRARY_INTERPRETE_H
#define MYINVENCIBLELIBRARY_INTERPRETE_H

/** @brief Clase encargada de interpretar sintaxis SQL.
 *
 * Parte del proyecto requiere interpretar la sintaxis de SQL, esta clase es la encargada al respecto.
 * La sintaxis SQL esta compuesta por las siguientes palabras clave:
 *
 * SELECT, INSERT, UPDATE, DELETE, FROM, INTO, SET, WHERE, DELETE, VALUES. Se implementan de la siguiente manera:
 *
 * INSERT INTO table (col1, col2, ...) VALUES (val1, val2, ...): Esta instruccion crea un json en el que coln = valn
 * y galeria = table. Seguidamente realiza una llamada al metodo PUT con el json especificado. Si algun valor no esta puesto
 * se dejara en blanco, a menos que sea el nombre o la imagen.
 *
 * DELETE FROM table WHERE cond: Primeramente realiza un SELECT table. Crea un json con el nombre (tomado del SELECT),
 * y galeria = table y realiza una llamada al servicio DELETE del REST por cada json creado de esta manera. Si no se especifica
 * condicion se eliminan todos los espacios de la tabla. (galeria)
 *
 * UPDATE table SET col1=val1, col2=val2, ... WHERE cond: Primeramente realiza un SELECT table. Obtiene los json de la llamada a SELECT
 * y modifica solo los campos col1, col2, ... especificados, los cambia por val1, val2, etc. Seguidamente por cada json
 * modificado asi realiza una llamada a UPDATE. Si no hay condicion se actualizan todos los campos de la tabla (galeria)
 *
 * SELECT FROM table WHERE cond : El metodo mas complejo de todos. Requiere modificar como se maneja el get de la base
 * de datos, el paquete que recibe el server desde la base de datos, el paquete que recibe el server desde el cliente,
 * el paquete que recibe el cliente desde el server, el metodo GET del server REST.
 *
 * Manejo del get de la base de datos: Se necesita un getUnico y un getAll. getAll llama a getU por cada file dentro de
 * la galeria y va creando su json de respuesta a partir de cada llamada.
 *
 * Paquete que recibe el server desde la base de datos: Este paquete ahora sera un json de una clase que tenga como atributos:
 * un array de json (cada uno representando una clase Metadata).
 *
 * El paquete que recibe el server desde el cliente: Este cambio sera solo para el metodo GET, ahora debera recibir un string
 * que va a representar la instruccion de SQL a interpretar o el ID de una imagen. Se va a necesitar un metodo para diferenciar
 * entre uno y otro.
 *
 * El paquete que recibe el cliente desde el server: Este cambio sera solo para el metodo GET, un json que tenga un array de
 * json. El cliente debe encargarse de desplegar cada json y verificar si en el json viene una imagen. Cabe destacar
 * que en el caso de venir una imagen en el json, solo va a ser un json dentro del array y va a tener el mensaje:imagen.
 *
 * El metodo GET del server REST: Va a tener que ser adaptado para recibir un string y devolver un json con array de jsons.
 * En caso de que se le pida una imagen, llamar al get de RAID y en caso de que se le pase una instruccion SQL la debe
 * interpretar. Tomar en cuenta que la aplicacion de la condicion se va a llevar a acabo en el server.
 *
 * Ahora la aplicacion del metodo SELECT será: Al server se le pasa la instruccion mediante su metodo GET de REST. El
 * server le pide a la base de datos todos los campos en la galeria table. Seguidamente se encarga de filtrar mediante
 * la condicion los json y retorna un array de jsons por cada json que paso la condicion. Si la condicion no es especificada
 * retorna todos los jsons de la galeria.
 * */
class Interprete{
    
};

#endif //MYINVENCIBLELIBRARY_INTERPRETE_H
