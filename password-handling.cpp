#include "password-handling.h"

int primes[] = {                                                    //First 60 prime numbers: used for hashing 
	2, 3, 5, 7, 11, 13, 17,
	19, 23, 29, 31, 37, 41,
	43, 47, 53, 59, 61, 67,
	71, 73, 79, 83, 89, 97,
	101, 103, 107, 109, 113,
	127, 131, 137, 139, 149,
	151, 157, 163, 167, 173,
	179, 181, 191, 193, 197,
	199, 211, 223, 227, 229,
	233, 239, 241, 251, 257,
	263, 269, 271, 277, 281
};
//-------------- Will use the strength indicator better --------------
//bool validPassword(string password) {
//	bool num = false, chr = false;
//	for (auto& i : password)										// Itirate all over the password
//		if (isalnum(i) && !isalpha(i)) num = true;					// i is a number
//		else chr = true;											// Since i isn't a number therefore it's a character
//	return num && chr && password.size() >= 8;						// True: if password meets required procedures or False: if password does not
//}

long long passwordHash(string password) {
	long long hash = 0;											// Initial hash value of zero
	long long mod = 1e9 + 21;										// Large prime number to avoid 
	for (int i = 0; i < password.size(); i++) {
		hash = (hash + (password[i] * primes[59 - i % 60])) % mod;  // Multiplying char values by prime numbers to reduce collision via having multiple divisors
	}
	return hash;
}

int passwordStrengthIndicator(string password) {

	if (password.size() < 8) return 0;								// Has to be at least 8 characters

	int strength = 0;

	if (password.size() >= 12) strength += 2;						// If size 12 or above at two points
	else strength++;												// If size 8~11 add only one point	

	bool sequence{}, seq{}, repetition{}, rep{}, specialChr{}, number{}, upper{}, lower{};

	for (int i = 0; i < password.size(); i++) {
		if (!upper && isupper(password[i])) { strength++; upper = true; }				// If it has an upper case letter add a point
		if (!lower && islower(password[i])) { strength++; lower = true; }				// If it has a lower case lateer add a point
		if (!number && isdigit(password[i])) { strength++; number = true; }				// If it has a digit add a point
		if (!specialChr && !isalnum(password[i])) { strength++; specialChr = true; }	// If it has a special character (not a letter or a digit) add a point


		if (i && !seq && (password[i] == password[i - 1] + 1 || password[i] == password[i - 1] - 1)) {
			if (sequence) { strength--; seq = true; }									// Removes a point if the password was ascendingly
			else sequence = true;														// or descendingly sequencial (ASCII-based) for three characters in a row
		}
		else sequence = false;


		if (i && !rep && password[i] == password[i - 1]) {
			if (repetition) { strength--; rep = true; }									// Removes a point if the character was repeated
			else repetition = true;														// three times in a row
		}
		else repetition = false;
	}
	if (specialChr && number && upper && lower) {					// If it meets all the good checks
		if (password.size() >= 12) strength += 2;					// And above 12 letters long add two points
		else strength++;											// Else just add one
	}

	return max(strength, 0);										// Return 0 if strength is negative
}
//0~8
//0~2 very weak
//3~4 weak
//5~6 strong
//7~8 very strong

/*
// FOR TESTING
#include<iostream> // This works dw
int main() {

	// Enter the password and it will show you the strength of each substring as if you're typing it letter by letter

	string s;

	for (int i = 0; i < 100; i++) {
		cout << "Enter Password: ";
		cin >> s;
		string ss = "";

		for (auto& character : s) {
			ss += character;
			cout << passwordStrengthIndicator(ss) << " ";
		}

		cout << endl;
	}
}
*/