#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <string>

std::vector<std::string> m_headers = {"A", "B", "C", "D", "E", "F", "G"};

void PrintMap(std::map<std::string, std::map<std::string, float> > m_distances){

	typename std::map<std::string, std::map<std::string, float> >::iterator it;
	typename std::map<std::string, float>::iterator itr;

	for(it=m_distances.begin(); it!=m_distances.end(); it++){
		for(itr=it->second.begin(); itr!=it->second.end(); itr++){
	    	std::cout << itr->first << ": " << itr->second << " ";
	    }
	    std::cout << "\n";
	}
	std::cout << "\n";
}

int main(int argc, char const *argv[]){	
	std::map<std::string, std::map<std::string, float> > m_distances;

	unsigned i,j;

	for(i=0; i<m_headers.size(); i++){
		for(j=0; j<m_headers.size(); j++){
			m_distances[m_headers[i]][m_headers[j]] = rand()%10;
		}
	}	


	PrintMap(m_distances);

	m_headers.erase(m_headers.begin()+1);
	m_distances.erase("B");

	PrintMap(m_distances);

	return 0;
}