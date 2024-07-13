#pragma once

#include <iostream>
#include <vector> 
#include <fstream> 
#include <string> 
#include "Hash_Table_Node.h"
using namespace std;




class Hash_Table {
private:
    const int k1 = 1; // константа для квадратичного поиска
    const int k2 = 2; // константа для квадратичного поиска
    
    int table_size; // текущее количество элементов в таблице
    int max_table_size; // ёмкость таблицы

    int hash_middle_square(Key* key) { // хэш функция середина квадрата
        int sum = key->date.day+key->date.month+key->date.year;
        long long square = static_cast<long long>(sum) * static_cast<long long>(sum); // возводим в квадрат
        string square_str = to_string(square); // переводим в строку
        int squared_len = square_str.length(); // длина квадрата ключа
        int key_len = to_string(sum).length();; // длина ключа
        int mid_len = key_len / 2; // длина середины
        int start_mid_pos = (squared_len / 2) - mid_len / 2; // начало середины
        string mid_str = square_str.substr(start_mid_pos, key_len); // середина
        int hash = stoi(mid_str); // преобразуем в число           
        return hash % max_table_size;
    }

    int quadratic_search(int index, int i) { // квадратичный поиск места вставки
        return (index + (i * k1) + (k2 * i * i)) % max_table_size;
    }

    void rehash() {
        vector<Hash_Table_Node> old_Table = table;
        int old_max_table_size = max_table_size;

        max_table_size = max_table_size * 2; // Увеличиваем ёмкость в 2 раза
        table.assign(max_table_size, Hash_Table_Node()); // чистим и увеличиваем таблицу
        table_size = 0; // обнуляем заполненость таблицы

        for (int i = 0; i < old_max_table_size; ++i) { // заполняем таблицу
            if (old_Table[i].status == ONE) {
                add(old_Table[i].key);
            }
        }
    }

    void Collisions(Key* node, int& index, int& i) { // разрешение коллизий
        int old_index = index;
        while (i < max_table_size) {
           /* if ((table[index].key->date.day == node->date.day) && (table[index].key->date.month == node->date.month) && (table[index].key->date.year == node->date.year)) {
                return;
            }*/
            if (table[index].status != ONE || ((table[index].key->date.day == node->date.day) && (table[index].key->date.month == node->date.month) && (table[index].key->date.year == node->date.year))) {
                table[index].key = node;
                table[index].status = ONE;
                table_size++;
                return;
            }
            else {
                index = quadratic_search(index, i++);
            }
        }
        rehash();
        index = old_index;
        i = 0;
        add(node);
    }

public:
    Hash_Table(int initial_max_table_size) : table_size(0), max_table_size(initial_max_table_size) { //Конструктор.  Исходный размер для хэш таблицы. 
        table.assign(max_table_size, Hash_Table_Node());
    }

    ~Hash_Table() {} // Деструктор

    int size_HT() {
        return max_table_size;
    }
    vector<Hash_Table_Node> table; // хэш-таблица

    void add(Key*& node) { // вставка узла

        int index = hash_middle_square(node); // считаем хэш
        int i = 0; // кол-во попыток разрешить коллизию

        if (table[index].status != ONE) { // первая проверка
            table[index].key = node;
            table[index].status = ONE;
            table_size++;
        }
        else {
            Collisions(node, index, i); // разрешение коллизий
        }

        if (table_size >= 0.75 * max_table_size) { // проверка на заполняемость таблицы 75%
            rehash();
        }
    }

    int count_elements_HT() { // считаем количество заполненых ячеек
        int count = 0;
        for (int i = 0; i < max_table_size; i++) {
            if (table[i].status == ONE) {
                count++;
            }
        }  
        return count;
    }

    bool search(Key* key) { // поиск
        int index = hash_middle_square(key);
        int i = 0;

        if (table[index].status == ONE && ((table[index].key->state_num.region == key->state_num.region) && (table[index].key->state_num.number == key->state_num.number) && (table[index].key->state_num.letters == key->state_num.letters) && (table[index].key->phone_num == key->phone_num) && (table[index].key->car.model == key->car.model) && (table[index].key->car.brand == key->car.brand) && (table[index].key->date.day == key->date.day) && (table[index].key->date.month == key->date.month) && (table[index].key->date.year == key->date.year))) {
            return true;
        }
        else {
            while (table[index].status != ZERO && ( (table[index].key->state_num.region != key->state_num.region) || (table[index].key->state_num.number != key->state_num.number) || (table[index].key->state_num.letters != key->state_num.letters) || (table[index].key->phone_num != key->phone_num) || (table[index].key->car.model != key->car.model) || (table[index].key->car.brand != key->car.brand) || (table[index].key->date.day != key->date.day) || (table[index].key->date.month != key->date.month) || (table[index].key->date.year != key->date.year))) {
                index = quadratic_search(index, i++);
                if (i > max_table_size) {
                    return false;
                }
                if (table[index].status == ONE && ((table[index].key->state_num.region == key->state_num.region) && (table[index].key->state_num.number == key->state_num.number) && (table[index].key->state_num.letters == key->state_num.letters) && (table[index].key->phone_num == key->phone_num) && (table[index].key->car.model == key->car.model) && (table[index].key->car.brand == key->car.brand) && (table[index].key->date.day == key->date.day) && (table[index].key->date.month == key->date.month) && (table[index].key->date.year == key->date.year))) {
                    return true;
                }

            }
            return false; // Ключ не найден
        }
    }

    bool search_with_steps(Key* key,int& steps) { // поиск
        int index = hash_middle_square(key);
        int i = 0;

        if (table[index].status == ONE && ((table[index].key->date.day == key->date.day) && (table[index].key->date.month == key->date.month) && (table[index].key->date.year == key->date.year))) {
            return true;
        }
        else {
            while (table[index].status != ZERO && ((table[index].key->date.day != key->date.day) || (table[index].key->date.month != key->date.month) || (table[index].key->date.year != key->date.year))) {
                index = quadratic_search(index, i++);
                steps++;
                if (i > max_table_size) {
                    return false;
                }
                if (table[index].status == ONE && ((table[index].key->date.day == key->date.day) && (table[index].key->date.month == key->date.month) && (table[index].key->date.year == key->date.year))) {
                    return true;
                }

            }
            return false; // Ключ не найден
        }
    }

    bool delete_node(Key* key) {// Удаление                              
        int index = hash_middle_square(key);
        int i = 0;
        if (table[index].status == ONE && ((table[index].key->car.brand == key->car.brand) && (table[index].key->car.model == key->car.model) && (table[index].key->date.day == key->date.day) && (table[index].key->date.month == key->date.month) && (table[index].key->date.year == key->date.year) && (table[index].key->phone_num == key->phone_num) && (table[index].key->state_num.letters == key->state_num.letters) && (table[index].key->state_num.number == key->state_num.number) && (table[index].key->state_num.region == key->state_num.region))) {
            table[index].status = TWO;
            table_size--;
            return true;
        }
        else {
            while (table[index].status != ZERO && ((table[index].key->car.brand != key->car.brand) || (table[index].key->car.model != key->car.model) || (table[index].key->date.day != key->date.day) || (table[index].key->date.month != key->date.month) || (table[index].key->date.year != key->date.year) || (table[index].key->phone_num != key->phone_num) || (table[index].key->state_num.letters != key->state_num.letters) || (table[index].key->state_num.number != key->state_num.number) || (table[index].key->state_num.region != key->state_num.region))) {

                index = quadratic_search(index, i++);
                if (i > max_table_size) {
                    return false;
                }
                if (table[index].status == ONE && ((table[index].key->car.brand == key->car.brand) && (table[index].key->car.model == key->car.model) && (table[index].key->date.day == key->date.day) && (table[index].key->date.month == key->date.month) && (table[index].key->date.year == key->date.year) && (table[index].key->phone_num == key->phone_num) && (table[index].key->state_num.letters == key->state_num.letters) && (table[index].key->state_num.number == key->state_num.number) && (table[index].key->state_num.region == key->state_num.region))) {
                    table[index].status = TWO;
                    table_size--;
                    return true;
                }
            }
            return false;
        }
    }
};