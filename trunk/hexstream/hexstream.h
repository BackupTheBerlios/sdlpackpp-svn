#ifndef hexstream_h
#define hexstream_h

#include <streambuf>
#include <iostream>

class hexstream;

class hexstreambuf : public std::streambuf
{
	hexstream &hs;
	std::ostream &os;
	size_t offset;

	char buf[16];
	char old[16];
	bool rle,in_rle;
protected:
	virtual int_type overflow(int_type c=EOF);
	virtual int sync();
public:
	hexstreambuf(hexstream &hs, bool rle=false, std::ostream &os=std::cout);
	virtual ~hexstreambuf();
	void reset() { offset=0; }
};

class hexstream : public hexstreambuf, public std::ostream
{
public:
	hexstream(bool rle=false, std::ostream &os=std::cout)
			: hexstreambuf(*this,rle,os)
			, std::ostream(this) {}
};

#endif
