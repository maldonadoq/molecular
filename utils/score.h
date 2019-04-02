#ifndef _SCORE_H_
#define _SCORE_H_

#include <utility>

class TScoreAlignment{
public:
	int m_score;
	std::pair<std::string, std::string> m_alignment;
	TScoreAlignment(int _score, std::pair<std::string, std::string> _alignment){
		this->m_score = _score;
		this->m_alignment = _alignment;
	}

	friend std::ostream& operator<< (std::ostream & out, TScoreAlignment &c){
		out << "score: " << c.m_score << "\nalign1: " << c.m_alignment.first << "\nalign2: " << c.m_alignment.second;
		return out;
   	}
};

#endif