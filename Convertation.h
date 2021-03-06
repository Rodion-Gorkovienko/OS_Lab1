#pragma once

namespace APIconstants {
	const size_t block_base{ 4096 };
	const size_t latin_al_size{ 26 };
	const size_t buff_size{ 256 };
	const size_t drive_path_size{ 4 };
}

DWORD str2fattr(std::string attribute) {
	if (attribute == "FILE_ATTRIBUTE_ARCHIVE") return FILE_ATTRIBUTE_ARCHIVE;
	else if(attribute == "FILE_ATTRIBUTE_COMPRESSED") return FILE_ATTRIBUTE_COMPRESSED;
	else if(attribute == "FILE_ATTRIBUTE_DEVICE") return FILE_ATTRIBUTE_DEVICE;
	else if(attribute == "FILE_ATTRIBUTE_DIRECTORY") return FILE_ATTRIBUTE_DIRECTORY;
	else if(attribute == "FILE_ATTRIBUTE_ENCRYPTED") return FILE_ATTRIBUTE_ENCRYPTED;
	else if(attribute == "FILE_ATTRIBUTE_HIDDEN") return FILE_ATTRIBUTE_HIDDEN;
	else if(attribute == "FILE_ATTRIBUTE_INTEGRITY_STREAM") return FILE_ATTRIBUTE_INTEGRITY_STREAM;
	else if(attribute == "FILE_ATTRIBUTE_NORMAL") return FILE_ATTRIBUTE_NORMAL;
	else if(attribute == "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED") return FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
	else if(attribute == "FILE_ATTRIBUTE_NO_SCRUB_DATA") return FILE_ATTRIBUTE_NO_SCRUB_DATA;
	else if(attribute == "FILE_ATTRIBUTE_OFFLINE") return FILE_ATTRIBUTE_OFFLINE;
	else if(attribute == "FILE_ATTRIBUTE_READONLY") return FILE_ATTRIBUTE_READONLY;
	else if(attribute == "FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS") return FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS;
	else if(attribute == "FILE_ATTRIBUTE_RECALL_ON_OPEN") return FILE_ATTRIBUTE_RECALL_ON_OPEN;
	else if(attribute == "FILE_ATTRIBUTE_REPARSE_POINT") return FILE_ATTRIBUTE_REPARSE_POINT;
	else if(attribute == "FILE_ATTRIBUTE_SPARSE_FILE") return FILE_ATTRIBUTE_SPARSE_FILE;
	else if(attribute == "FILE_ATTRIBUTE_SYSTEM") return FILE_ATTRIBUTE_SYSTEM;
	else if(attribute == "FILE_ATTRIBUTE_TEMPORARY") return FILE_ATTRIBUTE_TEMPORARY;
	else if(attribute == "FILE_ATTRIBUTE_VIRTUAL") return FILE_ATTRIBUTE_VIRTUAL;
	else return 0;
}

std::string fattr2str(DWORD attribute) {
	switch (attribute)
	{
	case FILE_ATTRIBUTE_ARCHIVE: return "FILE_ATTRIBUTE_ARCHIVE";
	case FILE_ATTRIBUTE_COMPRESSED: return "FILE_ATTRIBUTE_COMPRESSED";
	case FILE_ATTRIBUTE_DEVICE: return "FILE_ATTRIBUTE_DEVICE";
	case FILE_ATTRIBUTE_DIRECTORY: return "FILE_ATTRIBUTE_DIRECTORY";
	case FILE_ATTRIBUTE_ENCRYPTED: return "FILE_ATTRIBUTE_ENCRYPTED";
	case FILE_ATTRIBUTE_HIDDEN: return "FILE_ATTRIBUTE_HIDDEN";
	case FILE_ATTRIBUTE_INTEGRITY_STREAM: return "FILE_ATTRIBUTE_INTEGRITY_STREAM";
	case FILE_ATTRIBUTE_NORMAL: return "FILE_ATTRIBUTE_NORMAL";
	case FILE_ATTRIBUTE_NOT_CONTENT_INDEXED: return "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED";
	case FILE_ATTRIBUTE_NO_SCRUB_DATA: return "FILE_ATTRIBUTE_NO_SCRUB_DATA";
	case FILE_ATTRIBUTE_OFFLINE: return "FILE_ATTRIBUTE_OFFLINE";
	case FILE_ATTRIBUTE_READONLY: return "FILE_ATTRIBUTE_READONLY";
	case FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS: return "FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS";
	case FILE_ATTRIBUTE_RECALL_ON_OPEN: return "FILE_ATTRIBUTE_RECALL_ON_OPEN";
	case FILE_ATTRIBUTE_REPARSE_POINT: return "FILE_ATTRIBUTE_REPARSE_POINT";
	case FILE_ATTRIBUTE_SPARSE_FILE: return "FILE_ATTRIBUTE_SPARSE_FILE";
	case FILE_ATTRIBUTE_SYSTEM: return "FILE_ATTRIBUTE_SYSTEM";
	case FILE_ATTRIBUTE_TEMPORARY: return "FILE_ATTRIBUTE_TEMPORARY";
	case FILE_ATTRIBUTE_VIRTUAL: return "FILE_ATTRIBUTE_VIRTUAL";
	default: return "UNINTENDED_VALUE";
	}
}


std::string dflag2sring(DWORD flag) {
	switch (flag)
	{
	case FILE_DAX_VOLUME: return "FILE_DAX_VOLUME";
	case FILE_FILE_COMPRESSION: return "FILE_FILE_COMPRESSION";
	case FILE_NAMED_STREAMS: return "FILE_NAMED_STREAMS";
	case FILE_PERSISTENT_ACLS: return "FILE_PERSISTENT_ACLS";
	case FILE_READ_ONLY_VOLUME: return "FILE_READ_ONLY_VOLUME";
	case FILE_SEQUENTIAL_WRITE_ONCE: return "FILE_SEQUENTIAL_WRITE_ONCE";
	case FILE_SUPPORTS_ENCRYPTION: return "FILE_SUPPORTS_ENCRYPTION";
	case FILE_SUPPORTS_EXTENDED_ATTRIBUTES: return "FILE_SUPPORTS_EXTENDED_ATTRIBUTES";
	case FILE_SUPPORTS_HARD_LINKS: return "FILE_SUPPORTS_HARD_LINKS";
	case FILE_SUPPORTS_OBJECT_IDS: return "FILE_SUPPORTS_OBJECT_IDS";
	case FILE_SUPPORTS_OPEN_BY_FILE_ID: return "FILE_SUPPORTS_OPEN_BY_FILE_ID";
	case FILE_SUPPORTS_REPARSE_POINTS: return "FILE_SUPPORTS_REPARSE_POINTS";
	case FILE_SUPPORTS_SPARSE_FILES: return "FILE_SUPPORTS_SPARSE_FILES";
	case FILE_SUPPORTS_TRANSACTIONS: return "FILE_SUPPORTS_TRANSACTIONS";
	case FILE_SUPPORTS_USN_JOURNAL: return "FILE_SUPPORTS_USN_JOURNAL";
	case FILE_SUPPORTS_BLOCK_REFCOUNTING: return "FILE_SUPPORTS_BLOCK_REFCOUNTING";
	case FILE_CASE_PRESERVED_NAMES: return "FILE_CASE_PRESERVED_NAMES";
	case FILE_CASE_SENSITIVE_SEARCH: return "FILE_CASE_SENSITIVE_SEARCH";
	case FILE_UNICODE_ON_DISK: return "FILE_UNICODE_ON_DISK";
	case FILE_VOLUME_IS_COMPRESSED: return "FILE_VOLUME_IS_COMPRESSED";
	case FILE_VOLUME_QUOTAS: return "FILE_VOLUME_QUOTAS";
	default: return "UNINTENDED_VALUE";
	}
}

std::wstring str2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	std::wstring buf;
	buf.resize(len);
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength,
		const_cast<wchar_t*>(buf.c_str()), len);
	return buf;
}