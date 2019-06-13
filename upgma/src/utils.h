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

	file << "\nplt.figure(figsize=(7, 5), num='Dendogram')\n";
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

#endif