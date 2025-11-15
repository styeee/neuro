#include <iostream>
#include <vector>
#include <initializer_list>

class neuron
{
	std::vector<float>values;
	std::vector<float>weights;
	float res;

	float f(float x)
	{return 1./(1.+exp(-x));}
public:
	void value(size_t id,float f)
	{values[id]=f;}
	void weight(size_t id,float f)
	{weights[id]=f;}
	
	void setup(size_t size)
	{values.resize(size);weights.resize(size);}

	void calc(float bias)
	{
		this->res=0;

		for(size_t i=0;i<values.size();i++)
			res+=values[i]*weights[i];

		res=f(res+bias);
	}

	float get()const
	{return res;}
};

class neuro_xor
{
	neuron a;
	float bias1;
	neuron b;
	float bias2;
	neuron c;
	float bias3;
public:
	void init()
	{
		a.setup(2);
		b.setup(2);
		c.setup(2);

		a.weight(0,20);
		a.weight(1,20);
		bias1=-30;

		b.weight(0,-20);
		b.weight(1,-20);  
		bias2=10;

		c.weight(0,20);
		c.weight(1,20);
		bias3=-10;
	}

	float calc(float x,float y)
	{
		a.value(0,x);
		a.value(1,y);
		b.value(0,x);
		b.value(1,y);

		a.calc(bias1);
		b.calc(bias2);

		c.value(0,a.get());
		c.value(1,b.get());
		c.calc(bias3);

		return c.get();
	}
};

int main()
{
	neuro_xor test;
	test.init();

	std::cout<<(test.calc(1,0)>.5)<<std::endl;
	std::cout<<(test.calc(0,1)>.5)<<std::endl;
	std::cout<<(test.calc(1,1)>.5)<<std::endl;
	std::cout<<(test.calc(0,0)>.5)<<std::endl;
	

	return 0;
}
