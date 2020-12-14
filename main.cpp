#include <iostream>
#include "trie.h"
#include <assert.h>
#include "test.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Test<std::vector<QString>,QChar> test ;
    test.StartTests();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
//    return 0;

}
