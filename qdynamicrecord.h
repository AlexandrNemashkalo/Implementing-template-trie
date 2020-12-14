#ifndef QDYNAMICRECORD_H
#define QDYNAMICRECORD_H

#include <QWidget>

class QDynamicRecord : public QWidget
{
    Q_OBJECT
public:
    QDynamicRecord(std::vector<QString> &key,QChar value,QWidget *parent = 0);
    ~QDynamicRecord();

    std::vector<QString> getKey() const;
    QChar getValue() const;

private:
    std::vector<QString> key;
    QChar value;

};

#endif // QDYNAMICRECORD_H
