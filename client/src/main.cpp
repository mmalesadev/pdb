#include "mainwindow.h"
#include <QApplication>
#include "PDBClient.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PDBClient pdbClient;
    MainWindow w(pdbClient);

    w.show();

    return a.exec();
}

