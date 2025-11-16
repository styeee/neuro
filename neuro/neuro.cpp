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
	real bias=0;
	real res=0;

	static real f(real x)
	{return 1./(1.+exp(-x));}

public:
	
	size_t size()const
	{return weights.size();}

	void calc()
	{
		res=0;
		for(size_t i=0;i<size();i++)
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

class network
{
	std::vector<layer*> self;
public:
	network(std::initializer_list<size_t> form)
	:
		self(form.size())
	{

		size_t i=0;

		for(size_t n:form)
			self[i++]=new layer(n);

		for(size_t i=1;i<self.size();i++)
			self[i]->connect(self[i-1]);
	}

	std::vector<real> predict(std::initializer_list<real> x)
	{
		size_t i=0;
		for(real e:x)
			self[0]->value(i++,e);

		for(size_t i=1;i<self.size();i++)
			self[i]->calc();

		std::vector<real> r(self.back()->size());
		for(size_t i=0;i<r.size();i++)
			r[i]=self.back()->value(i);

		return r;
	}

	void weight(std::vector<real> w)
	{
		size_t n=0;

		for(size_t k=1;k<self.size();k++)
		for(size_t j=0;j<self[k]->size();j++)
		{
			for(size_t i=0;i<self[k-1]->size();i++)
				self[k]->weight(j,i,w[n++]);

			self[k]->set_bias(j,w[n++]);
		}
	}
};

int main()
{
	network net({2,2,1});
	net.weight({20,20,-10, -20,-20,30, 20,20,-30 });

	return net.predict({0,1})[0]>.5;
}