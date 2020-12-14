#ifndef TRIE_H
#define TRIE_H

#include "node.h"
#include <QDataStream>


template <class K, class V>
class Trie
{
public:
    Trie();

    Trie(const Trie &other);

    ~Trie(){
        node->ClearNode();
        delete node;
        node = nullptr;
    }

    Node<K, V> *GetNode() const
    {
        return node;
    }

    bool Insert(K key, V name);

    bool Contains(K key) const;

    void Remove(K key);

    void ClearAll();

    int Size() const;

    int SizeKey(K key) const;

    QList<Node<K, V>> GetKeys(K key);

    bool operator ==(const Trie &other) const;

    const V &operator [](const K &key);

    friend QDataStream &operator <<(QDataStream &stream, const Trie<K, V> &t)
    {
        stream << *(t.node);
        return stream;
    }

    friend QDataStream &operator >>(QDataStream &stream, Trie<K, V> &t)
    {
        t.ClearAll();
        stream >> *(t.node);
        return stream;
    }

private:
    Node<K, V> *node;

};


template<class K, class V> //конструктор по умолчанию
Trie<K, V>::Trie()
{
    node = new Node<K, V>;
}

template<class K, class V> //конструктор копирования
Trie<K, V>::Trie(const Trie &other)
{
    node = new Node<K, V>(*other.node, 0);
}

template<class K, class V> //метод добавления пары ключ-значение
bool Trie<K, V>::Insert(K key, V name)
{
    if (Contains(key))
        return false;

    int pos = 0;
    Node<K, V> *tmp = node;
    while (pos < key.size())
    {
        int i;
        for (i = 0; i < tmp->childs.size() && tmp->childs[i].GetKey().at(pos) !=
                                                             key.at(pos); i++);
        if (i == tmp->childs.size())
        {
            Node<K, V> *newtmp;
            if (pos == key.size()-1)
            {
                newtmp = new Node<K, V>(Node<K,V>::Slice(key,pos+1), name, tmp->lvl+1);
            }
            else
                newtmp = new Node<K, V>(Node<K,V>::Slice(key,pos+1), V(), tmp->lvl+1);
            tmp->AddChild(*newtmp);
            tmp = &(tmp->childs[tmp->childs.size()-1]);
        }
        else
            tmp = &(tmp->childs[i]);
        pos++;
    }
    if (tmp->GetKey().at(pos-1) == key.at(key.size()-1))
    {
        tmp->SetKeyName(name);
        tmp->exist = true;
        return true;
    }
}

template<class K, class V> //проверка существования ключа
bool Trie<K, V>::Contains(K key) const
{
    int pos = 0;
    Node<K,V> *findNode = node->Find(key, pos);
    if (findNode!= nullptr && findNode->exist)
        return true;
    return false;
}

template<class K, class V> //удаление пары ключ-значение по ключу
void Trie<K, V>::Remove(K key)
{
    if (!Contains(key))
        return;
    int pos = 0;
    Node<K, V> *tmp = node;
    while (pos < key.size())
    {
        int i;
        for (i = 0; i < tmp->childs.size() && tmp->childs[i].GetKey().at(pos) !=
                                                             key.at(pos); i++);
        tmp = &(tmp->childs[i]);
        pos++;
    }

    while (tmp != node)
    {
        if (tmp->HasChilds())
        {
            if (tmp->GetKey() == Node<K,V>::Slice(key,key.size()))
            {
                tmp->SetKeyName(V());
                tmp->exist = false;
            }
            return;
        }
        if (tmp->exist && tmp->lvl != key.size())
            return;
        Node<K, V> *p = tmp->GetParent();
        p->DelChild(*tmp);
        tmp = p;
    }
}

template<class K, class V> //метод удаляющий все элементы
void Trie<K, V>::ClearAll()
{
    node->ClearNode();
}

template<class K, class V> //метод, возвращающий количество хранящихся элементов
int Trie<K, V>::Size() const
{
    int k = 0;
    return node->CountKeys(k);
}

template<class K, class V> //метод, возвращающий размер ключа
int Trie<K, V>::SizeKey(K key) const
{
    int pos = 0;
    Node<K, V> *tmp = node;
    while (pos < key.size())
    {
        int i;
        for (i = 0; i < tmp->childs.size() && tmp->childs[i].GetKey().at(pos) !=
                                                             key.at(pos); i++);
        if (tmp->childs.size()) tmp = &(tmp->childs[i]);
        pos++;
    }

    int k = 0;
    if (tmp->GetParent())
        return tmp->GetParent()->CountKeys(k);
    else
        return tmp->CountKeys(k);
}

template<class K, class V> //метод, возвращающий список ключей, начинающихся с заданной пос-ти
QList<Node<K, V> > Trie<K, V>::GetKeys(K key)
{
    int pos = 0;
    Node<K, V> *tmp = node;
    while (pos < key.size())
    {
        int i;
        for (i = 0; i < tmp->childs.size(); i++)
        {
            if(tmp->childs[i].GetKey().at(pos) ==key.at(pos))
            {
                break;
            }
        }

        if (i <tmp->childs.size())
            tmp = &(tmp->childs[i]);
        else
            return QList<Node<K, V> >();
        pos++;
    }

    QList<Node<K, V> > lst;
    if (tmp->exist)
        lst.append(*tmp);
    tmp->GetKeys(lst);
    return lst;
}

template<class K, class V> //перегрузка оператора ==
bool Trie<K, V>::operator ==(const Trie &other) const
{
    return (*node == *other.node);
}

template<class K, class V> //оператор []
const V& Trie<K, V>::operator [](const K &key)
{
    int pos = 0;
    Node<K, V> *tmp = node;
    while (pos < key.size())
    {
        int i;
        for (i = 0; i < tmp->childs.size() && tmp->childs[i].GetKey().at(pos) !=
                                                             key.at(pos); i++);
        tmp = &(tmp->childs[i]);
        pos++;
    }
    V *str = new V(tmp->GetKeyName());
    return *str;
}

#endif // TRIE_H
