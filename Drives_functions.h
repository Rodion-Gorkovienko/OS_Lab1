#pragma once
#include <windows.h>
#include <set>
#include "Convertation.h"

void print_drives_list(std::vector<std::string>*) {
	std::vector<char> drives_names;
	std::string str;

	DWORD drives = GetLogicalDrives();
	for (unsigned char i = 0; i < 8*sizeof(DWORD); ++i) {
		if (drives & 1) {
			drives_names.push_back(i + 'A');
		}
		drives >>= 1;
	}

	std::cout << "Drives list:" << std::endl;
	for (auto& i : drives_names) {
		std::cout << i << std::endl;
	}
}

void print_drive_type(std::vector<std::string>* drive) {
	switch (GetDriveType(str2ws((*drive)[0]).c_str()))
	{
	case 0:
		std::cout << "The type of device cannot be determined." << std::endl;
		break;
	case 1:
		std::cout << "The root directory does not exist." << std::endl;
		break;
	case DRIVE_REMOVABLE:
		std::cout << (*drive)[0] << " is DRIVE_REMOVABLE." << std::endl;
		break;
	case DRIVE_FIXED:
		std::cout << (*drive)[0] << " is DRIVE_FIXED." << std::endl;
		break;
	case DRIVE_REMOTE:
		std::cout << (*drive)[0] << " is DRIVE_REMOTE." << std::endl;
		break;
	case DRIVE_CDROM:
		std::cout << (*drive)[0] << " is DRIVE_CDROM." << std::endl;
		break;
	case DRIVE_RAMDISK:
		std::cout << (*drive)[0] << " is DRIVE_RAMDISK." << std::endl;
		break;
	default:
		std::cout << "Unknown drive type." << std::endl;
		break;
	}
}

void print_volume_information(std::vector<std::string>* drive) {
	const std::set<DWORD> flags_set({
		FILE_DAX_VOLUME,
		FILE_FILE_COMPRESSION,
		FILE_NAMED_STREAMS,
		FILE_PERSISTENT_ACLS,
		FILE_READ_ONLY_VOLUME,
		FILE_SEQUENTIAL_WRITE_ONCE,
		FILE_SUPPORTS_ENCRYPTION,
		FILE_SUPPORTS_EXTENDED_ATTRIBUTES,
		FILE_SUPPORTS_HARD_LINKS,
		FILE_SUPPORTS_OBJECT_IDS,
		FILE_SUPPORTS_OPEN_BY_FILE_ID,
		FILE_SUPPORTS_REPARSE_POINTS,
		FILE_SUPPORTS_SPARSE_FILES,
		FILE_SUPPORTS_TRANSACTIONS,
		FILE_SUPPORTS_USN_JOURNAL,
		FILE_SUPPORTS_BLOCK_REFCOUNTING,
		FILE_CASE_PRESERVED_NAMES,
		FILE_CASE_SENSITIVE_SEARCH,
		FILE_UNICODE_ON_DISK,
		FILE_VOLUME_IS_COMPRESSED,
		FILE_VOLUME_QUOTAS
	});

	TCHAR volume_name[APIconstants::buff_size];
	DWORD* serial_number = new DWORD;
	*serial_number = 0;
	DWORD* max_component_length = new DWORD;
	*max_component_length = 0;
	DWORD* flags = new DWORD;
	*flags = 0;
	TCHAR file_system_name[APIconstants::buff_size];

	if (GetVolumeInformation(str2ws((*drive)[0]).c_str(), volume_name, APIconstants::buff_size, serial_number, max_component_length, flags, file_system_name, APIconstants::buff_size)) {
		std::wcout << "Volume name: " << volume_name << std::endl;
		std::wcout << "Serial number: " << *serial_number << std::endl;
		std::cout << "Max component length: " << *max_component_length << std::endl;
		std::wcout << "File system name: " << file_system_name << std::endl;
		std::cout << "File system flags:" << std::endl;;
		for (const DWORD& flag : flags_set)
			if (flags_set.count(flag & *flags))
				std::cout << "- " << dflag2sring(flag) << std::endl;
	}
	else
		std::cout << "The root directory does not exist." << std::endl;
	delete serial_number;
	delete max_component_length;
	delete flags;
}

void print_disk_free_space(std::vector<std::string>* drive) {
	DWORD* sectors_per_cluster = new DWORD;
	*sectors_per_cluster = 0;
	DWORD* bytes_per_sector = new DWORD;
	*bytes_per_sector = 0;
	DWORD* number_of_free_clusters = new DWORD;
	*number_of_free_clusters = 0;
	DWORD* total_number_of_clusters = new DWORD;
	*total_number_of_clusters = 0;

	GetDiskFreeSpace(str2ws((*drive)[0]).c_str(), sectors_per_cluster, bytes_per_sector, number_of_free_clusters, total_number_of_clusters);
	std::cout << "Sectors per cluster: " << *sectors_per_cluster << std::endl;
	std::cout << "Bytes per sector: " << *bytes_per_sector << std::endl;
	std::cout << "Number of free clusters: " << *number_of_free_clusters << std::endl;
	std::cout << "Total number of clusters: " << *total_number_of_clusters << std::endl;

	delete sectors_per_cluster;
	delete bytes_per_sector;
	delete number_of_free_clusters;
	delete total_number_of_clusters;
}