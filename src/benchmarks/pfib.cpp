#include <iostream>
#include <chrono>

using namespace std;

unsigned long long Fib(unsigned long long n) {
	if (n <= 2) return 1;

	return Fib(n-1) + Fib(n-2);
}

int main() {
	// you will need to modify all parameters, etc.
	int n = 40;

	for (int i = 0; i < n; i++) {

		auto start = chrono::high_resolution_clock::now();

		Fib(i);

		auto end = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
		
		cout << "size: " << i << " duration: " << duration << endl;
	}
}



