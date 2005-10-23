#include "pack++.h"
#include <sstream>
//#include <iterator>
//#include <algorithm>

using namespace std;

namespace Packer
{

size_t Pack::size()
{
	ostringstream oss;
	pack(oss);
	return oss.str().size();
}

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
//Uint16
template<> std::ostream &pack(std::ostream &os, const Uint16 &t_)
{
	//cerr<<"pack Uint16\n";
	Uint16 t(SDL_Swap16(t_));
	os.write(reinterpret_cast<const char*>(&t),sizeof(t));
	return os;
}
template<> std::istream &unpack(std::istream &is, Uint16 &t)
{
	//cerr<<"unpack Uint16\n";
	is.read(reinterpret_cast<char*>(&t),sizeof(t));
	t=SDL_Swap16(t);
	return is;
}
//Uint32
template<> std::ostream &pack(std::ostream &os, const Uint32 &t_)
{
	//cerr<<"pack Uint32\n";
	Uint32 t(SDL_Swap32(t_));
	os.write(reinterpret_cast<const char*>(&t),sizeof(t));
	return os;
}
template<> std::istream &unpack(std::istream &is, Uint32 &t)
{
	//cerr<<"unpack Uint32\n";
	is.read(reinterpret_cast<char*>(&t),sizeof(t));
	t=SDL_Swap32(t);
	return is;
}
#ifdef SDL_HAS_64BIT_TYPE
//Uint64
template<> std::ostream &pack(std::ostream &os, const Uint64 &t_)
{
	//cerr<<"pack Uint64\n";
	Uint64 t(SDL_Swap64(t_));
	os.write(reinterpret_cast<const char*>(&t),sizeof(t));
	return os;
}
template<> std::istream &unpack(std::istream &is, Uint64 &t)
{
	//cerr<<"unpack Uint64\n";
	is.read(reinterpret_cast<char*>(&t),sizeof(t));
	t=SDL_Swap64(t);
	return is;
}
#endif //SDL_HAS_64BIT_TYPE
//Float32
template<> std::ostream &pack(std::ostream &os, const Float32 &t_)
{
	//cerr<<"pack Float32\n";
	Float32 t(t_);
	reverse((char*)&t,((char*)&t)+sizeof(t));
	os.write(reinterpret_cast<const char*>(&t),sizeof(t));
	return os;
}
template<> std::istream &unpack(std::istream &is, Float32 &t)
{
	//cerr<<"unpack Float32\n";
	is.read(reinterpret_cast<char*>(&t),sizeof(t));
	reverse((char*)&t,((char*)&t)+sizeof(t));
	return is;
}
//Float64
template<> std::ostream &pack(std::ostream &os, const Float64 &t_)
{
	//cerr<<"pack Float64\n";
	Float64 t(t_);
	reverse((char*)&t,((char*)&t)+sizeof(t));
	os.write(reinterpret_cast<const char*>(&t),sizeof(t));
	return os;
}
template<> std::istream &unpack(std::istream &is, Float64 &t)
{
	//cerr<<"unpack Float64\n";
	is.read(reinterpret_cast<char*>(&t),sizeof(t));
	reverse((char*)&t,((char*)&t)+sizeof(t));
	return is;
}
//Float96
template<> std::ostream &pack(std::ostream &os, const Float96 &t_)
{
	//cerr<<"pack Float96\n";
	Float96 t(t_);
	reverse((char*)&t,((char*)&t)+sizeof(t));
	os.write(reinterpret_cast<const char*>(&t),sizeof(t));
	return os;
}
template<> std::istream &unpack(std::istream &is, Float96 &t)
{
	//cerr<<"unpack Float96\n";
	is.read(reinterpret_cast<char*>(&t),sizeof(t));
	reverse((char*)&t,((char*)&t)+sizeof(t));
	return is;
}

#endif //SDL_BYTEORDER == SDL_LIL_ENDIAN

template<> std::ostream &pack(std::ostream &os, const string &t)
{
	Uint32 sz=t.size();
	pack(os,sz);
	os.write(t.data(),sz);
	return os;
}

template<> std::istream &unpack(std::istream &is, string &t)
{
	Uint32 sz;
	unpack(is,sz);
	t.resize(sz);
	//copy_n(istream_iterator<unsigned char>(is),sz,t.begin()); //sgi ext
	//is.read(const_cast<char*>(t.data()),sz); // not portable
	for(size_t i=0;i<sz;++i)
		is.read(&t[i],1);
	return is;
}

std::ostream &operator<<(std::ostream &os, const Pack &p)
{
	p.print(os);
	return os;
}

std::istream &operator>>(std::istream &is, Pack &p)
{
	return p.unpack(is);
}

};
