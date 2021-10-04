#pragma once

#include "Confirmation.h"

void create_file(std::vector<std::string>* property) {
	HANDLE created = CreateFile(str2ws((*property)[0]).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (created != INVALID_HANDLE_VALUE) {
		std::cout << "The file was created successfully." << std::endl;
		CloseHandle(created);
	}
	else
		std::cout << "Error occurred while creating the file. Error code: " << GetLastError() << std::endl;
}

void move_file(std::vector<std::string>* property) {
	if (MoveFile(str2ws((*property)[1]).c_str(), str2ws((*property)[0]).c_str()))
		std::cout << "The file was moved successfully." << std::endl;
	else {
		DWORD error_code = GetLastError();
		if (error_code == ERROR_FILE_EXISTS || error_code == ERROR_ALREADY_EXISTS) {
			std::cout << "The file already exists. Overwrite?" << std::endl;
			char choice = confirmation();
			if (choice == 'Y' || choice == 'y') {
				if (MoveFileEx(str2ws((*property)[1]).c_str(), str2ws((*property)[0]).c_str(), MOVEFILE_REPLACE_EXISTING))
					std::cout << "The file was moved successfully." << std::endl;
				else
					std::cout << "Error occurred while moving the file. Error code: " << GetLastError() << std::endl;
			}
			else if (choice == 'N' || choice == 'n')
				std::cout << "The file was not moved." << std::endl;
		}
		else
			std::cout << "Error occurred while moving the file. Error code: " << GetLastError() << std::endl;
	}
}

void copy_file(std::vector<std::string>* property) {
	if (CopyFile(str2ws((*property)[1]).c_str(), str2ws((*property)[0]).c_str(), true))
		std::cout << "The file was copied successfully." << std::endl;
	else {
		DWORD error_code = GetLastError();
		if (error_code == ERROR_FILE_EXISTS || error_code == ERROR_ALREADY_EXISTS) {
			std::cout << "The file already exists. Overwrite?" << std::endl;
			char choice = confirmation();
			if (choice == 'Y' || choice == 'y') {
				if (CopyFile(str2ws((*property)[1]).c_str(), str2ws((*property)[0]).c_str(), false))
					std::cout << "The file was copied successfully." << std::endl;
				else
					std::cout << "Error occurred while coping the file. Error code: " << GetLastError() << std::endl;
			}
			else if (choice == 'N' || choice == 'n')
				std::cout << "The file was not copied." << std::endl;
		}
		else
			std::cout << "Error occurred while coping the file. Error code: " << GetLastError() << std::endl;
	}
}
