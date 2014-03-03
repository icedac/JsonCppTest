// #include "stdafx.h"
#include <chrono>
#include <vector>
#include <string>
#include <iostream>

void test_push_back()
{
	const int SIZE = 1000000;
	std::string test_s;
	std::vector< char > test_v;
	test_s.reserve(SIZE + 1);
	test_v.reserve(SIZE + 1);
	auto start = std::chrono::high_resolution_clock::now();
	auto end = start;

	std::cout << "std::string::push_back - ";
	start = std::chrono::high_resolution_clock::now();
	for (int j = 0; j < 100; ++j)
	{
		test_s = "";
		for (int i = 0; i < SIZE; ++i)
		{
			test_s.push_back('a' + (i % 10));
		}
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "]msec\n";

	std::cout << "std::vector<char>::push_back - ";
	start = std::chrono::high_resolution_clock::now();
	for (int j = 0; j < 100; ++j)
	{
		test_v.resize(0);
		for (int i = 0; i < SIZE; ++i)
		{
			test_v.push_back('a' + (i % 10));
		}
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "]msec\n";

	char buf[SIZE + 1];
	char* p = nullptr;
	std::cout << "char* add - ";
	start = std::chrono::high_resolution_clock::now();
	for (int j = 0; j < 100; ++j)
	{
		buf[0] = 0;
		p = &(buf[0]);
		for (int i = 0; i < SIZE; ++i)
		{
			*p++ = ('a' + (i % 10));
		}
		*p = '\0';
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "]msec\n";
}