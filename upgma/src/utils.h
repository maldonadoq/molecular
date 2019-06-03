#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>

using std::cout;
using std::vector;
using std::string;
using std::to_string;
using std::ifstream;
using std::ofstream;

struct TItem
{
	string m_value;
	unsigned m_number;
	unsigned m_idx;
	unsigned m_idxs[2] = {0,0};
	float m_distance;
};

void Create_Python(string _body){
	ofstream file("upgma.py");
	file << "from scipy.cluster.hierarchy import dendrogram\n";
	file << "from matplotlib import pyplot as plt\n";

	file << "\n" << _body << "\n";

	file << "\nplt.figure(figsize=(10, 5), num='Dendogram')\n";
	file << "dendrogram(upgma_values,\n";
	file << "			orientation='left',\n";
	file << "			labels=upgma_labels,\n";
	file << "			leaf_font_size=6,\n";
	file << "			distance_sort='descending',\n";
	file << "			show_leaf_counts=True)\n";
	file << "plt.show()\n";

	file.close();
}

template<class T>
void print_vector(vector<T> _vector){
	for(unsigned i=0; i<_vector.size(); i++){
		cout << "[" << _vector[i] << "]\t";
	}
	cout << "\n";
}

template<class T>
void print_matrix(std::vector<std::vector<T> > _m){
	for(unsigned i=0; i<_m.size(); i++){
		for(unsigned j=0; j<_m[i].size(); j++){
			std::cout << _m[i][j] << "\t";
		}
		std::cout << "\n";
	}
}

string item_to_str(TItem _item){
	return "[" + to_string(_item.m_idxs[0]) + ", " + to_string(_item.m_idxs[1]) + ", " + to_string(_item.m_distance) + ", " + to_string(_item.m_number) + "]";
}

void print_items(vector<TItem> _vector){
	for(unsigned i=0; i<_vector.size(); i++){
		cout << "[" << _vector[i].m_value << "]\t";
	}
	cout << "\n";
}

void pre_processing_file(string _filename,
	vector<vector<float> > &_vdata, vector<string> &_vname){

	ifstream file(_filename);

	if(!file.is_open()){
		cout << "error! cvs wrong!!\n";
		return;
	}
	
	vector<vector<float> > vdata;
	vector<float> tmp(7);
	string value;

	getline(file,value,'\n');			//RowName
	while(file.good()){		
		getline(file,value,'\t');
		_vname.push_back(value);

		getline(file,value,'\t');	tmp[0] = stod(value); // X1
		getline(file,value,'\t');	tmp[1] = stod(value); // X2
		getline(file,value,'\t');	tmp[2] = stod(value); // X3
		getline(file,value,'\t');	tmp[3] = stod(value); // X4
		getline(file,value,'\t');	tmp[4] = stod(value); // X5
		getline(file,value,'\t');	tmp[5] = stod(value); // X6
		getline(file,value,'\n');	tmp[6] = stod(value); // X7

		vdata.push_back(tmp);
		// print_vector(tmp);
	}
	file.close();

	unsigned ndim = 7;
	float dist;

	for(unsigned i=0; i<vdata.size(); i++){
		tmp.clear();		
		for(unsigned j=0; j<i+1; j++){
			dist = 0;
			for(unsigned k=0; k<ndim; k++){
				dist += (pow(vdata[i][k] - vdata[j][k],2));
			}
			tmp.push_back(sqrt(dist));
		}
		_vdata.push_back(tmp);
	}
}

void pre_processing_file_two(string _filename,
	vector<vector<float> > &_vdata, vector<string> &_vname){

	ifstream file(_filename);

	if(!file.is_open()){
		cout << "error! cvs wrong!!\n";
		return;
	}
	
	vector<vector<float> > vdata;
	vector<float> tmp(2);
	string value;

	getline(file,value,'\n');			//RowName
	while(file.good()){		
		getline(file,value,'\t');
		_vname.push_back(value);

		getline(file,value,'\t');	tmp[0] = stod(value); // X1
		getline(file,value,'\n');	tmp[1] = stod(value); // X2
		vdata.push_back(tmp);
	}

	file.close();

	unsigned ndim = 2;
	float dist;

	for(unsigned i=0; i<vdata.size(); i++){
		tmp.clear();		
		for(unsigned j=0; j<i+1; j++){
			dist = 0;
			for(unsigned k=0; k<ndim; k++){
				dist += (pow(vdata[i][k] - vdata[j][k],2));
			}
			tmp.push_back(sqrt(dist));
		}
		_vdata.push_back(tmp);
	}
}

#endif