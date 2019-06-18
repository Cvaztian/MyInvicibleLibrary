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
    string input = get_insertar_nombre().substr(0,6);
    bool ver = (input == "INSERT" || input == "insert");
    nlohmann::json js;
    if(ver && (!get_insertar_ruta().empty() && get_insertar_ruta() != "Ruta de la imagen")){
        //proceso
        string jsS = Interprete::Interpretar(get_insertar_nombre()).first;
        js = nlohmann::json::parse(jsS);
        js["size"] = abrir_archivo(get_insertar_ruta()).first;
        pair<int, char*> auxiliar = abrir_archivo(get_insertar_ruta());
    std::vector<char> imagen = std::vector<char>();
    int tam = auxiliar.first;
    for(int i =0; i<tam; i++){
        imagen.push_back(auxiliar.second[i]);
    }
    js["imagen"] = imagen;
    string jaja = js.dump();
    RestClient::Put(js);
    }else{
        if(ver){
            //tirar advertencia
        }else{
            if(input == "SELECT" || input == "select"){
                pair<string,string> interpretted = Interprete::Interpretar(get_insertar_nombre());
                LinkedList<LinkedList<string>> jsonLinked = Interprete::getCampos(interpretted.first, interpretted.second);
            }
        }
    }

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



string MainWindow::get_insertar_ruta(){
    string ruta = ui->insertar_ruta->text().toStdString();
    return ruta;
}

string MainWindow::get_insertar_nombre(){
    string nombre = ui->insertar_nombre->toPlainText().toStdString();
    return nombre;
}

void MainWindow::on_modificar_boton_clicked(){}
void MainWindow::on_modificar_cargar_clicked(){}
void MainWindow::on_eliminar_boton_clicked(){}
void MainWindow::on_eliminar_cargar_clicked(){}
string MainWindow::get_insertar_ano(){}
string MainWindow::get_insertar_autor(){}
string MainWindow::get_insertar_galeria(){}
string MainWindow::get_insertar_descripcion(){}
string MainWindow::get_visualizar_nombre(){}
string MainWindow::get_visualizar_galeria(){}
int MainWindow::get_modificar_ano(){}
string MainWindow::get_modificar_autor(){}
string MainWindow::get_modificar_nombre(){}
string MainWindow::get_modificar_galeria(){}
string MainWindow::get_modificar_descripcion(){}
string MainWindow::get_eliminar_nombre(){}
string MainWindow::get_eliminar_galeria(){}
void MainWindow::show_visualizar_ruta(string ruta){}
void MainWindow::show_modificar_ruta(string ruta){}
void MainWindow::show_eliminar_ruta(string ruta){}


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