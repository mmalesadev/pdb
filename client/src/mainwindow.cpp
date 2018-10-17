#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(PDBClient& pdbClient, QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    pdbClient_(pdbClient)
{
    ui_->setupUi(this);
   
}

void MainWindow::guiConsoleMessage(const std::string& message)
{
    ui_->guiConsole->append(QString::fromStdString(message));
}

void MainWindow::on_actionConnectToServer_triggered()
{
    guiConsoleMessage("testMessage");
}

MainWindow::~MainWindow()
{
    delete ui_;
}
