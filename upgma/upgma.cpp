#include <iostream>
#include <stdio.h>
#include "src/utils.h"
#include "src/upgma.h"

using std::string;
using std::cout;
using std::stoi;

typedef std::vector<float> dvect;

// build: g++ upgma.cpp -o upgma.out
int main(int argc, char const *argv[]){
	vector<string> headers;
	vector<dvect>  distances;

	string filename = "../data/Upgma-Small.txt";
	pre_processing_file_two(filename, distances, headers);

	/*int nc = 5;
	vector<std::string > headers   = {"A","B","C","D","E"};
	vector<dvect>        distances = std::vector<dvect>(nc, dvect(nc));

	distances[0] = {0};
	distances[1] = {17,0};
	distances[2] = {21,30,0};
	distances[3] = {31,34,28,0};
	distances[4] = {23,21,39,43,0};*/

	/*int nc = 6;
	vector<std::string > headers   = {"A","B","C","D","E","F"};
	vector<dvect>        distances = std::vector<dvect>(nc, dvect(nc));

	distances[0] = {0};
	distances[1] = {1,0};
	distances[2] = {3,3,0};
	distances[3] = {6,6,5,0};
	distances[4] = {7,7,6,1,0};
	distances[5] = {10,10,9,7,8,0};*/

	/*int nc = 4;
	vector<std::string > headers   = {"A","B","C","D"};
	vector<dvect>        distances = std::vector<dvect>(nc, dvect(nc));

	distances[0] = {0};
	distances[1] = {3,0};
	distances[2] = {5,4,0};
	distances[3] = {7,1,2,0};*/

	/*int nc = 6;
	vector<std::string > headers   = {"A","B","C","D","E","F"};
	vector<dvect>        distances = std::vector<dvect>(nc, dvect(nc));

	distances[0] = {0};
	distances[1] = {2,0};
	distances[2] = {4,4,0};
	distances[3] = {6,6,6,0};
	distances[4] = {6,6,6,4,0};
	distances[5] = {8,8,8,8,8,0};*/

	unsigned ncluster = 1;

	TUpgma *up = new TUpgma();
	up->Init(distances, headers);
	up->Run(ncluster);

	delete up;
	return 0;
}