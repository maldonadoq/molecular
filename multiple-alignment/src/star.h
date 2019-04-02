#ifndef _STAR_H_
#define _STAR_H_

#include "../../single-alignment/src/needleman-wunsch.h"
#include "../../utils/print.h"

class TStar{
private:
	TNeedlemanWunsch *m_nw;
	std::vector<std::string> m_dnas;
	std::vector<std::vector<TScoreAlignment> > m_scores;

	int match_score;					// match score
	int mismatch_score;					// mismatch score
	int gap_score;						// gap score	
public:
	TStar(int _match, int _mismatch, int _gap){
		this->match_score    = _match;
		this->mismatch_score = _mismatch;
		this->gap_score      = _gap;
		this->m_nw = new TNeedlemanWunsch(match_score, mismatch_score, gap_score);
	}

	void FSetDna(std::vector<std::string>);
	void FScores();
	void FPrintScores();
	int  FSumPairs(std::vector<std::string >);
	int  FSimilarity(char, char);

	unsigned FGetMaximunIdx();
	unsigned FMaxSize(unsigned);
	std::vector<std::string> FGetMultipleAlignment();

	~TStar(){	}
};

void TStar::FSetDna(std::vector<std::string> _dnas){
	this->m_dnas = _dnas;

	this->m_scores = std::vector<std::vector<TScoreAlignment> >(m_dnas.size(),
		std::vector<TScoreAlignment>(m_dnas.size()));
}

unsigned TStar::FGetMaximunIdx(){
	unsigned idx;
	int maxi = 0;
	int tmp;

	for(unsigned j=0; j<m_scores[0].size(); j++)
		maxi += m_scores[0][j].m_score;
	idx = 0;

	for(unsigned i=1; i<m_scores.size(); i++){
		tmp = 0;
		for(unsigned j=0; j<m_scores[i].size(); j++){
			tmp += m_scores[i][j].m_score;
		}

		if(tmp > maxi){
			maxi = tmp;
			idx = i;
		}
	}

	return idx;
}

void TStar::FScores(){
	TScoreAlignment tmp;
	for(unsigned i=0; i<m_dnas.size(); i++){
		for(unsigned j=i+1; j<m_dnas.size(); j++){
			tmp = m_nw->FGetScoreAlignment(m_dnas[i], m_dnas[j]);
			this->m_scores[i][j] = tmp;
			this->m_scores[j][i] = TScoreAlignment(tmp.m_score,
				std::make_pair(tmp.m_alignment.second, tmp.m_alignment.first));
		}
	}
}

unsigned TStar::FMaxSize(unsigned _idx){
	unsigned size = 0;
	for(unsigned j=0; j<m_scores[_idx].size(); j++){
		if(m_scores[_idx][j].m_alignment.first.size() > size){
			size = m_scores[_idx][j].m_alignment.first.size();
		}
	}

	return size;
}

std::vector<std::string> TStar::FGetMultipleAlignment(){
	std::vector<std::string> align;

	unsigned idx = FGetMaximunIdx();	
	// std::cout << "idx: " << idx << "\n";

	unsigned size = FMaxSize(idx);
	// std::cout << "size: " << size << "\n";

	bool init = false;
	for(unsigned j=0; j<m_scores[idx].size(); j++){
		if(j!=idx){
			if(!init){
				align.push_back(str_complete(m_scores[idx][j].m_alignment.first ,'-',size));
				align.push_back(str_complete(m_scores[idx][j].m_alignment.second,'-',size));
				init =true;
			}
			else{
				align.push_back(str_complete(m_scores[idx][j].m_alignment.second,'-',size));
			}
		}
	}

	return align;
}

int TStar::FSimilarity(char _a, char _b){
	int tmp;

	if(_a=='-' or _b=='-')	tmp = -gap_score;
	else if(_a==_b)	tmp = match_score;
	else if(_a!=_b)	tmp = -mismatch_score;

	return tmp;
}

int TStar::FSumPairs(std::vector<std::string > _alignments){
	int score = 0;
	int tmp;

	for(unsigned j=0; j<_alignments[0].size(); j++){
		tmp = 0;
		for(unsigned i=0; i<_alignments.size(); i++){
			for(unsigned k=i+1; k<_alignments.size(); k++){
				tmp += FSimilarity(_alignments[i][j], _alignments[k][j]);
			}
		}
		// std::cout << tmp << " ";
		score += tmp;
	}
	// std::cout << "\n";

	return score;
}

void TStar::FPrintScores(){
	for(unsigned i=0; i<m_scores.size(); i++){
		for(unsigned j=0; j<m_scores[i].size(); j++){
			std::cout << m_scores[i][j].m_score << " ";
		}
		std::cout << "\n";
	}
}

#endif