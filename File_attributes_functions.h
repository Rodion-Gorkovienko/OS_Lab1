#pragma once
#include "Convertation.h"

void print_system_time(const SYSTEMTIME& time) {
	std::cout << time.wYear << '-' << time.wMonth << '-' << time.wDay << ' ' << time.wDayOfWeek << ' ' << time.wHour << ':'
		<< time.wMinute << ':' << time.wSecond << ':' << time.wMilliseconds << std::endl;
}

void print_time(const std::string& time_name, const FILETIME& time) {
	SYSTEMTIME system_time;
	std::cout << time_name << ": ";
	if (FileTimeToSystemTime(&time, &system_time))
		print_system_time(system_time);
	else
		std::cout << "Cannot get time." << std::endl;
}

void get_file_time(std::vector<std::string>* property) {
	HANDLE handle = CreateFile(str2ws((*property)[0]).c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		FILETIME* file_creation_time = new FILETIME;
		FILETIME* file_acess_time = new FILETIME;
		FILETIME* file_write_time = new FILETIME;
		if (GetFileTime(handle, file_creation_time, file_acess_time, file_write_time)) {
			print_time("Creation time", *file_creation_time);
			print_time("Last access time", *file_acess_time);
			print_time("Last write time", * file_write_time);
		}
		else
			std::cout << "Error occurred while getting time info. Error code: " << GetLastError() << std::endl;
		CloseHandle(handle);
		delete file_creation_time;
		delete file_acess_time;
		delete file_write_time;
	}
	else
		std::cout << "Error occurred while opening the file. Error code: " << GetLastError() << std::endl;
}

void print_file_attributes(DWORD file_attributes) {
	std::cout << "File attributes:" << std::endl;
	const std::set<DWORD> s_attributes({
		FILE_ATTRIBUTE_ARCHIVE,
		FILE_ATTRIBUTE_COMPRESSED,
		FILE_ATTRIBUTE_DEVICE,
		FILE_ATTRIBUTE_DIRECTORY,
		FILE_ATTRIBUTE_ENCRYPTED,
		FILE_ATTRIBUTE_HIDDEN,
		FILE_ATTRIBUTE_INTEGRITY_STREAM,
		FILE_ATTRIBUTE_NORMAL,
		FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
		FILE_ATTRIBUTE_NO_SCRUB_DATA,
		FILE_ATTRIBUTE_OFFLINE,
		FILE_ATTRIBUTE_READONLY,
		FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS,
		FILE_ATTRIBUTE_RECALL_ON_OPEN,
		FILE_ATTRIBUTE_REPARSE_POINT,
		FILE_ATTRIBUTE_SPARSE_FILE,
		FILE_ATTRIBUTE_SYSTEM,
		FILE_ATTRIBUTE_TEMPORARY,
		FILE_ATTRIBUTE_VIRTUAL
		});
	for (const DWORD& attribute : s_attributes) {
		if (s_attributes.count(attribute & file_attributes))
			std::cout << "- " << fattr2str(attribute) << std::endl;
	}
}

void get_file_attributes(std::vector<std::string>* property) {
	DWORD file_attributes = GetFileAttributes(str2ws((*property)[0]).c_str());
	if (file_attributes != INVALID_FILE_ATTRIBUTES)
		print_file_attributes(file_attributes);
	else
		std::cout << "Error occurred while openning the file. Error code: " << GetLastError() << std::endl;
}

void get_file_information(std::vector<std::string>* property) {
	HANDLE created = CreateFile(str2ws((*property)[0]).c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, NULL);
	if (created != INVALID_HANDLE_VALUE) {
		BY_HANDLE_FILE_INFORMATION* bhfi = new BY_HANDLE_FILE_INFORMATION;
		if (GetFileInformationByHandle(created, bhfi)) {
			std::cout << "Serial number: " << bhfi->dwVolumeSerialNumber << std::endl;
			print_time("Creation time", bhfi->ftCreationTime);
			print_time("Last access time", bhfi->ftLastAccessTime);
			print_time("Last write time", bhfi->ftLastWriteTime);
			std::cout << "File size: " << (static_cast<long long>(bhfi->nFileSizeHigh) << 32) + static_cast<long long>(bhfi->nFileSizeLow) << std::endl;
			std::cout << "Number of links: " << bhfi->nNumberOfLinks << std::endl;
			std::cout << "Unique identifier: "<< (static_cast<long long>(bhfi->nFileIndexHigh) << 32) + static_cast<long long>(bhfi->nFileIndexLow) << std::endl;
			print_file_attributes(bhfi->dwFileAttributes);
		}
		else
			std::cout << "Error occurred while getting info by handle. Error code: " << GetLastError() << std::endl;
		CloseHandle(created);
		delete bhfi;
	}
	else
		std::cout << "Error occurred while opening the file. Error code: " << GetLastError() << std::endl;
}

void set_file_time(std::vector<std::string>* property) {
	HANDLE handle = CreateFile(str2ws((*property)[0]).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		SYSTEMTIME* system_time = new SYSTEMTIME;
		FILETIME* file_creation_time = new FILETIME;
		FILETIME* file_acess_time = new FILETIME;
		FILETIME* file_write_time = new FILETIME;
		GetSystemTime(system_time);
		if (SystemTimeToFileTime(system_time, file_creation_time) && SystemTimeToFileTime(system_time, file_acess_time) && SystemTimeToFileTime(system_time, file_write_time)) {
			if (SetFileTime(handle, file_creation_time, file_acess_time, file_write_time)) {
				std::cout << "New file time succesfully set." << std::endl;
			}
			else
				std::cout << "Error occurred while setting time info. Error code: " << GetLastError() << std::endl;
		}
		else
			std::cout << "Error occurred while converting time info. Error code: " << GetLastError() << std::endl;
		CloseHandle(handle);
		delete system_time;
		delete file_creation_time;
		delete file_acess_time;
		delete file_write_time;
	}
	else
		std::cout << "Error occurred while opening the file. Error code: " << GetLastError() << std::endl;
}

void set_file_attributes(std::vector<std::string>* property) {
	if (SetFileAttributes(str2ws((*property)[1]).c_str(), str2fattr((*property)[0])))
		std::cout << "New attribute succesfully set." << std::endl;
	else
		std::cout << "Error occurred while setting attribute. Error code: " << GetLastError() << std::endl;
}