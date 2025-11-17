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

	void weight(real power)
	{
		for(size_t k=0;k<weights.size();k++)//каждый слой
		for(size_t j=0;j<weights[k].size();j++)//каждый нейрон
		for(size_t i=0;i<weights[k][j].size();i++)//каждый сосед
		{
			const real d=layers[k+1][i].error*layers[k][j].res*power;
			weights[k][j][i]-=d;
		}

		for(size_t j=1;j<layers.size();j++)//каждый слой
		for(size_t i=0;i<layers[j].size();i++)//каждый нейрон
			layers[j][i].bias-=layers[j][i].error*power;
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

	void backward(const std::vector<real> target)
	{
		for(size_t i=0;i<layers.back().size();i++)
		{
			layers.back()[i].error=(target[i]-layers.back()[i].res);
			layers.back()[i].error*=df(layers.back()[i].res);
		}

		for(int k=layers.size()-2;k>=0;k--)
		for(size_t j=0;j<layers[k].size();j++)
		{
			layers[k][j].error=0;
			for(size_t i=0;i<layers[k+1].size();i++)
			{
				real const d=layers[k+1][i].error*weights[k][j][i];
				layers[k][j].error+=d;
			}
			layers[k][j].error*=df(layers[k][j].res);
		}
	}

	void learn
	(
		const std::vector<real>& i,
		const std::vector<real>& o,
		size_t count,
		real power
	)
	{
		input(i);
		forward();
		backward(o);
		weight(power);
	}

	void good_weight()
	{
		weight({20,-20,20,-20,20,20},{-10,30,-30});
	}
};

int main()
{
    network test({2,2,1});
	test.good_weight();
    
    test.input({0,0}); test.forward(); 
    std::cout << "0,0 -> " << test.output()[0] << std::endl;
    
    test.input({0,1}); test.forward();
    std::cout << "0,1 -> " << test.output()[0] << std::endl;
    
    test.input({1,0}); test.forward();
    std::cout << "1,0 -> " << test.output()[0] << std::endl;
    
    test.input({1,1}); test.forward();
    std::cout << "1,1 -> " << test.output()[0] << std::endl;

    return 0;
}