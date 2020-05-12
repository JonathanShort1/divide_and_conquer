#include <iostream>
#include <chrono>

using namespace std;

unsigned long long Fib(unsigned long long n) {
	if (n <= 2) return 1;

	return Fib(n-1) + Fib(n-2);
}

int main() {
	// you will need to modify all parameters, etc.
	unsigned long long n = 9;

	auto start = chrono::high_resolution_clock::now();

	unsigned long long res = Fib(n);

	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();


	cout << "Fibbonacci of " << n << ": " << res << endl;
	cout << "time: " << duration << endl;

}



