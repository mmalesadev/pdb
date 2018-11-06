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
    QString ipQ = ui_->ipLine->text();                  
    QString portQ = ui_->portLine->text();
    
    // Zamiana z QString na string
    std::string ip = ipQ.toStdString();
    std::string port = portQ.toStdString();

    guiConsoleMessage("Connecting to server: " + ip + ":" + port);  //"192.168.0.11" "7171"
    pdbClient_.connectToServer(ip, port);
}

MainWindow::~MainWindow()
{
    delete ui_;
}
