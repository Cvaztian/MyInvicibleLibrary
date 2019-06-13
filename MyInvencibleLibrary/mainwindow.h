#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int get_insertar_ano();
    string get_insertar_ruta();
    string get_insertar_autor();
    string get_insertar_nombre();
    string get_insertar_galeria();
    string get_insertar_descripcion();
    string get_visualizar_nombre();
    string get_visualizar_galeria();
    int get_modificar_ano();
    string get_modificar_autor();
    string get_modificar_nombre();
    string get_modificar_galeria();
    string get_modificar_descripcion();
    string get_eliminar_nombre();
    string get_eliminar_galeria();
    void show_visualizar_ruta(string ruta);
    void show_modificar_ruta(string ruta);
    void show_eliminar_ruta(string ruta);
    int getFileSize(ifstream *file);
    pair<int, char*>  abrir_archivo(string ruta);

protected:

   void dragEnterEvent(QDragEnterEvent *event);
   void dragLeaveEvent(QDragLeaveEvent *event);
   void dragMoveEvent(QDragMoveEvent *event);
   void dropEvent(QDropEvent *event);

private slots:
   void on_insertar_boton_clicked();
   void on_visualizar_boton_clicked();
   void on_modificar_boton_clicked();
   void on_modificar_cargar_clicked();
   void on_eliminar_boton_clicked();
   void on_eliminar_cargar_clicked();
};

#endif // MAINWINDOW_H
