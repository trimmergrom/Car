#include<iostream>
#include<thread>


using namespace std::chrono_literals;

bool finish = false;

void Plus()
{
	while(!finish)
	{
		std::cout << "+ ";
		std::this_thread::sleep_for(10ms);
	}
}
void Minus()
{
	while (!finish)
	{
		std::cout << "- ";
		std::this_thread::sleep_for(20ms);
	}
}

void main()
{
	std::thread plus_thread(Plus);
	std::thread minus_thread(Minus);

	std::cin.get();
	finish = true;
	
	minus_thread.join();
	plus_thread.join();
}