#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "vector"
#include "QMessageBox"
#include <QFileDialog>
#include "QHBoxLayout"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadFileBtn_clicked();

    void on_deleteFile_clicked();

    void on_downloadFileBtn_clicked();

    void on_createNewBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
