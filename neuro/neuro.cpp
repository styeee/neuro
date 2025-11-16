#include <iostream>
#include <vector>
#include <math.h>

typedef float real;

class neuron
{
private:
	friend class layer;

	std::vector<real> weights;
	std::vector<neuron>* prev;
	real res;
	real bias;

	real f(real x)
	{return 1./(1.+exp(-x));}

public:
	
	void calc()
	{
		res=0;
		for(size_t i=0;i<weights.size();i++)
			res+=(*prev)[i].value()*weights[i];
		res=f(res+bias);
	}

	real weight(size_t id)const
	{return weights[id];}

	void weight(size_t id,real v)
	{weights[id]=v;}

	real value()const
	{return res;}

	void value(real v)
	{res=v;}
};

class layer
{
private:
	std::vector<neuron> self;
public:
	layer(size_t size)
	:
		self(size)
	{}

	void calc()
	{
		for(neuron& e:self)
			e.calc();
	}

	void connect(layer* prev)
	{
		for(neuron& e:self)
		{
			e.weights.resize(prev->size());
			e.prev=&prev->self;
		}
	}

	size_t size()const
	{return self.size();}

	real weight(size_t i,size_t j)const
	{return self[i].weight(j);}

	void weight(size_t i,size_t j,real v)
	{self[i].weight(j,v);}

	real value(size_t id)const
	{return self[id].value();}

	void value(size_t id,real v)
	{return self[id].value(v);}

	void set_bias(size_t id,real v)
	{self[id].bias=v;}
};

real test(real x,real y)
{
	layer start(2);
		start.value(0,x);
		start.value(1,y);

	layer first(2);
		first.connect(&start);
		
		first.weight(0,0,20);
		first.weight(0,1,20);
		first.set_bias(0,-10);

		first.weight(1,0,-20);
		first.weight(1,1,-20);
		first.set_bias(1,30);

	layer second(1);
		second.connect(&first);
		second.weight(0,0,20);
		second.weight(0,1,20);
		second.set_bias(0,-30);

	first.calc();
	second.calc();

	return second.value(0);
}

int main()
{
	return test(1,0)>.5;
}