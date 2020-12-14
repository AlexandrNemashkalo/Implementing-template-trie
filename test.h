#ifndef TEST_H
#define TEST_H

#include "cassert"
#include "iostream"
#include "trie.h"
#include "node.h"


template <class K, class V>
class Test
{

public:
    void StartTests()
    {
        Trie<std::vector<QString>,QChar> trie;
        DefaultConstructorTest(trie);

        std::vector<QString> key = {"green","red"};
        QChar value = 'a';
        InsertTest(trie, key, value);
        ContainsAddedKeyTest(trie,key);
        GetValueByKeyTest(trie,key,value);
        RemoveTest(trie, key);
        ContainsRemovedKeyTest(trie,key);

        std::vector<QString> key1 = {"green","red","blue"};
        QChar value1 = 'b';
        Trie<std::vector<QString>,QChar> copyTrie(trie);
        CopyConstructorTest(trie,copyTrie,key1,value1);

        std::vector<QString> key2 = {"red","red","red"};
        QChar value2 = 'c';
        trie.Insert(key2,value2);

        Trie<std::vector<QString>,QChar> defaultTrie;
        ClearAllTest(trie,defaultTrie);

        trie.Insert(key,value);
        ReadAndWriteFileTests(trie);

        std::cout<<"All tests completed"<<std::endl;
    }

    void DefaultConstructorTest( const Trie<K,V>  &trie)
    {
        assert(trie.Size() == 0);
    }

    void InsertTest(Trie<K,V> &trie, K &key, V &value)
    {
        int pastSize = trie.Size();
        trie.Insert(key,value);
        assert(pastSize + 1 == trie.Size());
    }

    void ContainsAddedKeyTest( const Trie<K,V> &trie, K &key)
    {
       assert(trie.Contains(key));
    }

    void GetValueByKeyTest(Trie<K,V> &trie, K &key, V &value)
    {
       assert(trie[key] == value);
    }

    void RemoveTest(Trie<K,V> &trie, K key)
    {
        int pastSize = trie.Size();
        trie.Remove(key);
        assert(pastSize - 1 == trie.Size());
        pastSize = trie.Size();
        trie.Remove(key);
        assert(pastSize == trie.Size());
    }

    void ContainsRemovedKeyTest( const Trie<K,V> &trie, K &key)
    {
       assert(!trie.Contains(key));
    }

    void ClearAllTest(Trie<K,V> &trie ,Trie<K,V> &defaultTrie)
    {
       trie.ClearAll();
       assert(trie == defaultTrie);
    }

    void CopyConstructorTest(Trie<K,V> &trie,Trie<K,V> &copyTrie, K &key, V &value)
    {
        assert(trie == copyTrie);
        copyTrie.Insert(key,value);
        assert(!(trie==copyTrie));
        copyTrie.Remove(key);
        assert(trie == copyTrie);
    }


    void ReadAndWriteFileTests(Trie<K,V> &trie)
    {
        QFile file(QString("../oop2/output.txt"));

        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out << trie;
        file.close();

        Trie<std::vector<QString>,QChar> outputTrie;
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in >> outputTrie;
        file.close();

        assert(trie == outputTrie);
    }
};

#endif // TEST_H
