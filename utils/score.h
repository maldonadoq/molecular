#ifndef _SCORE_H_
#define _SCORE_H_

#include <utility>
#include <string>

using std::string;

class TPairwiseAlignment{
public:
	int m_score;
	std::pair<std::string, std::string> m_alignment;
	TPairwiseAlignment(int _score, std::pair<std::string, std::string> _alignment){
		this->m_score = _score;
		this->m_alignment = _alignment;
	}

	TPairwiseAlignment(){
		this->m_score = 0;
		this->m_alignment = std::make_pair("","");
	}

	friend std::ostream& operator<< (std::ostream & out, TPairwiseAlignment &c){
		out << "score: " << c.m_score << "\nalign1: " << c.m_alignment.first << "\nalign2: " << c.m_alignment.second;
		return out;
   	}
};

std::string str_complete(std::string _dna, char _c, unsigned _size){
	std::string dna = _dna;
	if(_dna.size()<_size){
		for(unsigned i=_dna.size(); i<_size; i++)
			dna += _c;
	}
	return dna;
}

struct TItem{
	string first;
	string second;
	int score;
};

#endif