#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QDataStream>
#include "trie.h"

template <class K, class V>
class Node
{
public:
    Node();
    Node(K b, V n, int l);
    Node(const Node &other, int l);
    bool exist;
    int lvl;
    QList<Node> childs;

    Node *GetParent() const
    {
        return parent;
    }

    void SetParent(Node *p)
    {
        parent = p;
    }

    K GetKey() const
    {
        return btn;
    }

    void SetBtn(K b)
    {
        btn = b;
    }

    V GetKeyName() const
    {
        return keyName;
    }

    void SetKeyName(V n)
    {
        keyName = n;
    }

    bool HasChilds()
    {
        return childs.size();
    }

    bool HasChild(const Node &n);

    void AddChild(Node &n);

    void DelChild(const Node &n);

    Node<K, V>* Find(K key, int &pos);

    Node<K, V>* FindAuto(K key, int &pos);

    void ClearNode();

    int Count(int &k);

    int CountKeys(int &k);

    QList<Node<K, V> > GetKeys(QList<Node> &lst);

    bool operator ==(const Node &other) const;

    static K Slice(K key , int pos)
    {
        K newkey =  K(pos);
        for(int j = 0; j<pos; j++)
        {
            newkey[j] = key[j];
        }
        return newkey;
    }

private:
    K btn;
    Node *parent;
    V keyName;

};

template <class K, class V>
QDataStream &operator <<(QDataStream &stream, const Node<K, V> &n)
{
    stream << n.childs.size();
    for (int i = 0; i < n.childs.size(); i++)
    {
        K key = n.childs[i].GetKey();
        stream << (int)key.size();
        for(int j = 0; j< key.size(); j++)
        {
            stream << key[j];
        }
        stream << n.childs[i].GetKeyName();
        stream << n.childs[i].exist;
    }

    for (int i = 0; i < n.childs.size(); i++)
        stream << n.childs[i];
    return stream;
}

template <class K, class V>
QDataStream &operator >>(QDataStream &stream, Node<K, V> &n)
{
    Node<K, V> tmp;
    int ncolors;
    V cn;
    bool ex;
    int chsize;
    stream >> chsize;
    for (int i = 0; i < chsize; i++)
    {
        stream >> ncolors;
        K b = K(ncolors);
        for(int j = 0; j< ncolors; j++)
        {
            stream >> b[j];
        }
        stream >> cn;
        stream >> ex;
        tmp.SetBtn(b);
        tmp.SetKeyName(cn);
        tmp.exist = ex;
        tmp.lvl = n.lvl+1;
        n.AddChild(tmp);
    }
    for (int i = 0; i < n.childs.size(); i++)
        stream >> n.childs[i];
    return stream;
}

template <class K, class V> //создание пустого узла
Node<K, V>::Node()
{
    lvl = 0;
    parent = nullptr;
    exist = false;
}

template <class K, class V> //проверка существования (создание) узла
Node<K, V>::Node(K b, V n, int l)
{
    lvl = l;
    if (n != V())
        exist = true;
    else
        exist = false;
    btn = b;
    keyName = n;
}

template <class K, class V> //конструктор копирования узла
Node<K, V>::Node(const Node &other, int l) : btn(other.btn),
                                      keyName(other.keyName), exist(other.exist)
{
    lvl = l;
    if (!other.parent) parent = nullptr;
    for (int i = 0; i < other.childs.size(); i++)
    {
        Node<K, V> n(other.childs[i], l+1);
        AddChild(n);
    }
}

template <class K, class V> //метод проверки существования дочернего узла
bool Node<K, V>::HasChild(const Node &n)
{
    for (int i = 0; i < childs.size(); i++)
        if (childs.at(i) == n)
            return true;
    return false;
}

template <class K, class V> //метод добавления дочернего узла
void Node<K, V>::AddChild(Node &n)
{
    childs.append(n);
    childs[childs.size()-1].SetParent(this);
}

template <class K, class V> //метод удаления дочернего узла
void Node<K, V>::DelChild(const Node &n)
{
    for (int i = 0; i < childs.size(); i++)
        if (childs[i] == n)
        {
            childs.erase(childs.begin()+i, childs.begin()+i+1);
            return;
        }
}

template <class K, class V> //метод поиска дочернего узла
Node<K, V> *Node<K, V>::Find(K key, int &pos)
{
    if(pos == key.size() && this->btn == Node<K, V>::Slice(key,pos) && this->exist)
        return this;

    if(pos<key.size())
    {
        pos++;
        for (int i = 0; i < childs.size(); i++)
        {
            Node<K, V> *findNode = childs[i].Find(key, pos);
            if(findNode != nullptr && findNode->GetKey() == key)
            {
                return findNode;
            }
        }
    }
}

template <class K, class V> //метод поиска ближайшего узла
Node<K, V> *Node<K, V>::FindAuto(K key, int &pos)
{
    for (int i = 0; i < childs.size(); i++)
        if (childs[i].btn == key[++pos])
            childs[i].FindAuto(key, ++pos);
    return this;
}

template <class K, class V> //метод удаления узла
void Node<K, V>::ClearNode()
{
    for (int i = 0; i < childs.size(); i++)
        childs[i].ClearNode();
    childs.clear();
}

template <class K, class V> //подсчёт дочерних узлов
int Node<K, V>::Count(int &k)
{
    k += childs.size();
    for (int i = 0; i < childs.size(); i++)
        childs[i].Count(k);
    return k;
}

template <class K, class V> //подсчёт ключей
int Node<K, V>::CountKeys(int &k)
{
    for (int i = 0; i < childs.size(); i++)
        if (childs[i].exist)
            k++;
    for (int i = 0; i < childs.size(); i++)
        childs[i].CountKeys(k);
    return k;
}

template <class K, class V> //метод, возврающий ключ
QList<Node<K, V> > Node<K, V>::GetKeys(QList<Node> &lst)
{
    for (int i = 0; i < childs.size(); i++)
        if (childs[i].exist)
            lst.append(childs[i]);
    for (int i = 0; i < childs.size(); i++)
        childs[i].GetKeys(lst);
    return lst;
}

template <class K, class V> //перегрузка оператора ==
bool Node<K, V>::operator ==(const Node &other) const
{
    if (btn == other.btn && keyName == other.keyName && childs.size()
                                                        == other.childs.size())
    {
        for (int i = 0; i < childs.size(); i++)
            if (!(childs[i] == other.childs[i]))
                return false;
        return true;
    }
    return false;
}

#endif // NODE_H
