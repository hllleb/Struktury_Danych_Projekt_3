#ifndef PROJEKT3_SEPARATECHAININGHASHTABLE_H
#define PROJEKT3_SEPARATECHAININGHASHTABLE_H

#include <vector>
#include <list>
#include "HashTable.h"

class SeparateChainingHashTable : public HashTable {
private:
    std::vector<std::list<std::pair<int, int>>> table;
    int size;

    // Funkcja haszująca 1: Modulo
    int hash1(int key) const {
        return key % size;
    }

public:
    SeparateChainingHashTable(int table_size) : size(table_size) {
        table.resize(size);
    }

    void Add(int key, int value) override {
        int index = hash1(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                pair.second = value; // Aktualizacja wartości
                return;
            }
        }
        table[index].emplace_back(key, value);
    }

    void Remove(int key) override {
        int index = hash1(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->first == key) {
                table[index].erase(it);
                return;
            }
        }
    }

    void Clear() override {
        for (auto& list : table) {
            list.clear();
        }
    }

    bool Contains(int key) override {
        int index = hash1(key);
        for (const auto& pair : table[index]) {
            if (pair.first == key) {
                return true;
            }
        }
        return false;
    }

    int GetHash(int key) override {
        return static_cast<int>(hash1(key));
    }
};


#endif //PROJEKT3_SEPARATECHAININGHASHTABLE_H
