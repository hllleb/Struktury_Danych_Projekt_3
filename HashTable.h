#ifndef PROJEKT3_HASHTABLE_H
#define PROJEKT3_HASHTABLE_H

class HashTable
{
public:
    virtual void Add(int key, int value) = 0;
    virtual void Remove(int key) = 0;
    virtual void Clear() = 0;
    virtual bool Contains(int key) = 0;
    virtual int GetHash(int key) = 0;
};

#endif //PROJEKT3_HASHTABLE_H
