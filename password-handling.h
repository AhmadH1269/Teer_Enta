#pragma once

#include<string>
using namespace std;

extern int primes[];

long long passwordHash(string password);
int passwordStrengthIndicator(string password);