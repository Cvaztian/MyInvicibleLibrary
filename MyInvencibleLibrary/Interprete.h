//
// Created by kevin on 14/06/19.
//

#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include "LinkedList.h"
#include "nlohmann/json.hpp"
#include <vector>

using namespace std;
using namespace nlohmann;
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
 * SELECT data1, data2, ... FROM table WHERE cond : El metodo mas complejo de todos. Requiere modificar como se maneja el get de la base
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
 *
 * @date 13/06/2019
 * @author Elorim
 * */
class Interprete{
public:

    /** @brief Interpreta una sintaxis SQL.
     *
     * Este metodo se encarga de interpretar una sintaxis SQL y de llamar a los metodos
     * necesarios para llevar a cabo la solicitud. Retorna un string o un json en su forma
     * de string, es trabajo del que llama distinguir entre ambos.
     * @param syntax Sintaxis que se quiere interpretar
     * @return String que contiene un json o un mensaje.
     * @throws Excepcion si la sintaxis no tiene el formato definido de SQL
     * */
    static pair<string,string> Interpretar(string syntax){
        string method = "";
        int i = 0;
        for(char& c : syntax){
            cout << c;
            if(c != 32){  // 32 es espacio
                method += c;
            }else if(c == 32){
                transform(method.begin(), method.end(), method.begin(), ::tolower);  // Pasa lo que sea que tenga method a lower case
                if(strncmp("select", method.c_str(), 6) ==0){
                    return Select(syntax);
                }else if(strncmp("insert", method.c_str(), 6) == 0){
                    return Insert(syntax);
                }else if(strncmp("update", method.c_str(), 6) == 0){
                    // Update
                }else if(strncmp("delete", method.c_str(), 6) == 0){
                    // Delete
                }else {
                    throw "SQL: Error de sintaxis.";
                }
            }
        }
    };

    /** @brief Aplica las condiciones a las lineas
     * Este metodo interpreta condiciones y las aplica al array de jsons que se le pasa.
     * @param lineas Json con un array de jsons
     * @param cond String de condiciones
     * @returns array de json condicionados
     * @throws Error si hay mala sintaxis
     * */
    static std::vector<nlohmann::json> Cond(nlohmann::json lineas, string cond){
        std::vector<nlohmann::json> lineasV = lineas["array"];  // Vector con las lineas
        if(cond.empty()){
            return lineas;
        }
        std::vector<nlohmann::json> lineasN = std::vector<nlohmann::json>();
        // Interpretar
        /* Plan:
         * Los comparadores son: =, <, >, <=, >=, !=.
         * Una estructura de casos van a convertir del comparador en string al operador que le
         * corresponda.
         * To do lo que esta antes de un comparador, le quito los espacios es la columna que voy
         * a comparar, entonces hago json[col].
         * Inmediatamente despues del comparador, si hay un espacio lo ignoro.
         * Lo que haya despues del comparador y antes del siguiente espacio (exceptuando el
         * primero) sera el valor, entonces hago json[col] comparador valor.
         * Comparo el valor que tiene la columna contra el solicitado, pasando ambos a strings.
         * Un AND se puede implementar con recursividad.
         * Un OR se puede implementar sumando los resultados.
         * */
        string comp;
        string col;
        string val;
        string AndOr;
        string newCond;
        bool colWarningB = false;
        bool ignore = true;
        const char* condC = cond.c_str();
        for(int i=0;i<cond.size();i++){  // Evalua la columna
            for(i;i<cond.size();i++){
                if(condC[i]!=32){
                    break;
                }else if(!ignore){
                    break;
                }
            }
            ignore = false;
            if(!colWarningB && !(condC[i] == 32 || condC[i]==33 || condC[i]==60 || condC[i] == 62 || condC[i] == 61)){
                col += condC[i];
            }else if(condC[i] == 32){
                colWarningB = true;
            }else if(colWarningB && !(condC[i]==33 || condC[i]==60 || condC[i] == 62 || condC[i] == 61 || condC[i]==73)){
                throw "SQL: error de syntaxis";
            }else{  // Evalua los comparadores
                //comp += condC[i];
                colWarningB = false;
                for(i;i<cond.size();i++){
                    if(condC[i] == 73 || condC[i] == 78 ||condC[i]==33 || condC[i]==60 || condC[i] == 62 || condC[i] == 61){
                       comp += condC[i];
                       if(comp.size() > 2){
                           throw "SQL:Error de sintaxis";
                       }else if(comp.size() == 2) {
                           if( comp == "in" || comp == "IN"){
                               i+=2;
                           }else if(!(comp == "==" || comp == "<=" || comp == ">=" || comp == "!=")){
                               throw  "SQL: Error de sintaxis";
                           }
                       }
                    }else if(!colWarningB && condC[i] == 32){
                        colWarningB = true;
                    }else if(colWarningB && (condC[i] ==32 || condC[i]==33 || condC[i]==60 || condC[i] == 62 || condC[i] == 61))
                    {
                        throw "SQL: Error de sintaxis";
                    }else{  // Evalua el valor
                        //val += condC[i];
                        for(i;i<cond.size();i++){
                            if(condC[i] != 32){
                                val+=condC[i];
                            }else{  // si hay OR o AND
                                i++;
                                for(i;i<cond.size();i++){
                                    if(condC[i] != 32){
                                        AndOr += condC[i];
                                    }else{
                                        transform(AndOr.begin(), AndOr.end(), AndOr.begin(), ::tolower);
                                        if(std::strncmp(AndOr.c_str(), "or", 2)==0 || (std::strncmp(AndOr.c_str(), "and",3)==0)){
                                            i++;
                                            for(i;i<cond.size();i++){
                                                newCond += condC[i];
                                            }
                                        }else{
                                            throw "SQL: Error de sintaxis";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // A este punto ya esta interpretado
        for(int i =0;i<lineasV.size();i++){
            nlohmann::json linea = lineasV[i];
            string colVal;
            string s = typeid(linea[col]).name();
            /*
            if(strncmp(s.c_str(),"i",1)==0){
                colVal = to_string(linea[col]);
            }else{
                colVal = linea[col];
            }*/
            if(comp == "="){
                if(is_number(val)){
                    if(linea[col] == atoi(val.c_str())){
                        lineasN.push_back(lineasV[i]);
                    }
                }else if(linea[col] == val){
                    lineasN.push_back(lineasV[i]);
                }
            }else if(comp == "<"){
                if(!is_number(val)){
                    throw "SQL: syntax error";
                }else if( linea[col] < std::atoi(val.c_str())){
                    lineasN.push_back(lineasV[i]);
                }
            }else if(comp == ">"){
                if(!is_number(val)){
                    throw "SQL: syntax error";
                }else if( linea[col] > std::atoi(val.c_str())){
                    lineasN.push_back(lineasV[i]);
                }
            }else if(comp == "!="){
                if(is_number(val)){
                    if(linea[col] != atoi(val.c_str())){
                        lineasN.push_back(lineasV[i]);
                    }
                }else if(linea[col] != val){
                    lineasN.push_back(lineasV[i]);
                }
            }else if(comp == "<="){
                if(!is_number(val)){
                    throw "SQL: syntax error";
                }else if( linea[col] <= std::atoi(val.c_str())){
                    lineasN.push_back(lineasV[i]);
                }
            }else if(comp == ">="){
                if(!is_number(val)){
                    throw "SQL: syntax error";
                }else if( linea[col] >= std::atoi(val.c_str())){
                    lineasN.push_back(lineasV[i]);
                }
            }else if(comp == "into" || comp == "IN"){
                if (val.find(colVal) != std::string::npos) {  // True si colVal es substring de val
                    lineasN.push_back(lineasV[i]);
                }
            }else{
                throw "SQL: Error de sintaxis";
            }
        }
        if(AndOr == "and"){
            return Cond({{"array",lineasN}}, newCond);
        }else if(AndOr == "or"){
            std::vector<nlohmann::json> orResponse = Cond(lineas,newCond);
            lineasN.insert(std::end(lineasN), std::begin(orResponse), std::end(orResponse));
        }
        return lineasN;
    };

    /** @brief Devuelve los campos solicitados.
     *
     * Este metodo toma un json con array de jsons y una sintaxis y devuelve una lista de listas
     *  de string con la siguiente estructura:
     *
     * La primera de las listas va a contener los campos que se solicitan.
     *
     * De la segunda en adelante va a tener los valores de esos campos.
     *
     * @param lineas Json con array de jsons que tiene las metadatas completas a filtrar
     * @param syntax Sintaxis SQL
     * @returns Lista de listas de strings conteniendo los valores solicitados.
     * */
     static LinkedList<LinkedList<string>> getCampos(nlohmann::json lineas, string syntax){
         std::vector<nlohmann::json> lineasV = lineas["array"];
         LinkedList<string> columnas = LinkedList<string>();
         LinkedList<LinkedList<string>> respuesta = LinkedList<LinkedList<string>>();
         string campo;
         const char* syntaxC = syntax.c_str();
         for(int i = 6; i<syntax.size();i++){  // se va a suponer que se sabe que la primera palabra es select
             if(syntaxC[i] != 32){
                 if(syntaxC[i] == 42){  // * trae to do
                     columnas.push_back("nombre");
                     columnas.push_back("galeria");
                     columnas.push_back("size");
                     columnas.push_back("descripcion");
                     columnas.push_back("year");
                     columnas.push_back("autor");
                     break;
                 }
                 for(i;i<syntax.size();i++){
                     if(syntaxC[i] != 32){  // mientras no sea un espacio
                         if(syntaxC[i] != 44){ // Mientras no sea una coma
                             campo += syntaxC[i];
                         }else{  // cuando se encuentra coma
                             columnas.push_back(campo);
                             campo = "";
                         }
                     }else{
                         columnas.push_back(campo);// Cuando es un espacio, termina
                         campo = "";
                         break;
                     }
                 }
                 break;
             }
         }

         respuesta.push_back(columnas);

         // Interpretado
         for(int i =0;i<lineasV.size();i++){  // por cada linea
             nlohmann::json linea = lineasV[i];
             LinkedList<string> lineaL = LinkedList<string>();  // Lista de la linea
             for(int j =0;j<columnas.getSize();j++){  // Por cada columna qu ese debe mostrar
                 try {
                     lineaL.push_back(linea[columnas.getElemento(
                             j)->getData()]); // agrego a la lista de la linea el valor de columna que tiene la linea
                 }catch(...){
                     // Pasa un error si se pide una columna que no existe en la base de datos, en este caso
                     lineaL.push_back(" ");  // Mete un vacio.
                 }
             }
             respuesta.push_back(lineaL);  // Se agrega la linea a la respuesta
         }

         return respuesta;
     };
private:

    /** @brief Metodo Insert SQL.
     *
     * Se ejecuta cuando la primera palabra de la sintaxis coincide con INSERT.
     * Se encarga de mimetizar el metodo INSERT que existe en SQL.
     * La sintaxis debe ser la siguiente:
     * INSERT INTO table (col1, col2, col3, ...) VALUES (val1, val2, val3, ...)
     *
     * @param syntax Sintaxis de SQL
     * @returns json a insertar en la base de datos
     * @throws Excepcion si no hay una sintaxis adecuadada
     * */
     static pair<string, string> Insert(string syntax){

         const char* syntaxC = syntax.c_str();
         string into;
         bool warning = false;
         bool ignore = true;
         bool open = false;
         bool galB = false;
         std::vector<string> columnas = std::vector<string>();
         std::vector<string> valores = std::vector<string>();
         string val;
         string values;
         string col;
         string galeria;

         for(int i =0;i<syntax.size();i++){ // Parte del hecho que la primera palabra es INSERT
             if(syntaxC[i] != 32){
                 ignore = false;  // Ignore espacios a la izquierda
             }else if(!ignore){  // Termina la primera palabra
                 if(syntaxC[i] == 32){  // Debo estar en un espacio
                     i++;
                 }else {
                     throw "SQL:Syntax error";
                 }
                 ignore = true;
                 for(i;i<syntax.size();i++){
                     if(syntaxC[i] != 32){
                         into += syntaxC[i];
                         ignore = false;  // Ignora espacios a la izquierda
                     }else if(!ignore){  // Termina la segunda palabra
                         transform(into.begin(), into.end(), into.begin(), ::tolower);
                         if(into == "into"){
                             if(syntaxC[i] == 32){  // Debo estar en un espacio
                                 i++;
                             }else {
                                 throw "SQL:Syntax error";
                             }
                             for(i;i<syntax.size();i++){
                                 ignore = true;
                                 if(!galB) {
                                     for (i; i < syntax.size(); i++) {
                                         if (syntaxC[i] != 32) {
                                             galeria += syntaxC[i];
                                             ignore = false;
                                         } else if (!ignore) {
                                             break;
                                         }
                                     }
                                     galB = true;
                                 }
                                 if(syntaxC[i]==32){
                                     continue;  // Ignore los espacios
                                 }else if(!open){  // Si no esta abierto
                                     if(syntaxC[i]==40){  // Abre (
                                         open = true;
                                     }else{  // si no es )
                                         throw "SQL:Syntax error";
                                     }
                                 }else if(open){  // Si ya esta abierto
                                     if(syntaxC[i]==41){  // Cierra ), busca nuevos valores
                                         columnas.push_back(col);
                                         open = false;
                                         i++;
                                         ignore = true;  // Ignore los primeros espacios
                                         if(syntaxC[i]!=32){ // Debo estar en espacio
                                             throw "SQL:Syntax error";
                                         }
                                         for(i;i<syntax.size();i++){
                                             if(syntaxC[i]!=32){
                                                 values += syntaxC[i];
                                                 ignore = false;
                                             }else if(!ignore){
                                                 transform(values.begin(), values.end(), values.begin(), ::tolower);
                                                 if(values == "values"){  // Si encuentra keyword values
                                                     for(i;i<syntax.size();i++){
                                                         if(syntaxC[i]==32){  //  Ignora espacios
                                                             continue;
                                                         }else if(!open){
                                                             if(syntaxC[i] == 40){ // Abre (
                                                                 open = true;
                                                             }else{  // No es ( ni espacio
                                                                 throw "SQL:Syntax error";
                                                             }
                                                         }else if(open){
                                                             //valores.push_back(col);
                                                             if(syntaxC[i]==41){  // Cierra )
                                                                 valores.push_back(val);
                                                                 open = false;
                                                                 // Si despues de aqui hay cualquier cosa, menos espacios
                                                                 // es error
                                                                 i++;
                                                                 for(i;i<syntax.size();i++){
                                                                     if(syntaxC[i]!=32){  // Si no es un espacio
                                                                         throw "SQL:Syntax error";
                                                                     }
                                                                 }
                                                             }else{  // Si no es )
                                                                 if(syntaxC[i]!=44){  // Si no es coma
                                                                     val += syntaxC[i];
                                                                 }else{ // Si es coma
                                                                     valores.push_back(val);  // Inserta la columna encontrada en columnas
                                                                     val = "";
                                                                 }
                                                             }
                                                         }
                                                     }
                                                 }else{  // si no
                                                     throw "SQL:Syntax error";
                                                 }
                                             }
                                         }
                                     }else{  // Si no es )
                                         if(syntaxC[i]!=44){  // Si no es una coma
                                             col += syntaxC[i];
                                         }else{  // Si es una coma
                                             columnas.push_back(col); // Inserta la columna encontrada en columnas
                                             col = "";  // reinicia la columna encontrada
                                         }
                                     }
                                 }
                             }
                         }else{
                             throw "SQL: Syntax error";
                         }
                     }
                 }
             }
         }
         // Interpretado, armar json
         // Verificando que las columnas tengan los valores que deben existir en la tabla
         LinkedList<string> columnasPred;
         columnasPred.push_back("nombre");
         columnasPred.push_back("size");
         columnasPred.push_back("descripcion");
         columnasPred.push_back("autor");
         columnasPred.push_back("year");
         columnasPred.push_back("galeria");
         bool existe = false;
         for(string elemento:columnas){  // Para cada elemento en columnas
             for(int i=0;i<columnasPred.getSize();i++){  // Para cada elemento en columnasPred
                 if(!existe && elemento == columnasPred.getElemento(i)->getData()){ // Si elemento esta en columnasPred
                     existe = true;
                 }
             }
             if(!existe){
                 throw "Se agrego una columna que no deberia existir";
             }
         }

         // Armando mapa
         std::vector<pair<string,string>> mapa;
         pair<string, string> pareja;
         if(columnas.size() != valores.size()){  // Si hay mas de uno que del otro
             throw "SQL:Syntax error";
         }
         for(int i=0;i<columnas.size();i++){  // Para cada valor de las columnas
             mapa.push_back(pair<string,string>(columnas[i], valores[i]));
         }

         if((!findInMapa(mapa,"galeria").empty() && findInMapa(mapa,"galeria") != galeria) || (!findInMapa(mapa,"size").empty() && !is_number(findInMapa(mapa, "size")))){  // Si en el mapa hay size y no es un numero o la galeria especificada es diferente a la pasada por sintaxis
             throw "Error de galeria o size";
         }

         //Armando Json
         nlohmann::json nuevo ={
                 {"nombre",findInMapa(mapa,"nombre")},
                 {"galeria",findInMapa(mapa,"galeria")},
                 {"autor",findInMapa(mapa,"autor")},
                 {"size",1},
                 {"descripcion",findInMapa(mapa,"descripcion")},
                 {"year",findInMapa(mapa,"year")},
                 {"id",-1},
                 {"mensaje",""},
                 {"protocolo",-1}
         };
         return pair<string, string>(nuevo.dump(),"");
     }

     /** @brief Encuentra un key en un mapa
      * Metodo auxiliar que encuentra el valor de una llave en un mapa
      * (ya que C++ aparentemente no trae mapas decentes)
      * @param mapa Mapa en formato de un vector de parejas
      * @param key String llave a buscar
      * @returns devuelve el valor de la llave o vacio si no lo encuentra
      * */
     static string findInMapa(std::vector<pair<string, string>> mapa, string key){
         for(pair<string,string> pareja:mapa){
             if(pareja.first == key){
                 return pareja.second;
             }
         }
         // Si llega aca no encontro match
         return "";
     }

    /** @brief Metodo SELECT SQL.
     *
     * Este metodo se ejecuta cuando la primera palabra de la sintaxis coincide con SELECT.
     * Se encarga de mimetizar el metodo SELECT que existe en SQL.
     * @param syntax Sintaxis de SQL.
     * @returns La galeria especificada y las condiciones que tiene la sintaxis en un pair.
     * @throws Excepcion si no hay una sintaxis adecuada.
     * */
    static pair<string,string> Select(string syntax){
        bool methodB = false;
        bool fromB = false;
        bool galeriaB = false;
        bool whereB = false;
        bool condB = false;
        bool dataB = false;
        string where = "";
        string from = "";
        string galeria = "";
        string condicion = "";

        for(char&c : syntax){
            if(!methodB && !dataB && c == 32){  // Itera sin hacer nada hasta que encuentra el primer espacio
                methodB = true;  // Al encontrarlo, methodB = true
            }else if(methodB && !dataB && c == 32 ){
                dataB = true;
            }else if(dataB && methodB && !fromB && c != 32){  // Buscando FROM
                from += c;
            }else if(dataB && methodB && !fromB && c == 32){  // Segundo espacio, ya deberia existir FROM
                transform(from.begin(), from.end(), from.begin(), ::tolower);
                if(strncmp(from.c_str(), "from",4) == 0){  // Si lo que hay entre espacios es FROM
                    fromB = true;
                }else{  // Si no...
                    throw "SQL: Error de sintaxis";
                }
            }else if(dataB && methodB && fromB && !galeriaB && c !=32){ // Buscando galeria
                galeria += c;
            }else if(dataB && methodB && fromB && !galeriaB && c == 32){  // Tercer espacio, ya deberia haber galeria
                galeriaB = true;
            }else if(dataB && methodB && fromB && galeriaB && !whereB && c != 32){  // Busca where
                where += c;
            }else if(dataB && methodB && fromB && galeriaB && !whereB && c == 32){ //  Cuarto espacio, deberia haber where
                transform(where.begin(), where.end(), where.begin(), ::tolower);
                if(strncmp(where.c_str(), "where",5) == 0){  // Si lo que hay entre espacios es WHERE
                    whereB = true;
                }else{  // Si no...
                    throw "SQL: Error de sintaxis";
                }
            }else if(dataB && methodB && fromB && galeriaB && whereB && !condB){  // Busca la condicion
                condicion += c;
            }
        }
        if(from.empty() && galeria.empty()){  // Si no se especifico galeria o FROM...
            throw "SQL: Error de sintaxis";
        }
        cout << "La galeria es: " << galeria << flush;

        // A este punto existe la galeria
        return pair<string,string>(galeria,condicion);


        if(!where.empty() && !condicion.empty()){  // si hay WHERE y la condicion no es vacio
            cout << "La condicion es " << condicion;
        }
    };

    static bool is_number(const std::string& s)
    {
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
    }

};



#endif //MYINVENCIBLELIBRARY_INTERPRETE_H
