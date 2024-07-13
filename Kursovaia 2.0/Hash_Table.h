#pragma once

#include <iostream>
#include <vector> 
#include <fstream> 
#include <string> 
#include "Hash_Table_Node.h"
using namespace std;




class Hash_Table {
private:
    const int k1 = 1; // ��������� ��� ������������� ������
    const int k2 = 2; // ��������� ��� ������������� ������
    
    int table_size; // ������� ���������� ��������� � �������
    int max_table_size; // ������� �������

    int hash_middle_square(Key* key) { // ��� ������� �������� ��������
        int sum = key->date.day+key->date.month+key->date.year;
        long long square = static_cast<long long>(sum) * static_cast<long long>(sum); // �������� � �������
        string square_str = to_string(square); // ��������� � ������
        int squared_len = square_str.length(); // ����� �������� �����
        int key_len = to_string(sum).length();; // ����� �����
        int mid_len = key_len / 2; // ����� ��������
        int start_mid_pos = (squared_len / 2) - mid_len / 2; // ������ ��������
        string mid_str = square_str.substr(start_mid_pos, key_len); // ��������
        int hash = stoi(mid_str); // ����������� � �����           
        return hash % max_table_size;
    }

    int quadratic_search(int index, int i) { // ������������ ����� ����� �������
        return (index + (i * k1) + (k2 * i * i)) % max_table_size;
    }

    void rehash() {
        vector<Hash_Table_Node> old_Table = table;
        int old_max_table_size = max_table_size;

        max_table_size = max_table_size * 2; // ����������� ������� � 2 ����
        table.assign(max_table_size, Hash_Table_Node()); // ������ � ����������� �������
        table_size = 0; // �������� ������������ �������

        for (int i = 0; i < old_max_table_size; ++i) { // ��������� �������
            if (old_Table[i].status == ONE) {
                add(old_Table[i].key);
            }
        }
    }

    void Collisions(Key* node, int& index, int& i) { // ���������� ��������
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
    Hash_Table(int initial_max_table_size) : table_size(0), max_table_size(initial_max_table_size) { //�����������.  �������� ������ ��� ��� �������. 
        table.assign(max_table_size, Hash_Table_Node());
    }

    ~Hash_Table() {} // ����������

    int size_HT() {
        return max_table_size;
    }
    vector<Hash_Table_Node> table; // ���-�������

    void add(Key*& node) { // ������� ����

        int index = hash_middle_square(node); // ������� ���
        int i = 0; // ���-�� ������� ��������� ��������

        if (table[index].status != ONE) { // ������ ��������
            table[index].key = node;
            table[index].status = ONE;
            table_size++;
        }
        else {
            Collisions(node, index, i); // ���������� ��������
        }

        if (table_size >= 0.75 * max_table_size) { // �������� �� ������������� ������� 75%
            rehash();
        }
    }

    int count_elements_HT() { // ������� ���������� ���������� �����
        int count = 0;
        for (int i = 0; i < max_table_size; i++) {
            if (table[i].status == ONE) {
                count++;
            }
        }  
        return count;
    }

    bool search(Key* key) { // �����
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
            return false; // ���� �� ������
        }
    }

    bool search_with_steps(Key* key,int& steps) { // �����
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
            return false; // ���� �� ������
        }
    }

    bool delete_node(Key* key) {// ��������                              
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