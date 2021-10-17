#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <windows.h>
#include <timeapi.h>
#include "Convertation.h"


size_t callback_count = 0;
VOID CALLBACK FileIOCompletionRoutine(DWORD error, DWORD number_of_bytes, LPOVERLAPPED p_over) { ++callback_count; }

BYTE** get_new_buffer(int a, int b) {
	BYTE** buffer = new BYTE * [a];
	for (size_t i = 0; i < a; ++i)
		buffer[i] = new BYTE[b];
	return buffer;
}

int getnum() {
	int i;
	char c;
	while (!(std::cin >> i))
		std::cout << "Invalid input. Try again:" << std::endl;
	return i;
}

void action_file_overlapped(
	const std::function<BOOL(HANDLE, LPVOID, DWORD, LPOVERLAPPED, LPOVERLAPPED_COMPLETION_ROUTINE)>& func,
	const HANDLE& file,
	const size_t& block_size,
	const size_t& operations_count,
	int64_t full_source_size,
	BYTE**& buffer,
	OVERLAPPED*& over) 
{
	size_t s_operations_count = 0;
	for (size_t i = 0; i < operations_count && full_source_size > 0; ++i) {
		++s_operations_count;
		func(file, buffer[i], block_size, &over[i], FileIOCompletionRoutine);
		full_source_size -= block_size;
	}

	while (callback_count < s_operations_count)
		SleepEx(SIZE_MAX, true);
	callback_count = 0;
}

void offset_refresh(int op_count, int block_size, int64_t* last_offset, OVERLAPPED* ol_read, OVERLAPPED* ol_write) {
	for (size_t i = 0; i < op_count; i++) {
		ol_write[i].Offset = static_cast<uint32_t>(*last_offset);
		ol_write[i].OffsetHigh = static_cast<uint32_t>(*last_offset >> 32);
		ol_read[i].Offset = ol_write[i].Offset;
		ol_read[i].OffsetHigh = ol_write[i].OffsetHigh;
		*last_offset += block_size;
	}
}

LONGLONG time_calculation(const LARGE_INTEGER& start, const LARGE_INTEGER& end, const LARGE_INTEGER& freq) {
	LARGE_INTEGER res;
	res.QuadPart = end.QuadPart - start.QuadPart;
	res.QuadPart *= 1000000;
	res.QuadPart /= freq.QuadPart;
	return res.QuadPart;
}

void emergency_closure(HANDLE source, HANDLE copy) {
	if (source == INVALID_HANDLE_VALUE) std::cout << "Source file was not opened. Error code: " << GetLastError() << std::endl;
	else CloseHandle(source);
	if (copy == INVALID_HANDLE_VALUE) std::cout << "File for copy was not created. Error code: " << GetLastError() << std::endl;
	else CloseHandle(copy);
}

void overlaped_copy(std::vector<std::string>* property) {
	std::string source_path;
	std::string copy_path;

	HANDLE source = CreateFile(str2ws((*property)[1]).c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
	HANDLE copy = CreateFile(str2ws((*property)[0]).c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

	if (source == INVALID_HANDLE_VALUE || copy == INVALID_HANDLE_VALUE) {
		emergency_closure(source, copy);
	}
	else {
		std::cout << "Enter block size:" << std::endl;
		size_t block_size = getnum()* APIconstants::block_base;

		std::cout << "Enter operations count:" << std::endl;
		size_t operations_count = getnum();

		BYTE** buffer = get_new_buffer(operations_count, block_size);

		uint32_t l_source_size = 0;
		DWORD* h_source_size = new DWORD;
		l_source_size = GetFileSize(source, h_source_size);
		int64_t* full_source_size = new int64_t;
		int64_t* last_offset = new int64_t;
		*full_source_size = (static_cast<long long>(*h_source_size) << 32) + static_cast<long long>(l_source_size);
		*last_offset = 0;


		OVERLAPPED* ol_read = new OVERLAPPED[operations_count];
		OVERLAPPED* ol_write = new OVERLAPPED[operations_count];

		offset_refresh(operations_count, block_size, last_offset, ol_read, ol_write);

		LARGE_INTEGER start_time, end_time, frequency;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start_time);
		do {
			action_file_overlapped(ReadFileEx, source, block_size, operations_count, *full_source_size, buffer, ol_read);
			action_file_overlapped(WriteFileEx, copy, block_size, operations_count, *full_source_size, buffer, ol_write);
			offset_refresh(operations_count, block_size, last_offset, ol_read, ol_write);
			*full_source_size -= block_size * operations_count;
		} while (*full_source_size > 0);
		QueryPerformanceCounter(&end_time);

		std::cout << "Time: " << time_calculation(start_time, end_time, frequency) << " microseconds." << std::endl;;

		long h = static_cast<uint32_t>(*h_source_size);
		SetFilePointer(copy, l_source_size, &h, FILE_BEGIN);
		SetEndOfFile(copy);
		CloseHandle(source);
		CloseHandle(copy);

		delete[] ol_read;
		delete[] ol_write;
		for (size_t i = 0; i < operations_count; ++i)
			delete[] buffer[i];
		delete[] buffer;
		delete h_source_size;
		delete full_source_size;
		delete last_offset;
	}
}
