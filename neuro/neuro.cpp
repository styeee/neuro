#include <iostream>
#include <vector>
#include <assert.h>
#include <math.h>

typedef float real;

class network
{
	static real f(real x)
	{return 1./(1.+exp(-x));}
	static real df(real x)
	{return x*(1-x);}

	struct neuron{real res;real bias;real error;};
	
	typedef std::vector<neuron> layer;
	std::vector<layer> layers;

	typedef std::vector<std::vector<real>> matrix;
	std::vector<matrix> weights;

public:

	network(const std::vector<size_t> form)
	:
		layers(form.size()),//количевство слоев
		weights(form.size()-1)//количевство весов
	{
		for(size_t i=0;i<form.size();i++)//каждый слой
			layers[i].resize(form[i]);//заполняется нейронами

		for(size_t j=0;j<form.size()-1;j++)//каждый вес
		{
			weights[j].resize(form[j]);//заполняется спискаи весов

			for(size_t i=0;i<form[j];i++)//каждый список весов
				weights[j][i].resize(form[j+1]);//заполняется весами
		}
	}

	void weight(const std::vector<real>& w,const std::vector<real>& b)
	{
		size_t n=0;
		for(size_t k=0;k<weights.size();k++)//каждый слой
		for(size_t j=0;j<weights[k].size();j++)//каждый нейрон
		for(size_t i=0;i<weights[k][j].size();i++)//каждый сосед
			weights[k][j][i]=w[n++];
		
		n=0;
		for(size_t j=1;j<layers.size();j++)//каждый слой
		for(size_t i=0;i<layers[j].size();i++)//каждый нейрон
			layers[j][i].bias=b[n++];
	}

	void input(const std::vector<real>& data)
	{
		assert(data.size()==layers[0].size());

		for(size_t i=0;i<data.size();i++)
			layers[0][i].res=data[i];
	}

	std::vector<real> output()
	{
		std::vector<real> r(layers.back().size());
		for(size_t i=0;i<r.size();i++)
			r[i]=layers.back()[i].res;
		return r;
	}

	void forward()
	{
		for(size_t k=1;k<layers.size();k++)//каждый слой
		for(size_t j=0;j<layers[k].size();j++)//каждый нейрон
		{
			layers[k][j].res=0;
			for(size_t i=0;i<layers[k-1].size();i++)//каждый сосед
				layers[k][j].res+=layers[k-1][i].res*weights[k-1][i][j];
			layers[k][j].res=f(layers[k][j].res+layers[k][j].bias);
		}
	}
};

int main()
{
	network test({2,2,1});
	test.weight({20,20, -20,-20, 20,20},{-10,30,-30});
	test.input({0,1});
	test.forward();

	return 0;
}