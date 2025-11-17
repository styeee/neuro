#pragma once
#include <vector>
#include <assert.h>

class network
{
	typedef float real;

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
		layers(form.size()),
		weights(form.size()-1)
	{
		for(size_t i=0;i<form.size();i++)
			layers[i].resize(form[i]);

		for(size_t j=0;j<form.size()-1;j++)
		{
			weights[j].resize(form[j]);

			for(size_t i=0;i<form[j];i++)
				weights[j][i].resize(form[j+1]);
		}
	}

	void weight(const std::vector<real>& w,const std::vector<real>& b)
	{
		size_t n=0;
		for(size_t k=0;k<weights.size();k++)
		for(size_t j=0;j<weights[k].size();j++)
		for(size_t i=0;i<weights[k][j].size();i++)
			weights[k][j][i]=w[n++];
		
		n=0;
		for(size_t j=1;j<layers.size();j++)
		for(size_t i=0;i<layers[j].size();i++)
			layers[j][i].bias=b[n++];
	}

	void weight(real power)
	{
		for(size_t k=0;k<weights.size();k++)
		for(size_t j=0;j<weights[k].size();j++)
		for(size_t i=0;i<weights[k][j].size();i++)
		{
			const real d=layers[k+1][i].error*layers[k][j].res*power;
			weights[k][j][i]+=d;
		}

		for(size_t j=1;j<layers.size();j++)
		for(size_t i=0;i<layers[j].size();i++)
			layers[j][i].bias+=layers[j][i].error*power;
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
		for(size_t k=1;k<layers.size();k++)
		for(size_t j=0;j<layers[k].size();j++)
		{
			layers[k][j].res=0;
			for(size_t i=0;i<layers[k-1].size();i++)
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
		real power
	)
	{
		input(i);
		forward();
		backward(o);
		weight(power);
	}

	std::vector<real> predict(const std::vector<real>& x)
	{
		input(x); 
		forward();
		return output();
	}

	static int test()
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

		bool t1=test.predict({0,0})[0]>.5;
		bool t2=test.predict({1,1})[0]>.5;
		bool t3=test.predict({0,1})[0]<.5;
		bool t4=test.predict({1,0})[0]<.5;

		return t1+t2*2+t3*4+t4*8;
	}
};