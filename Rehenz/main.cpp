#include <iostream>
using std::cout;
using std::endl;
#include "rehenz.h"
using namespace Rehenz;

int main()
{
	cout << "Hello~ Rehenz~" << endl;

	float noise[50];
	Perlin1D(123U, 0, 10, 5, noise);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
			cout << noise[i * 5 + j] << " ";
		cout << endl;
	}

	return 0;
}