#ifndef PROJEKT3_OPENADDRESSINGHASHTABLE_H
#define PROJEKT3_OPENADDRESSINGHASHTABLE_H

#include <utility>
#include <vector>
#include <stdexcept>
#include "HashTable.h"

class OpenAddressingHashTable : public HashTable {
private:
    std::vector<std::pair<int, int>> table; // Para: klucz, wartość
    std::vector<bool> occupied; // Flaga zajętości
    int size;

    // Funkcja haszująca 2: Mnożenie
    int hash2(int key) const {
        return (key * 31) % size;
    }

public:
    OpenAddressingHashTable(int table_size) : size(table_size) {
        table.resize(size, {0, 0});
        occupied.resize(size, false);
    }

    void Add(int key, int value) override {
        int index = hash2(key);
        int original_index = index;
        do {
            if (!occupied[index]) {
                table[index] = {key, value};
                occupied[index] = true;
                return;
            }
            if (table[index].first == key) {
                table[index].second = value; // Aktualizacja wartości
                return;
            }
            index = (index + 1) % size; // Liniowe sondowanie
            if (index == original_index) {
                throw std::runtime_error("Table is full");
            }
        } while (true);
    }

    void Remove(int key) override {
        int index = hash2(key);
        int original_index = index;
        do {
            if (occupied[index] && table[index].first == key) {
                occupied[index] = false;
                return;
            }
            index = (index + 1) % size;
            if (index == original_index) {
                break;
            }
        } while (true);
    }

    void Clear() override {
        std::fill(occupied.begin(), occupied.end(), false);
    }

    bool Contains(int key) override {
        int index = hash2(key);
        int original_index = index;
        do {
            if (occupied[index] && table[index].first == key) {
                return true;
            }
            index = (index + 1) % size;
            if (index == original_index || !occupied[index]) {
                break;
            }
        } while (true);
        return false;
    }

    int GetHash(int key) override {
        return static_cast<int>(hash2(key));
    }
};


#endif //PROJEKT3_OPENADDRESSINGHASHTABLE_H
