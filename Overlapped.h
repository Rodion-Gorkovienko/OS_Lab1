#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <windows.h>
#include <timeapi.h>
#include "Convertation.h"
#undef max


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
	int l_file_size,
	BYTE**& buffer,
	OVERLAPPED*& over) 
{
	size_t s_operations_count = 0;
	for (size_t i = 0; i < operations_count; ++i) {
		if (l_file_size > 0) {
			++s_operations_count;
			func(file, buffer[i], block_size, &over[i], FileIOCompletionRoutine);
			l_file_size -= block_size;
		}
	}
	while (callback_count < s_operations_count)
		SleepEx(SIZE_MAX, true);
	for (size_t i = 0; i < operations_count; i++)
		over[i].Offset += block_size * operations_count;
	callback_count = 0;
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

		int l_source_size = 0;
		DWORD* h_source_size = new DWORD;
		l_source_size = GetFileSize(source, h_source_size);
		
		OVERLAPPED* ol_read = new OVERLAPPED[operations_count];
		OVERLAPPED* ol_write = new OVERLAPPED[operations_count];

		for (size_t i = 0; i < operations_count; ++i) {
			ol_write[i].Offset = i * block_size;
			ol_write[i].OffsetHigh = i * *h_source_size;
			ol_read[i].Offset = ol_write[i].Offset;
			ol_read[i].OffsetHigh = ol_write[i].OffsetHigh;
		}

		LARGE_INTEGER start_time, end_time, frequency;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start_time);
		int curr_size = l_source_size;
		do {
			action_file_overlapped(ReadFileEx, source, block_size, operations_count, l_source_size, buffer, ol_read);
			action_file_overlapped(WriteFileEx, copy, block_size, operations_count, l_source_size, buffer, ol_write);
			curr_size -= block_size * operations_count;
		} while (curr_size > 0);
		QueryPerformanceCounter(&end_time);

		std::cout << "Time: " << time_calculation(start_time, end_time, frequency) << " microseconds." << std::endl;;

		SetFilePointer(copy, l_source_size, nullptr, FILE_BEGIN);
		SetEndOfFile(copy);
		CloseHandle(source);
		CloseHandle(copy);

		delete[] ol_read;
		delete[] ol_write;
		for (size_t i = 0; i < operations_count; ++i)
			delete[] buffer[i];
		delete[] buffer;
		delete h_source_size;
	}
}