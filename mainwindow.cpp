#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "QScrollArea"
#include "trie.h"
#include "workwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Открыть файл"),
                                QDir::currentPath(),
                                "TXT File(*.txt)");

    if(!(fileName =="")){
        ui->fileEdit->setText(fileName);
    }
}

void MainWindow::on_deleteFile_clicked()
{
    ui->fileEdit->setText("");
}

void MainWindow::on_downloadFileBtn_clicked()
{
    //создаем новое окно и передаем словарь
    try{
        Trie<std::vector<QString>,QChar> trie ;
        if(ui->fileEdit->text() == "")
            throw 1;

        QFile file(ui->fileEdit->text());

        if(file.open(QIODevice::ReadOnly))
        {
            QDataStream in(&file);
            in >> trie;
            file.close();

            QWidget wid;

            WorkWindow window(&wid,&trie);
            window.setModal(true);
            window.exec();
        }
        else
            QMessageBox ::warning(this,"Заголовок","Не удалось загрузить файл");  
    }
    catch(...)
    {
        QMessageBox ::warning(this,"Заголовок","Не удалось загрузить файл");
    }
}

void MainWindow::on_createNewBtn_clicked()
{
    //создаем новое окно и передаем пустой словарь
    Trie<std::vector<QString>,QChar> trie ;
    QWidget wid;

    WorkWindow window(&wid,&trie);
    window.setModal(true);
    window.exec();
}
