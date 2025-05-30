#ifndef PROJEKT3_CUCKOOHASHTABLE_H
#define PROJEKT3_CUCKOOHASHTABLE_H

#include <vector>
#include <string>
#include <stdexcept>
#include "HashTable.h"

class CuckooHashTable : public HashTable {
private:
    std::vector<std::pair<int, int>> table1, table2;
    std::vector<bool> occupied1, occupied2;
    int size;
    static const int MAX_ATTEMPTS = 10;

    // Funkcja haszująca 3
    int hash3(int key) const {
        return ((key >> 4) ^ key) % size;
    }

public:
    CuckooHashTable(int table_size) : size(table_size) {
        table1.resize(size, {0, 0});
        table2.resize(size, {0, 0});
        occupied1.resize(size, false);
        occupied2.resize(size, false);
    }

    void Add(int key, int value) override {
        int current_key = key;
        int current_value = value;
        bool use_table1 = true;
        int index;

        for (int i = 0; i < MAX_ATTEMPTS; ++i) {
            if (use_table1) {
                index = hash3(current_key);
                if (!occupied1[index]) {
                    table1[index] = {current_key, current_value};
                    occupied1[index] = true;
                    return;
                }
                std::swap(current_key, table1[index].first);
                std::swap(current_value, table1[index].second);
            } else {
                index = hash3(current_key);
                if (!occupied2[index]) {
                    table2[index] = {current_key, current_value};
                    occupied2[index] = true;
                    return;
                }
                std::swap(current_key, table2[index].first);
                std::swap(current_value, table2[index].second);
            }
            use_table1 = !use_table1;
        }
        throw std::runtime_error("Cuckoo hash table insertion failed");
    }

    void Remove(int key) override {
        int index1 = hash3(key);
        if (occupied1[index1] && table1[index1].first == key) {
            occupied1[index1] = false;
            return;
        }
        int index2 = hash3(key);
        if (occupied2[index2] && table2[index2].first == key) {
            occupied2[index2] = false;
            return;
        }
    }

    void Clear() override {
        std::fill(occupied1.begin(), occupied1.end(), false);
        std::fill(occupied2.begin(), occupied2.end(), false);
    }

    bool Contains(int key) override {
        int index1 = hash3(key);
        if (occupied1[index1] && table1[index1].first == key) {
            return true;
        }
        int index2 = hash3(key);
        if (occupied2[index2] && table2[index2].first == key) {
            return true;
        }
        return false;
    }

    int GetHash(int key) override {
        return static_cast<int>(hash3(key));
    }
};

#endif //PROJEKT3_CUCKOOHASHTABLE_H
