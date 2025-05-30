#include <vector>
#include <list>
#include <stdexcept>
#include <string>
#include "HashTable.h"
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include "SeparateChainingHashTable.h"
#include "OpenAddressingHashTable.h"
#include "CuckooHashTable.h"

using namespace std;

// Generowanie kluczy dla różnych przypadków
std::vector<std::pair<int, int>> generate_keys(int count, const std::string &type, int table_size)
{
    std::vector<std::pair<int, int>> keys;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    if (type == "optimistic")
    {
        // Klucze bez kolizji
        for (int i = 0; i < count; ++i)
        {
            keys.emplace_back(i * table_size + 1, dis(gen));
        }
    }
    else if (type == "average")
    {
        // Losowe klucze
        for (int i = 0; i < count; ++i)
        {
            keys.emplace_back(dis(gen), dis(gen));
        }
    }
    else if (type == "pessimistic")
    {
        // Klucze powodujące kolizje
        for (int i = 0; i < count; ++i)
        {
            keys.emplace_back(i * table_size, dis(gen)); // Wszystkie klucze dają ten sam hasz
        }
    }
    return keys;
}

// Funkcje testujące

double measure_add_time(HashTable &ht, const std::vector<std::pair<int, int>> &keys, int iterations)
{
    double total_time = 0.0;
    for (int i = 0; i < iterations; ++i)
    {
        ht.Clear();
        auto start = std::chrono::high_resolution_clock::now();
        for (const auto &pair: keys)
        {
            try
            {
                ht.Add(pair.first, pair.second);
            }
            catch (const std::exception &)
            {
                // Ignoruj wyjątki w testach
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        total_time += std::chrono::duration<double, std::micro>(end - start).count();
    }
    return total_time / iterations;
}

double measure_remove_time(HashTable &ht, const std::vector<std::pair<int, int>> &keys, int iterations)
{
    double total_time = 0.0;
    for (int i = 0; i < iterations; ++i)
    {
        ht.Clear();
        for (const auto &pair: keys)
        {
            try
            {
                ht.Add(pair.first, pair.second);
            }
            catch (const std::exception &)
            {
            }
        }
        auto start = std::chrono::high_resolution_clock::now();
        for (const auto &pair: keys)
        {
            ht.Remove(pair.first);
        }
        auto end = std::chrono::high_resolution_clock::now();
        total_time += std::chrono::duration<double, std::micro>(end - start).count();
    }
    return total_time / iterations;
}


std::pair<double, double> measure_time(HashTable &ht, const std::vector<std::pair<int, int>> &keys, int iterations)
{
    double total_add_time = 0.0;
    double total_remove_time = 0.0;
    for (int i = 0; i < iterations; ++i)
    {
        ht.Clear();
        auto start = std::chrono::high_resolution_clock::now();
        for (const auto &pair: keys)
        {
            try
            {
                ht.Add(pair.first, pair.second);
            }
            catch (const std::exception &)
            {
                // Ignoruj wyjątki w testach
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        total_add_time += std::chrono::duration<double, std::micro>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        for (const auto &pair: keys)
        {
            try
            {
                ht.Remove(pair.first);
            }
            catch (const std::exception &)
            {
                // Ignoruj wyjątki w testach
            }
        }
        end = std::chrono::high_resolution_clock::now();
        total_remove_time += std::chrono::duration<double, std::micro>(end - start).count();
    }

    return {total_add_time / iterations, total_remove_time / iterations};
}

int main()
{
    std::vector<int> table_sizes = {/*10, 50, 100, 250, 500, 1000, 2000, 5000, 10000,*/ 25000, /*50000, 100000*/ };
    std::vector<std::string> cases = {"optimistic", "average", "pessimistic"};
    const int ITERATIONS = 100;
    //const int NUM_KEYS = 500; // Liczba kluczy (mniejsza dla kukułkowego)

    std::ofstream csv_file("hash_table_times2.csv");
    csv_file << "TableType,Operation,Case,TableSize,Time\n";

    for (int table_size: table_sizes)
    {
        int num_keys = table_size;
        for (const auto &case_type: cases)
        {
            std::cout << case_type << " " << table_size << std::endl;
            auto keys = generate_keys(num_keys, case_type, table_size);

            // Test SeparateChainingHashTable
            {
                SeparateChainingHashTable ht(table_size);
                std::cout << "SeparateChaining" << std::endl;
                auto time = measure_time(ht, keys, ITERATIONS);
                csv_file << "SeparateChaining,Add," << case_type << "," << table_size << "," << time.first << "\n";
                csv_file << "SeparateChaining,Remove," << case_type << "," << table_size << "," << time.second << "\n";
            }

            // Test OpenAddressingHashTable
            {
                OpenAddressingHashTable ht(table_size);
                std::cout << "OpenAddressing" << std::endl;
                auto time = measure_time(ht, keys, ITERATIONS);
                csv_file << "OpenAddressing,Add," << case_type << "," << table_size << "," << time.first << "\n";
                csv_file << "OpenAddressing,Remove," << case_type << "," << table_size << "," << time.second << "\n";
            }

            // Test CuckooHashTable
            {
                CuckooHashTable ht(table_size);
                std::cout << "CuckooAddressing" << std::endl;
                auto time = measure_time(ht, keys, ITERATIONS);
                csv_file << "Cuckoo,Add," << case_type << "," << table_size << "," << time.first << "\n";
                csv_file << "Cuckoo,Remove," << case_type << "," << table_size << "," << time.second << "\n";
            }
        }
    }

    csv_file.close();
    std::cout << "Testy zakończone, wyniki zapisane do hash_table_times.csv\n";
    return 0;
}

//// Przykład użycia
//int main() {
//    // Test dla SeparateChainingHashTable
//    SeparateChainingHashTable sc(100);
//    sc.Add(1, 100);
//    sc.Add(2, 200);
//    std::cout << "SeparateChaining contains 1: " << sc.Contains(1) << "\n";
//    std::cout << "Hash of 1: " << sc.GetHash(1) << "\n";
//    sc.Remove(1);
//    std::cout << "SeparateChaining contains 1 after removal: " << sc.Contains(1) << "\n";
//
//    // Test dla OpenAddressingHashTable
//    OpenAddressingHashTable oa(100);
//    oa.Add(1, 100);
//    oa.Add(2, 200);
//    std::cout << "OpenAddressing contains 1: " << oa.Contains(1) << "\n";
//    std::cout << "Hash of 1: " << oa.GetHash(1) << "\n";
//    oa.Remove(1);
//    std::cout << "OpenAddressing contains 1 after removal: " << oa.Contains(1) << "\n";
//
//    // Test dla CuckooHashTable
//    CuckooHashTable ch(100);
//    ch.Add(1, 100);
//    ch.Add(2, 200);
//    std::cout << "Cuckoo contains 1: " << ch.Contains(1) << "\n";
//    std::cout << "Hash of 1: " << ch.GetHash(1) << "\n";
//    ch.Remove(1);
//    std::cout << "Cuckoo contains 1 after removal: " << ch.Contains(1) << "\n";
//
//    return 0;
//}