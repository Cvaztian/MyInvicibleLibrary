#include "Interprete.h"
#include <string>
#include "dbmanager.h"
#include "LinkedList.h"


int main(){
    //Interprete::Interpretar("SELECT rows FROM table WHERE cond");
  //  DBManager manager = DBManager();
//    manager.SelectAll("Pruebas");


    json js = {{"galeria","ay"},
               {"descripcion","aija"},
               {"nombre","kevin"}};
    vector<json> prueba = vector<json>();
    prueba.push_back(js);
    json pruebaJ = {{"array",prueba}};
    //LinkedList<LinkedList<string>> pruebaL = Interprete::getCampos(pruebaJ, "select nombre from kk");
    //cout << pruebaL.getElemento(1)->getData().getElemento(0)->getData() <<flush;

    /* Pruebas de insert */
    string nuevo = Interprete::Interpretar("INSERT INTO Pruebas (nombre,galeria) VALUES (Kevin,Pruebas)").first;
    return 0;
}
