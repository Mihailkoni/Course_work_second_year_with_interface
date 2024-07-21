#pragma once
#include <iostream>
#include "Date.h"
#include "State_Number.h"
#include "Car.h"
#include <string>
using namespace std;

struct Key {
	State_Number state_num; // госномер
	long long phone_num; // номер телефона
	Car car; // марка и модель
	Date date; // дата
	int first_hash_key; // начальный хэш-ключа
};