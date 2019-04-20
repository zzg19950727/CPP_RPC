#include <iostream>
#include <exception>
#include <chrono>
#include <thread>
#include "add.h"

void add_test()
{
	for(int i=1; i<20; ++i)
	{
		try
		{
			std::cout<<i<<"+"<<i<<"+"<<i<<"="<<add(i,i,i)<<std::endl;
		}
		catch(std::exception& e)
		{
			std::cout<<"add "<<i;
			std::cout<<e.what()<<std::endl;
		}
	}
}

void sub_test()
{
	for(int i=1; i<20; ++i)
	{
		try
		{
			std::cout<<i<<"-"<<i<<"="<<sub(i,i)<<std::endl;
		}
		catch(std::exception& e)
		{
			std::cout<<"sub "<<i;
			std::cout<<e.what()<<std::endl;
		}
	}
}

void mul_test()
{
	for(int i=1; i<20; ++i)
	{
		try
		{
			std::cout<<i<<"*"<<i<<"="<<mul(i,i)<<std::endl;
		}
		catch(std::exception& e)
		{
			std::cout<<"mul "<<i;
			std::cout<<e.what()<<std::endl;
		}
	}
}

void div_test()
{
	for(int i=1; i<20; ++i)
	{
		try
		{
			double a = i;
			std::cout<<i<<"/"<<i<<"="<<div(a,a)<<std::endl;
		}
		catch(std::exception& e)
		{
			std::cout<<"div "<<i;
			std::cout<<e.what()<<std::endl;
		}
	}
}

void echo_test()
{
	for(int i=1; i<20; ++i)
	{
		try
		{
			std::cout<<"echo:"<<echo()<<std::endl;
		}
		catch(std::exception& e)
		{
			std::cout<<"echo "<<i;
			std::cout<<e.what()<<std::endl;
		}
	}
}

int main()
{
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
