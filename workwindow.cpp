#include "workwindow.h"
#include "ui_workwindow.h"
#include "trie.h"
#include "vector"
#include "QFrame"
#include "QVBoxLayout"
#include "QLabel"
#include "QGridLayout"
#include "QSizePolicy"
#include "QScrollArea"
#include "QLineEdit"
#include "QPushButton"
#include "QListWidgetItem"
#include "QMessageBox"
#include <QFileDialog>
#include "iostream"
#include "qdynamicrecord.h"
#include <QInputDialog>


WorkWindow::WorkWindow(QWidget *parent,Trie<std::vector<QString>,QChar> *trie ) :
    QDialog(parent),
    ui(new Ui::WorkWindow),
    trie(trie)
{

    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            SLOT(showDeleteBtn(QListWidgetItem *, QListWidgetItem *)));

    QList< Node<std::vector<QString>,QChar> > records = trie->GetKeys(std::vector<QString>());

    for(int i =0; i< records.size(); i ++){
        AddKeyValue(records[i].GetKey(),records[i].GetKeyName());
    }
}

WorkWindow::~WorkWindow()
{
    key.clear();
    trie->ClearAll();
    //delete trie;
    delete ui;
}


void WorkWindow::AddKeyValue(std::vector<QString> key,QChar value)
{
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget );

    QDynamicRecord* wgt = new QDynamicRecord(key,value);
    QLayout* l = new QHBoxLayout;

    QString keyString ="";
    for(int i = 0; i< key.size();i++){
        keyString = keyString + key[i]+" ";
    }

    QLabel * keyRecord = new QLabel(keyString);
    keyRecord->setWordWrap(true);
    keyRecord->setFixedWidth(270);
    l->addWidget(keyRecord);

    QLabel * valueRecord = new QLabel(value);
    valueRecord->setWordWrap(true);
    l->addWidget(valueRecord);

    QWidget * btnWgt = new QWidget();
    btnWgt->setFixedHeight(25);
    btnWgt->setFixedWidth(30);

    QPushButton* deleteGanreBtn = new QPushButton( "❌", btnWgt );
    deleteGanreBtn->setFixedWidth(25);
    deleteGanreBtn->setFixedHeight(25);
    deleteGanreBtn->hide();
    connect( deleteGanreBtn, SIGNAL( clicked() ), SLOT( onDeleteRecordBtnClicked() ) );

    l->addWidget( btnWgt );
    wgt->setLayout( l );
    item->setSizeHint( wgt->sizeHint() );

    ui->listWidget->setItemWidget( item, wgt );
}


void WorkWindow::onDeleteRecordBtnClicked()
{
    if(ui->listWidget->count()>1)
    {
        QListWidgetItem* item = ui->listWidget->currentItem();
        QDynamicRecord *widget = qobject_cast<QDynamicRecord*>(ui->listWidget->itemWidget(item));

        trie->Remove(widget->getKey());
        delete ui->listWidget->takeItem(ui->listWidget->currentRow());
    }
    else{
       QMessageBox::information( this, "",QString("Словарь должен содержать хотя бы одну запись"));
    }
}


void WorkWindow::showDeleteBtn(QListWidgetItem * itemCur, QListWidgetItem * itemPr )
{
    QWidget *widget1 = qobject_cast<QWidget*>(ui->listWidget->itemWidget(itemCur));
    QPushButton *btnCur = widget1->findChild<QPushButton*>();
    btnCur->show();

    if(QWidget *widget2 = qobject_cast<QWidget*>(ui->listWidget->itemWidget(itemPr)))
    {
        QPushButton *btnPr = widget2->findChild<QPushButton*>();
        btnPr->hide();
    }
}



void WorkWindow::on_addRecordBtn_clicked()
{
    //создаем ноую запись через интерфейс
    if(this->key.size()<1)
    {
        QMessageBox::information( this, "",QString("Ключ должен содержать хотя бы один цвет"));
        return;
    }

    if(trie->Contains(this->key))
    {
        QMessageBox::information( this, "",QString("Такой ключ уже существует"));
        return;
    }
    bool bOk;
    QString value = QInputDialog::getText( 0,
                                         "Input",
                                         "Char:",
                                         QLineEdit::Normal,
                                         "",
                                         &bOk
                                        );
    if (!bOk)
    {
        // Была нажата кнопка Cancel
        return;
    }

    if(value.size()!= 1)
    {
        QMessageBox::information( this, "",QString("Значение - это символ"));
        return;
    }
    else
    {
        trie->Insert(this->key,value[0]);
        AddKeyValue(this->key,value[0]);
    }
}

void WorkWindow::on_addColorBtn_clicked()
{
    //добавляем цвет в ключ
    if(ui->colorInput->text()!="")
    {
        key.push_back(ui->colorInput->text());
        ui->listKey->addItem(ui->colorInput->text());
    }
}

void WorkWindow::on_pushButton_clicked()
{
    //очищаем ключ
    key.clear();
    ui->listKey->clear();
}

void WorkWindow::on_pushButton_3_clicked()
{
    //получение значения по ключу
    if(trie->Contains(key))
    {
        QChar value = (*trie)[key];
        QMessageBox::information( this, "",value);
    }
    else
    {
        QMessageBox::information( this, "",QString("Для такого ключа нет значения"));
    }
}

void WorkWindow::on_pushButton_4_clicked()
{
    //получение ключа по значению
    key.clear();
    ui->listKey->clear();

    QList< Node<std::vector<QString>,QChar> > records = trie->GetKeys(std::vector<QString>());
    for(int i = 0; i<records.size();i++){
        if(records[i].GetKeyName() == ui->valueInput->text()[0]){
            for(int j =0;j<records[i].GetKey().size();j++){
                key.push_back(records[i].GetKey()[j]);
                ui->listKey->addItem(records[i].GetKey()[j]);
            }
            return;
        }
    }
    QMessageBox::information( this, "",QString("Такого значения в словаре нет"));
}

void WorkWindow::on_pushButton_2_clicked()
{
    //умный поиск
    QList< Node<std::vector<QString>,QChar> > records = trie->GetKeys(key);

    if(records.size()==0)
    {
        QMessageBox::information( this, "",QString("Нет ключей начинающихся с заданного ключа"));
        return;
    }

    QString result;
    for(int i=0;i<records.size();i++)
    {
        result += QString(records[i].GetKeyName()) +": ";
        for(int j=0;j<records[i].GetKey().size();j++)
        {
            result = result + records[i].GetKey()[j]+" ";
        }
        result = result +"\n";
    }
    QMessageBox::information( this, "",result);
}

void WorkWindow::on_pushButton_6_clicked()
{
    //загружаем словарь в файл
    QString fileName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Открыть файл"),
                                QDir::currentPath(),
                                "Images (*.txt);;All files (*.*)");
    if(!(fileName ==""))
    {
       QFile file(fileName);
       file.open(QIODevice::WriteOnly);
       QDataStream out(&file);
       out << (*trie);
       file.close();

       QMessageBox ::information(this,"Результат","Словарь был успешно загружен в файл");
    }
}



