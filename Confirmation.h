#pragma once
#include <iostream>
#include <conio.h>

char confirmation() {
	unsigned char in;
	do {
		std::cout << "Y - yes, N - no: ";
		in = _getch();
		if (in != 'y' && in !='Y' && in != 'n' && in != 'N')
		{
			std::cout << std::endl << "Invalid input. Try again." << std::endl;
		}
	} while (in != 'y' && in != 'Y' && in != 'n' && in != 'N');
	std::cout << std::endl;
	return in;
}