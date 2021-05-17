#include <iostream>
#include <cstdlib>
#include <fstream> 
#include <string>
#include <algorithm> 
#include <iomanip>
#include <math.h>
#include <time.h>

#define h_X 33.40819 //Hotel X coordinate
#define h_Y 39.19001 // Hotel Y coordinate
using namespace std;
const int tree_array_size = 1000005; // Define array size
int heap_Size = 0;					 // Element number for heap array (myArr)

void min_heapify(double array[], int index)
{
	int left_child_index = 2 * index;		 //Get the left child index
	int right_child_index = (2 * index) + 1; //Get the right child index

	int smallest = index; // finding smallest among index, left child and right child

	if (left_child_index <= heap_Size)
	{
		if (array[left_child_index] < array[smallest])
		{								 // If left child is smaller than smalest element than swap them
			smallest = left_child_index; // New smallest is left child
		}
	}

	if (right_child_index <= heap_Size)
	{
		if (array[right_child_index] < array[smallest])
		{								  // If right child is smaller than smalest element than swap them
			smallest = right_child_index; // New smallest is right child
		}
	}

	if (smallest != index) // If minumum is not root
	{ 
		swap(array[index], array[smallest]); 
		min_heapify(array, smallest); // Recursively heapify the affected sub-tree
	}
}

void delete_root(double array[])
{								 // Delete root function
	double minm = array[1];		 // Get the root of the tree which is smallest element
	array[1] = array[heap_Size]; // Get the last element in the root. Now root is last element
	heap_Size--;				 // Decrease the size of array
	min_heapify(array, 1);		 // Send to the heapify function because root is not smallest
	cout << "Distance of called taxi :" << minm << endl;
}

void shiftUp(double array[], int index, double key)
{						
	array[index] = key; //Update the element which index is index
	while ((index > 1) && (array[index / 2] > array[index])) //If parent is greater than the child than swap them
	{										  
		swap(array[index], array[index / 2]); // Swap parent and child
		index = index / 2;					  // Than, go up root
	}
}

void insert(double array[], double key)
{
	heap_Size++; //Increase before insert because it is better for us for index operations in tree operations
	shiftUp(array, heap_Size, key);
}

int frequency_of_primes(int n)
{
	int i, j;
	int freq = n - 1;
	for (i = 2; i <= n; ++i)
		for (j = sqrt(i); j > 1; --j)
			if (i % j == 0)
			{
				--freq;
				break;
			}
	return freq;
}

int main(int argc, char *argv[])
{

	ifstream file;
	file.open("locations.txt");

	if (!file)
	{
		cerr << "File cannot be opened!";
		exit(1);
	}

	srand(time(NULL));
	const int m = atof(argv[1]); 
	const double p = atof(argv[2]); 

	int operation = 0; // Operation counter
	int updated = 0;   // Updated counter
	int added = 0;	   // Added counter
	string line;
	double *myArr = new double[tree_array_size]; // Dynamic memory allocation
	double X, Y;								 // Ýnput longitude and latitude
	getline(file, line);						 // this is the header line
	clock_t t;
	int f;
	t = clock();
	cout << "Calculating running time...\n"
		 << endl;
	f = frequency_of_primes(99999);
	for (int i = 0; i < m; i++)
	{												   // For loop m times
		bool Probability = (rand() % 100) < p*100;     // Probabilty of p
		if (!Probability)
		{ // If probability is 1-p
			file >> X;
			file >> Y;
			double x = h_X - X; // (x1-x2)
			double y = h_Y - Y; // (y1-y2)
			double dist;
			dist = pow(x, 2) + pow(y, 2); //calculating Euclidean distance
			dist = sqrt(dist);
			insert(myArr, dist);
			getline(file, line, '\n'); // this is for reading the \n character into dummy variable.
			added++;
			operation++;
		}
		else
		{ // If probability is p
			if (heap_Size >= 1)
			{															   // If first operation is update than continue because there is no element. So can not updated.
				int rand_index = rand() % heap_Size + 1;				   // Get the random index
				shiftUp(myArr, rand_index, myArr[rand_index] - 0.01); // Decrease random element
				operation++;
				updated++;
			}
		}
		if ((heap_Size >= 1) && (operation % 100 == 0))
		{						// Delete every 100 operations
			delete_root(myArr); // Delete root
			operation++;
		}
	}
	cout << "Total added taxis : " << added << endl
		 << "Total updated taxis : " << updated << endl; // Cout total added and updated taxis
	t = clock() - t;
	printf("It took me %d clicks (%f miliseconds).\n", t, ((((float)t) / CLOCKS_PER_SEC) / 1000));
	delete[] myArr; // delete array
	return 0;
}
