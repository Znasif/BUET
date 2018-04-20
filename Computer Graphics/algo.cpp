#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <iostream>
#include <algorithm>

int main() {
	int myints[] = { 1,2,3 };

	std::sort(myints, myints + 3);

	std::cout << "The 3! possible permutations with 3 elements:\n";
	do {
		std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';
	} while (std::next_permutation(myints, myints + 3));

	std::cout << "After loop: " << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';

	return 0;
}