#ifndef _STAR_H_
#define _STAR_H_

#include "../../utils/score.h"
#include "../../utils/print.h"
#include <thread>

using std::thread;
using std::vector;
using std::cout;

template<class F>
class TStar{
protected:	
	static vector<string> m_dnas;
	static vector<vector<TItem> > m_mpairs;

	static int match_score;					// match score
	static int mismatch_score;				// mismatch score
	static int gap_score;					// gap score	

public:
	TStar(int, int, int);
	virtual ~TStar();

	unsigned get_maximun();				// +
	unsigned max_size(unsigned);				// -
	void	 set_dna(vector<string>);	// +
	int		 similarity(char, char);	// +
	void	 alignment();				// +
	
	vector<string> run();				// +

	static void  thread_alignment(unsigned, unsigned); // +
};

template<class F>	vector<string> TStar<F>::m_dnas;
template<class F>	vector<vector<TItem> > TStar<F>::m_mpairs;
template<class F>	int TStar<F>::match_score;
template<class F>	int TStar<F>::mismatch_score;
template<class F>	int TStar<F>::gap_score;


template<class F>
TStar<F>::TStar(int _match, int _mismatch, int _gap){
	this->match_score    = _match;
	this->mismatch_score = _mismatch;
	this->gap_score      = _gap;
}

template<class F>
void TStar<F>::set_dna(vector<string> _dnas){
	this->m_dnas = _dnas;

	this->m_mpairs = vector<vector<TItem> >(m_dnas.size(),
		vector<TItem>(m_dnas.size()));
}

template<class F>
int TStar<F>::similarity(char _a, char _b){
	int tmp;

	if(_a=='-' or _b=='-')	tmp = -gap_score;
	else if(_a==_b)	tmp = match_score;
	else if(_a!=_b)	tmp = -mismatch_score;

	return tmp;
}

template<class F>
void TStar<F>::thread_alignment(unsigned i, unsigned _j){
	TItem tmp;
	F tfunc(match_score, mismatch_score, gap_score);

	for(unsigned j=_j; j<m_dnas.size(); j++){
		tmp = tfunc.run(m_dnas[i], m_dnas[j]);
		m_mpairs[i][j] = tmp;
		m_mpairs[j][i] = {tmp.second, tmp.first, tmp.score};
	}
}

template<class F>
void TStar<F>::alignment(){
	thread th[m_dnas.size()-1];

	for(unsigned i=0; i<m_dnas.size()-1; i++){
		th[i] = thread(this->thread_alignment, i, i+1);
	}

	for(unsigned i=0; i<m_dnas.size()-1; i++){
		th[i].join();
	}
}

template<class F>
unsigned TStar<F>::get_maximun(){
	unsigned idx;
	int maxi = 0;
	int tmp;

	for(unsigned j=0; j<m_mpairs[0].size(); j++)
		maxi += m_mpairs[0][j].score;
	idx = 0;

	for(unsigned i=1; i<m_mpairs.size(); i++){
		tmp = 0;
		for(unsigned j=0; j<m_mpairs[i].size(); j++){
			tmp += m_mpairs[i][j].score;
		}

		if(tmp > maxi){
			maxi = tmp;
			idx = i;
		}
	}

	return idx;
}

template<class F>
unsigned TStar<F>::max_size(unsigned _idx){
	unsigned size = 0;
	for(unsigned j=0; j<m_mpairs[_idx].size(); j++){
		if(m_mpairs[_idx][j].first.size() > size){
			size = m_mpairs[_idx][j].first.size();
		}
	}

	return size;
}

template<class F>
vector<string> TStar<F>::run(){
	alignment();
	std::vector<std::string> align;
	unsigned idx = get_maximun();
	unsigned size = max_size(idx);

	bool init = false;
	for(unsigned j=0; j<m_mpairs[idx].size(); j++){
		if(j!=idx){
			if(!init){
				align.push_back(str_complete(m_mpairs[idx][j].first ,'-',size));
				align.push_back(str_complete(m_mpairs[idx][j].second,'-',size));
				init =true;
			}
			else{
				align.push_back(str_complete(m_mpairs[idx][j].second,'-',size));
			}
		}
	}

	return align;
}

template<class F>
TStar<F>::~TStar(){
	m_dnas.clear();
	m_mpairs.clear();
}

#endif