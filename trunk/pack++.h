#ifndef PackXX_h
#define PackXX_h

#include <vector>
#include <string>
#include <iostream>
#include "SDL.h"
#include "SDL_endian.h"

//#include "begin_code.h"

typedef float Float32;
typedef double Float64;
typedef long double Float96;

namespace Packer
{

struct Pack
{
	virtual std::ostream &print(std::ostream &os) const =0;
	virtual std::ostream &pack(std::ostream &os) const =0;
	virtual std::istream &unpack(std::istream &is) =0;
	size_t size();
};

std::ostream &operator<<(std::ostream &os, const Pack &p);
std::istream &operator>>(std::istream &is, Pack &p);

template<class T> std::ostream &print(std::ostream &os, const T &t)
{
	os<<t;
	return os;
}
template<class T> std::ostream &pack(std::ostream &os, const T &t)
{
	os.write(reinterpret_cast<const char*>(&t),sizeof(T));
	return os;
}
template<class T> std::istream &unpack(std::istream &is, T &t)
{
	is.read(reinterpret_cast<char*>(&t),sizeof(T));
	return is;
}

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
//Uint16
template<> std::ostream &pack(std::ostream &os, const Uint16 &t_);
template<> std::istream &unpack(std::istream &is, Uint16 &t);
//Uint32
template<> std::ostream &pack(std::ostream &os, const Uint32 &t_);
template<> std::istream &unpack(std::istream &is, Uint32 &t);
#ifdef SDL_HAS_64BIT_TYPE
//Uint64
template<> std::ostream &pack(std::ostream &os, const Uint64 &t_);
template<> std::istream &unpack(std::istream &is, Uint64 &t);
#endif // SDL_HAS_64BIT_TYPE
//Float32
template<> std::ostream &pack(std::ostream &os, const Float32 &t_);
template<> std::istream &unpack(std::istream &is, Float32 &t);
//Float64
template<> std::ostream &pack(std::ostream &os, const Float64 &t_);
template<> std::istream &unpack(std::istream &is, Float64 &t);
//Float96
template<> std::ostream &pack(std::ostream &os, const Float96 &t_);
template<> std::istream &unpack(std::istream &is, Float96 &t);
#endif // SDL_BYTEORDER == SDL_LIL_ENDIAN
template<> std::ostream &pack(std::ostream &os, const std::string &t_);
template<> std::istream &unpack(std::istream &is, std::string &t);

template<class T> std::ostream &print(std::ostream &os, const std::vector<T> &t)
{
	//os<<"["<<t.size()<<"]";
	Uint32 ct=0;
	for(typename std::vector<T>::const_iterator i=t.begin(); i!=t.end(); ++i, ++ct)
	{
		os<<"\n\t\t["<<ct<<"]\t";
		print(os,*i);
	}
	return os;
}

template<class T> std::ostream &pack(std::ostream &os, const std::vector<T> &t)
{
	Uint32 sz=t.size();
	pack(os,sz);
	for(typename std::vector<T>::const_iterator i=t.begin(); i!=t.end(); ++i)
		pack(os,*i);
	return os;
}

template<class T> std::istream &unpack(std::istream &is, std::vector<T> &t)
{
	Uint32 sz;
	unpack(is,sz);
	t.resize(sz);
	for(typename std::vector<T>::iterator i=t.begin(); i!=t.end(); ++i)
		unpack(is,*i);
	return is;
}

template<class T> std::ostream &print(std::ostream &os, const T t[], int n)
{
	//os<<"["<<t.size()<<"]";
	for(int i=0; i<n; ++i)
	{
		os<<"\n\t\t["<<i<<"]\t";
		print(os,t[i]);
	}
	return os;
}

template<class T> std::ostream &pack(std::ostream &os, const T t[], int n)
{
	for(int i=0; i<n; ++i)
		pack(os,t[i]);
	return os;
}

template<class T> std::istream &unpack(std::istream &is, T t[], int n)
{
	for(int i=0; i<n; ++i)
		unpack(is,t[i]);
	return is;
}

};

//#include "close_code.h"

#endif
