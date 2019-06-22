
#include <iostream>
#include <utility>
#include "inc/needleman-wunsch.h"

using std::cout;
using std::string;

int main(int argc, char const *argv[]){

	string dnaa = "TACACGCCTCTTCGCC";
	string dnab = "CTGTGAGG";

 	int match    = 1;
	int mismatch = -1;
	int gap      = -1;

 	TNeedlemanWunsch *nw = new TNeedlemanWunsch(match, mismatch, gap);

 	TItem res = nw->run(dnaa, dnab);

 	cout << "score : " << res.score << "\n";
 	cout << "align1: " << res.first << "\n";
 	cout << "align2: " << res.second << "\n";

 	delete nw;
	return 0;
}