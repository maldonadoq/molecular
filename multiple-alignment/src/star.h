#ifndef _STAR_H_
#define _STAR_H_

#include "../../single-alignment/src/needleman-wunsch.h"

class TStar{
private:
	TNeedlemanWunsch m_nw;
	std::vector<std::string> m_dnas;	
public:
	TStar(int _match, int _mismatch, int _gap){
		this->m_nw = TNeedlemanWunsch(_match, _mismatch, _gap);
	}

	void FSetDna(std::vector<std::string> _dnas){
		this->m_dnas = _dnas;
	}

	~TStar(){	}
};

#endif