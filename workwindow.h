#ifndef WORKWINDOW_H
#define WORKWINDOW_H

#include <QWidget>
#include "workwindow.h"
#include "ui_workwindow.h"
#include "string.h"
#include "iostream"
#include <QFileDialog>
#include <QMessageBox>
#include "trie.h"
#include <QDialog>
#include "QFile"

namespace Ui {
class WorkWindow;
}

class WorkWindow : public QDialog
{
    Q_OBJECT

public:
    std::vector<QString> key;
    explicit WorkWindow(QWidget *parent = nullptr,Trie<std::vector<QString>,QChar> *trie = nullptr);
    ~WorkWindow();
    void AddKeyValue(std::vector<QString> key,QChar value);

private:
    Ui::WorkWindow *ui;
    Trie<std::vector<QString>,QChar> *trie = nullptr;

private slots:
    void onDeleteRecordBtnClicked();

    void showDeleteBtn(QListWidgetItem *current, QListWidgetItem *previous);

    void on_addRecordBtn_clicked();

    void on_addColorBtn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

};




#endif // WORKWINDOW_H

