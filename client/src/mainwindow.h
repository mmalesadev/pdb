#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "PDBClient.h"
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(PDBClient& pdbClient, QWidget *parent = nullptr);
    ~MainWindow();

private:
    void guiConsoleMessage(const std::string& message);

private slots:
    void on_actionConnectToServer_triggered();

private:
    Ui::MainWindow *ui_;
    PDBClient& pdbClient_;
};

#endif // MAINWINDOW_H
