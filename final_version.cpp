#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <omp.h>
#include <chrono>
#include <assert.h>

std::vector<std::vector<int>> sequential(int,int,int);
std::vector<std::vector<int>> parralel(int,int,int,int);

void printdata(std::vector<std::vector<int>> output,int points)
{
	std::ofstream myfile;
  	myfile.open("output.txt");

	for (int a = 0; a <output.size(); a++)
	{	auto i = 0.00;
		std::cout<<"\nDimension: "<<a+2<<std::endl;
		std::cout<<"Distance\tFraction\tCount\n";
		for (int b = 0; b < output[a].size() ; b++)
		{
			std::cout<<i<<"\t\t"<<100*output[a][b]/points<<"%\t\t"<<output[a][b]<<std::endl;
			myfile<<a+2<<"\t"<<i<<"\t"<<output[a][b]<<"\n";
			i += 0.01;
		}
	}
	myfile.close();
}

int main(int argc, char const *argv[])
{
	assert(argc==4);
	/*
		./omp dimensions points noofthreads 
	*/

	int dim = atoi(argv[1]);
	int points = atoi(argv[2]);
	int interval = 100; 
	int threads = atoi(argv[3]);

	std::vector<std::vector<int>> output;

	auto start1 = std::chrono::high_resolution_clock::now();
	
	parralel(dim,points,interval,threads);

	auto stop1 = std::chrono::high_resolution_clock::now();
	auto dt1 = stop1 - start1;
 	

	auto start = std::chrono::high_resolution_clock::now();
	
	output =  sequential(dim,points,interval);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dt = stop - start;

	printdata(output,points);

	auto ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(dt1);
 	std::cout << "\nParralel Version Took: "<< ms1.count() << " milliseconds."<< std::endl;

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dt);
 	std::cout << "\nSequential Version Took: "<< ms.count() << " milliseconds."<< std::endl;

	return 0;
}

std::vector<std::vector<int>> sequential(int dim,int points,int interval)
{
	std::default_random_engine DRE;
	std::normal_distribution<float> URD(0,1);
	std::vector<std::vector<int>> output;

	for (int w = 2; w <= dim ; w++)
	{	std::vector<int> v1(100,0);
		for (int i = 0; i < points; i++)
		{
			float d = 0;
			std::vector<float> v;
			/*
				Method of dimensionality+2 refered from 
				https://en.m.wikipedia.org/wiki/N-sphere#Spherical_coordinates
				Paragraph: "Uniformaly at random within the n-ball"
			*/
			for (int j = 0; j < w+2; j++)
			{
				float p = URD(DRE);
				v.push_back(p);
				d += p*p;
			}
			d = std::sqrt(d);
			/*
				Method is refered from 
				https://www.datasciencecentral.com/forum/topics/generating-random-points-on-a-sphere
			*/
			for(int j = 0; j < w+2; j++)
			{
				v[j] *= (1/d);
			}
			d = 0;
			
			for(int j = 0; j < w; j++)
			{
				d += (v[j]*v[j]);
			}
			d = std::sqrt(d);
			
			if(d > 1) i--;
			else
			{
				float d1 = (d*100);
				v1[int(d1)] += 1;
			}
		}
		output.push_back(v1);
	}

	return output;
}

std::vector<std::vector<int>> parralel(int dim,int points,int interval,int threads)
{
	std::default_random_engine DRE;
	std::normal_distribution<float> URD(0,1);
	std::vector<std::vector<int>> output;

 #pragma omp parallel for num_threads(threads) schedule(dynamic) private(DRE,URD)
 	#pragma for
 	for (int w = 2; w <= dim ; w++)
	{	std::vector<int> v1(100,0);
		for (int i = 0; i < points; i++)
		{
			float d = 0;
			std::vector<float> v;
	
			for (int j = 0; j < w+2; j++)
			{
				float p = URD(DRE);
				v.push_back(p);
				d += p*p;
			}
			d = std::sqrt(d);
			
			for(int j = 0; j < w+2; j++)
			{
				v[j] *= (1/d);
			}
			d = 0;
			
			for(int j = 0; j < w; j++)
			{
				d += (v[j]*v[j]);
			}
			d = std::sqrt(d);
			
			if(d > 1) i--;
			else
			{
				float d1 = (d*100);
				v1[int(d1)] += 1;
			}
		}
		output.push_back(v1);
	}	
	return output;
}
