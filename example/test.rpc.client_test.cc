#include <iostream>
#include <exception>
#include <chrono>
#include <thread>
#include "add.h"

static int count = 2000;

void add_test()
{
	int m = 0;
	for(int i=1; i<=count; ++i)
	{
		try
		{
			add(i,i,i);
		}
		catch(std::exception& e)
		{
			++m;
		}
	}
//	std::cout<<"add error:"<<m<<std::endl;
}

void sub_test()
{
	int m = 0;
	for(int i=1; i<=count; ++i)
	{
		try
		{
			sub(i,i);
		}
		catch(std::exception& e)
		{
			++m;
		}
	}
//	std::cout<<"sub error:"<<m<<std::endl;
}

void mul_test()
{
	int m = 0;
	for(int i=1; i<=count; ++i)
	{
		try
		{
			mul(i,i);
		}
		catch(std::exception& e)
		{
			++m;
		}
	}
//	std::cout<<"mul error:"<<m<<std::endl;
}

void div_test()
{
	int m = 0;
	for(int i=1; i<=count; ++i)
	{
		try
		{
			double a = i;
			div(a,a);
		}
		catch(std::exception& e)
		{
			++m;
		}
	}
//	std::cout<<"div error:"<<m<<std::endl;
}

void echo_test()
{
	int m = 0;
	for(int i=1; i<=count; ++i)
	{
		try
		{
			echo();
		}
		catch(std::exception& e)
		{
			++m;
		}
	}
//	std::cout<<"echo error:"<<m<<std::endl;
}

int main()
{
	std::thread* workers[1000];
	for(int i=0; i<1; ++i)
	{
		workers[i] = new std::thread(add_test);
	}
	for(int i=0; i<1; ++i)
	{
		workers[i]->join();
		delete workers[i];
	}
	return 0;
	std::thread thread1(add_test);
	std::thread thread2(sub_test);
	std::thread thread3(mul_test);
	std::thread thread4(div_test);
	std::thread thread5(echo_test);

	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();
	return 0;
}
