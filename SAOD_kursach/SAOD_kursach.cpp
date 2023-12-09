#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <random>
#include <sstream>

#include "BTree.h"

using namespace std::chrono_literals;
using seconds = std::chrono::seconds;
using milliseconds = std::chrono::milliseconds;
using microseconds = std::chrono::microseconds;
using nanoseconds = std::chrono::nanoseconds;



void tryAddKeys(BTree& tree, const std::vector<int>& keys) {
    tree.insertList(keys);
    tree.print();

    if (tree.getKeyCount() != keys.size()) {
        throw "Not all keys have been added";
    }
}

void tryRemoveKeys(BTree& tree, const std::vector<int>& keys) {
    for (int el : keys) {
        std::cout << "rem : " << el << std::endl;
        tree.remove(el);
        tree.print();
    }

    //tree.print();

    if (tree.getKeyCount() != 0) {
        throw "Not all keys have been deleted";
    }
}

void testing(BTree& tree) {
    std::vector<int> keys = { 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 11, 12, 13, 14, 15 };

    try {
        tryAddKeys(tree, keys);
        tryRemoveKeys(tree, keys);
        tree.clear();
        std::cout << "TESTING COMPLITED" << std::endl;
    }
    catch (const char* msg) {
        std::cout << "TESTING FAILED : " << msg << std::endl;
    }
}

void measurements(BTree& tree, const size_t& keyAmount, const size_t& numOfMeasurements) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, keyAmount * 10000);

    microseconds elapsedAddingTime = 0ms;
    microseconds elapsedRemoveTime = 0ms;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;

    std::vector<int> keys;
    keys.reserve(50);

    for (size_t i = 0; i < numOfMeasurements; ++i) {
        for (size_t j = 0; j < keyAmount; ++j) {
            int rndNum = dist(gen);

            start = std::chrono::steady_clock::now();

            tree.insert(rndNum);
            keys.push_back(rndNum);

            end = std::chrono::steady_clock::now();
            elapsedAddingTime += std::chrono::duration_cast<microseconds> (end - start);
        }

        /*for (auto el : keys) {
            std::cout << el << std::endl;
        }*/

        for (size_t j = 0; j < keys.size(); ++j) {
            start = std::chrono::steady_clock::now();

            tree.print();
            std::cout << "removing : " << keys[j] << std::endl;
            tree.remove(keys[j]);
            end = std::chrono::steady_clock::now();
            elapsedRemoveTime += std::chrono::duration_cast<microseconds> (end - start);
        }

        //std::cout << i + 1 << " cycle complited" << std::endl;

        tree.clear();
        keys.clear();
    }

    //---------------------------------------------------------------------------------------
    std::cout << "MEASUREMENTS : ";
    std::cout << "ELAPSED TIME (mcs) : " << std::endl;
    std::cout << "\tAdding : " << elapsedAddingTime.count() / numOfMeasurements << std::endl;
    std::cout << "\tRemoving : " << elapsedRemoveTime.count() / numOfMeasurements << std::endl;
    //---------------------------------------------------------------------------------------
}


//void measurements() {
//    std::ofstream out_insert("C:\\Users\\dmkuz\\Documents\\Лабы\\САОД\\out_insert.txt", std::ios::app);
//    std::ofstream out_remove("C:\\Users\\dmkuz\\Documents\\Лабы\\САОД\\out_remove.txt", std::ios::app);
//    std::ofstream out_search("C:\\Users\\dmkuz\\Documents\\Лабы\\САОД\\out_find.txt", std::ios::app);
//    if (!out_insert || !out_remove || !out_search) {
//        std::cout << "Cannot open files" << std::endl;
//        exit(EXIT_FAILURE);
//    }
//
//
//    std::vector<int> T_vec = { 4, 5, 8, 12, 15, 17 };
//    for (int k = 0; k < T_vec.size(); k++) {
//        const int T = T_vec[k];
//
//        out_insert << T << std::endl;
//        out_remove << T << std::endl;
//        out_search << T << std::endl;
//
//        std::vector<size_t> el_num = { 1000000, 2000000, 3000000,
//                                       4000000, 5000000, 6000000,
//                                       7000000, 8000000, 9000000,
//                                       10000000, 11000000, 12000000,
//                                       18000000, 19000000, 20000000 };
//
//        for (int i = 0; i < el_num.size(); i++) {
//            size_t el_numb = el_num[i];
//            BTree tr = BTree(T);
//
//            for (int j = 0; j <= el_numb; j++) {
//                tr.insert(j);
//            }
//
//            out_insert << el_numb << std::endl;
//            out_remove << el_numb << std::endl;
//            out_search << el_numb << std::endl;
//
//            std::random_device rd;
//            std::mt19937 gen(rd());
//            std::uniform_int_distribution<> dist(0, el_numb);
//
//
//            for (int p = 0; p < 20; p++) {
//                int random_num = dist(gen);
//
//                auto begin = std::chrono::steady_clock::now();
//                tr.insert(random_num);
//
//                auto end = std::chrono::steady_clock::now();
//                auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin);
//                out_insert << elapsed_ns.count() << " ";
//
//
//                begin = std::chrono::steady_clock::now();
//                tr.search(random_num);
//
//                end = std::chrono::steady_clock::now();
//                elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin);
//                out_search << elapsed_ns.count() << " ";
//
//
//                begin = std::chrono::steady_clock::now();
//                tr.remove(random_num);
//
//                end = std::chrono::steady_clock::now();
//                elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin);
//                out_remove << elapsed_ns.count() << " ";
//            }
//
//            out_insert << std::endl << std::endl;
//            out_remove << std::endl << std::endl;
//            out_search << std::endl << std::endl;
//        }
//    }
//    out_insert.close();
//    out_remove.close();
//    out_search.close();
//}


int main() {
    BTree tree = BTree(3);

    //testing(tree);
    measurements(tree, 50, 20);

    return 0;
}