#ifndef _STAR_H_
#define _STAR_H_

#include "msa.h"

class TStar: public TMsa{
public:
	TStar(int _match, int _mismatch, int _gap)
		: TMsa(_match, _mismatch, _gap){
		
	}

	void FPrintScores();
	unsigned FMaxSize(unsigned);
	
	std::vector<std::string> FGetMultipleAlignment();

	~TStar(){	}
};

unsigned TStar::FMaxSize(unsigned _idx){
	unsigned size = 0;
	for(unsigned j=0; j<m_matrix_alignments[_idx].size(); j++){
		if(m_matrix_alignments[_idx][j].m_alignment.first.size() > size){
			size = m_matrix_alignments[_idx][j].m_alignment.first.size();
		}
	}

	return size;
}

std::vector<std::string> TStar::FGetMultipleAlignment(){
	std::vector<std::string> align;

	unsigned idx = FGetMaximunIdx();	
	std::cout << "idx: " << idx << "\n";	

	unsigned size = FMaxSize(idx);
	// std::cout << "size: " << size << "\n";

	bool init = false;
	for(unsigned j=0; j<m_matrix_alignments[idx].size(); j++){
		if(j!=idx){
			if(!init){
				align.push_back(str_complete(m_matrix_alignments[idx][j].m_alignment.first ,'-',size));
				align.push_back(str_complete(m_matrix_alignments[idx][j].m_alignment.second,'-',size));
				std::cout << "align\n" << m_matrix_alignments[idx][j].m_alignment.first << "\n";
				init =true;
			}
			else{
				align.push_back(str_complete(m_matrix_alignments[idx][j].m_alignment.second,'-',size));
			}
		}
	}

	return align;
}

void TStar::FPrintScores(){
	for(unsigned i=0; i<m_matrix_alignments.size(); i++){
		for(unsigned j=0; j<m_matrix_alignments[i].size(); j++){
			std::cout << m_matrix_alignments[i][j].m_score << " ";
		}
		std::cout << "\n";
	}
}

#endif
