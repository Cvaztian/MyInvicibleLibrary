#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <fstream>
#include "Interprete.h"

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    event->accept();
}
void MainWindow::dragLeaveEvent(QDragLeaveEvent *event){
    event->accept();
}
void MainWindow::dragMoveEvent(QDragMoveEvent *event){
    event->accept();
}
void MainWindow::dropEvent(QDropEvent *event){
    QString ruta;
    QList<QUrl> urls;
    QList<QUrl>::Iterator i;
    urls = event->mimeData()->urls();
    for(i = urls.begin(); i!=urls.end(); i++){
        ruta = i->path();
        QPixmap pixmap(ruta);
        ui->insertar_ruta->setText(ruta);
        ui->insertar_imagen->width();
        ui->insertar_imagen->height();
        ui->insertar_imagen->setPixmap(pixmap.scaled(ui->insertar_imagen->width(),ui->insertar_imagen->height(),Qt::KeepAspectRatio));
        ui->insertar_imagen->show();
        ui->insertar_ruta->show();
    }


}

void MainWindow::on_insertar_boton_clicked()
{
    //verificacion de imagen cargada
    string input = get_insertar_galeria().substr(0,5);
    bool ver = (input == "INSERT" || input == "insert");
    if(ver && (!get_insertar_ruta().empty() && get_insertar_ruta() != "Ruta de la imagen"){
        //proceso
    }else{
        //tirar advertencia
    }
    pair<int, char*> auxiliar = abrir_archivo(get_insertar_ruta());
    std::vector<char> imagen = std::vector<char>();
    int tam = auxiliar.first;
    for(int i =0; i<tam; i++){
        imagen.push_back(auxiliar.second[i]);
    }
    nlohmann::json sendable = {
            {"imagen",imagen},
            {"size",tam},
            {"mensaje",""},
            {"protocolo",-1},
            {"nombre",get_insertar_nombre()},
            {"autor",get_insertar_autor()},
            {"descripcion",get_insertar_descripcion()},
            {"galeria",get_insertar_galeria()},
            {"id",-1},
            {"year",get_insertar_ano()}
    };
    RestClient::Put(sendable);  // Esta linea devuelve el resultado de la operacion
}

void MainWindow::on_visualizar_boton_clicked()
{

    //tabla de prueba

    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(5);
    QStringList m_TableHeader;
    m_TableHeader <<"#"<<"Name"<<"Text";
    ui->tableWidget->setHorizontalHeaderLabels(m_TableHeader);
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("Hello"));
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem("Hello"));
    ui->tableWidget->setItem(2, 3, new QTableWidgetItem("Hello"));

    //tabla de prueba
    nlohmann::json sendable = {
            {"imagen",std::vector<char>()},
            {"size",-1},
            {"mensaje",""},
            {"protocolo",-1},
            {"nombre",get_visualizar_nombre()},
            {"autor",""},
            {"descripcion",""},
            {"galeria",get_visualizar_galeria()},
            {"id",-1},
            {"year",""}
    };

    string peticion = "select * from Pruebas WHERE id = 1";

    RestClient::Get(peticion);

    LinkedList<LinkedList<string>> lineas = Interprete::getCampos(nlohmann::json::parse(RestClient::respuesta),peticion);

    cout << RestClient::respuesta << flush;
    /* Esto es para obtener una imagen
    nlohmann::json ayay = nlohmann::json::parse(RestClient::respuesta);  // Al parecer llega bien
    vector<nlohmann::json> prueba = ayay["array"];
    nlohmann::json asd = prueba[0];
    cout << asd["id"] << flush;*/
}

void MainWindow::on_modificar_boton_clicked()
{

}

void MainWindow::on_modificar_cargar_clicked()
{
    nlohmann::json sendable = {
            {"imagen",std::vector<char>()},
            {"size",-1},
            {"mensaje",""},
            {"protocolo",-1},
            {"nombre",get_modificar_nombre()},
            {"autor",""},
            {"descripcion",""},
            {"galeria",get_modificar_galeria()},
            {"id",-1},
            {"year",""}
    };
    RestClient::Get(sendable.dump());
    cout << RestClient::respuesta;
}

void MainWindow::on_eliminar_boton_clicked()
{

}

void MainWindow::on_eliminar_cargar_clicked()
{
    nlohmann::json sendable = {
            {"imagen",std::vector<char>()},
            {"size",-1},
            {"mensaje",""},
            {"protocolo",-1},
            {"nombre",get_eliminar_nombre()},
            {"autor",""},
            {"descripcion",""},
            {"galeria",get_eliminar_galeria()},
            {"id",-1},
            {"year",""}
    };
    RestClient::Delete(sendable.dump());
    cout << RestClient::respuesta;

}


string MainWindow::get_insertar_ano(){
    string ano = ui->insertar_ano->toPlainText().toStdString();
    return ano;
}
string MainWindow::get_insertar_ruta(){
    string ruta = ui->insertar_ruta->text().toStdString();
    return ruta;
}
string MainWindow::get_insertar_autor(){
    string autor = ui->insertar_autor->toPlainText().toStdString();
    return autor;
}
string MainWindow::get_insertar_nombre(){
    string nombre = ui->insertar_nombre->toPlainText().toStdString();
    return nombre;
}
string MainWindow::get_insertar_galeria(){
    string galeria = ui->insertar_galeria->toPlainText().toStdString();
    return galeria;
}
string MainWindow::get_insertar_descripcion(){
    string descripcion = ui->insertar_descripcion->toPlainText().toStdString();
    return descripcion;
}
string MainWindow::get_visualizar_nombre(){
    string nombre = ui->visualizar_nombre->toPlainText().toStdString();
    return nombre;
}
string MainWindow::get_visualizar_galeria(){
    string galeria = ui->visualizar_galeria->toPlainText().toStdString();
    return galeria;
}
int MainWindow::get_modificar_ano(){
    int ano = ui->modificar_ano->toPlainText().toInt();
    return ano;
}
string MainWindow::get_modificar_autor(){
    string autor = ui->modificar_autor->toPlainText().toStdString();
    return autor;
}
string MainWindow::get_modificar_nombre(){
    string nombre = ui->modificar_nombre->toPlainText().toStdString();
    return nombre;
}
string MainWindow::get_modificar_galeria(){
    string galeria = ui->modificar_galeria->toPlainText().toStdString();
    return galeria;
}
string MainWindow::get_modificar_descripcion(){
    string descripcion = ui->modificar_descripcion->toPlainText().toStdString();
    return descripcion;
}
string MainWindow::get_eliminar_nombre(){
    string nombre = ui->eliminar_nombre->toPlainText().toStdString();
    return nombre;
}
string MainWindow::get_eliminar_galeria(){
    string galeria = ui->eliminar_galeria->toPlainText().toStdString();
    return galeria;
}
void MainWindow::show_visualizar_ruta(string ruta){
    QString ruta_aux = QString::fromUtf8(ruta.c_str());
    ui->visualizar_ruta->setText(ruta_aux);
}
void MainWindow::show_modificar_ruta(string ruta){
    QString ruta_aux = QString::fromUtf8(ruta.c_str());
    ui->visualizar_ruta->setText(ruta_aux);
}
void MainWindow::show_eliminar_ruta(string ruta){
    QString ruta_aux = QString::fromUtf8(ruta.c_str());
    ui->visualizar_ruta->setText(ruta_aux);
}
pair<int, char*> MainWindow::abrir_archivo(string ruta) {
       ifstream retornable;
       retornable.open(ruta, ios::in | ios::binary);
       if (retornable.is_open()) {
           retornable.seekg(0,ios::end);
           int size = retornable.tellg();
           retornable.seekg(ios::beg);
           string fullChunkName;

// Create a buffer to hold each chunk
           char *retorno = new char[size];

// Keep reading until end of file
           if (retornable.is_open()) {
               retornable.read(retorno, size);
               retornable.close();
               pair<int, char *> salida;
               salida.first = size;
               salida.second = retorno;
               return salida;
           }
       }
   }

   /*
//aqui
pair<int, char*> MainWindow::crear_archivo(string ruta) {
    ifstream retornable;
    retornable.open(ruta, ios::in | ios::binary);
    if (retornable.is_open()) {
        retornable.seekg(0,ios::end);
        int size = retornable.tellg();
        retornable.seekg(ios::beg);
        string fullChunkName;

// Create a buffer to hold each chunk
        char *retorno = new char[size];

// Keep reading until end of file
        if (retornable.is_open()) {
            retornable.read(retorno, size);
            retornable.close();
            pair<int, char *> salida;
            salida.first = size;
            salida.second = retorno;
            return salida;
        }
    }
}
*/