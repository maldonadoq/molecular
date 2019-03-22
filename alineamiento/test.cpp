#include <iostream>
#include <utility>

int main(int argc, char const *argv[]){
	std::pair<int, int> a = std::make_pair(1,2);
	std::pair<int, int> b = std::make_pair(2,2);

	if(a == b)	std::cout << "equal\n";
	else	std::cout << "not equal\n";

	return 0;
}