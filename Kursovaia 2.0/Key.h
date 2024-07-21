#pragma once
#include <iostream>
#include "Date.h"
#include "State_Number.h"
#include "Car.h"
#include <string>
using namespace std;

struct Key {
	State_Number state_num; // ��������
	long long phone_num; // ����� ��������
	Car car; // ����� � ������
	Date date; // ����
	int first_hash_key; // ��������� ���-�����
};