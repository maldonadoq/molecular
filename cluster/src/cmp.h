#ifndef _CMP_H_
#define _CMP_H_

template<class T>
class TC{
public:
	inline T operator()(T x, T y){
		// return std::min(x,y);
		// return std::max(x,y);
		return (x+y)/2;
	}
};

template<class T>
class TC1{
public:
	inline T operator()(T x, T y){
		return std::min(x,y);
	}
};

template<class T>
class TC2{
public:
	inline T operator()(T x, T y){
		return std::max(x,y);
	}
};

template<class T>
class TC3{
public:
	inline T operator()(T x, T y){
		return (x+y)/2;
	}
};

#endif