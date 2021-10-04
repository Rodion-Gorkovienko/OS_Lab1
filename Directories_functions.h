#pragma once
#include <windows.h>
#include "Convertation.h"

void create_directory(std::vector<std::string>* directory) {
	if (CreateDirectory(str2ws((*directory)[0]).c_str(), nullptr))
		std::cout << "The directory at path " << (*directory)[0] << " has been successfully created" << std::endl;
	else
		std::cout << "Error occurred while creating the directory. Error code: " << GetLastError() << std::endl;
}

void remove_directory(std::vector<std::string>* directory) {
	if (RemoveDirectory(str2ws((*directory)[0]).c_str()))
		std::cout << "The directory at path " << (*directory)[0] << " has been successfully removed." << std::endl;
	else
		std::cout << "Error occurred while removing the directory. Error code: " << GetLastError() << std::endl;;
}