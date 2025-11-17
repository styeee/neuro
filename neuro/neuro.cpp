#include <iostream>
#include <vector>
#include <assert.h>
#include <math.h>
#include "network.h"

int main()
{
	network test({2,2,1});
	test.weight({0.2, -0.3, -0.1, 0.4, 0.5,-0.4},{0.1,-0.2,0.05});
	
	const float k=.6;
	for(int e=0;e<20000;e++)
	{
		test.learn({0,0},{0},k);
		test.learn({0,1},{1},k);
		test.learn({1,0},{1},k);
		test.learn({1,1},{0},k);
	}

	std::cout<<"0 XOR 0=";
	std::cout<<test.predict({0,0})[0];
	std::cout<<std::endl;

	std::cout<<"1 XOR 1=";
	std::cout<<test.predict({1,1})[0];
	std::cout<<std::endl;

	std::cout<<"0 XOR 1=";
	std::cout<<test.predict({0,1})[0];
	std::cout<<std::endl;

	std::cout<<"1 XOR 0=";
	std::cout<<test.predict({1,0})[0];
	std::cout<<std::endl;
	
	return test.test();
}
