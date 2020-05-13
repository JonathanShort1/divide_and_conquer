#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>

using namespace std;

/* This function takes last element as pivot, places 
the pivot element at its correct position in sorted 
	array, and places all smaller (smaller than pivot) 
to left of pivot and all greater elements to right 
of pivot */
int partition (vector<int>& arr, int low, int high) 
{ 
	int pivot = arr[high]; // pivot 
	int i = (low - 1); // Index of smaller element 

	for (int j = low; j <= high- 1; j++) 
	{ 
		// If current element is smaller than the pivot 
		if (arr[j] < pivot) 
		{ 
			i++; // increment index of smaller element 
			swap(arr[i], arr[j]); 
		} 
	} 
	swap(arr[i + 1], arr[high]); 
	return (i + 1); 
} 

/* The main function that implements QuickSort 
arr[] --> Array to be sorted, 
low --> Starting index, 
high --> Ending index */
void quickSort(vector<int>& arr, int low, int high) 
{ 
	if (low < high) 
	{ 
		/* pi is partitioning index, arr[p] is now 
		at right place */
		int pi = partition(arr, low, high); 

		// Separately sort elements before 
		// partition and after partition 
		quickSort(arr, low, pi - 1); 
		quickSort(arr, pi + 1, high); 
	}
}

// Driver program to test above functions 
int main() 
{ 
	int n = 1000000;
	
	for (int i = 10000; i < n; i += 10000) {
		vector<int> v(i);
		iota(begin(v), end(v), 0);
		shuffle(begin(v), end(v), mt19937(random_device{}()));

		auto start = chrono::high_resolution_clock::now();

		quickSort(v, 0, i-1);

		auto end = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
		
		cout << "duration: " << duration << endl;
	}
}
