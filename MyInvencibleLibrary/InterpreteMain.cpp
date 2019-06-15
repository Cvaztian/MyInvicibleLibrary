#include "Interprete.h"
#include <string>
//#include "dbmanager.h"
#include "LinkedList.h"


int main(){
    //Interprete::Interpretar("SELECT rows FROM table WHERE cond");
  //  DBManager manager = DBManager();
//    manager.SelectAll("Pruebas");


    nlohmann::json js = {{"galeria","ay"},
               {"descripcion","aija"},
               {"nombre","kevin"}};
    vector<nlohmann::json> prueba = vector<nlohmann::json>();
    prueba.push_back(js);
    nlohmann::json pruebaJ = {{"array",prueba}};
    //LinkedList<LinkedList<string>> pruebaL = Interprete::getCampos(pruebaJ, "select nombre from kk");
    //cout << pruebaL.getElemento(1)->getData().getElemento(0)->getData() <<flush;

    /* Pruebas de insert */
    string nuevo = Interprete::Interpretar("UPDATE Pruebas SET autor=Kevi,descripcion=Ay WHERE id!=3").first;
    return 0;
}
