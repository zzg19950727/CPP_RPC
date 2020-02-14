#include "add.h"
#include <iostream>
#include <thread>

int add(int a, int b, int c)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	return a + b + c;
}

int sub(int a, int b)
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(30));
	return a-b;
}

float mul(float a, float b)
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(30));
	return a*b;
}

double div(double a, double b)
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(30));
	return a/b;
}

string echo()
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(30));
	return "echo";
}
