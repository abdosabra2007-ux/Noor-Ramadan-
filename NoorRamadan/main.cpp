#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);


    app.setApplicationName("نور رمضان");

    MainWindow w;
    w.show();

    return app.exec();
}
