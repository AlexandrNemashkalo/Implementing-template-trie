#include "qdynamicrecord.h"

QDynamicRecord::QDynamicRecord(std::vector<QString> &key_, QChar value_,QWidget *parent):
    QWidget(parent),
    key(key_),
    value(value_)
{
}

QDynamicRecord::~QDynamicRecord()
{
}

std::vector<QString> QDynamicRecord::getKey() const
{
    return this->key;
}

QChar QDynamicRecord::getValue() const
{
    return this->value;
}
