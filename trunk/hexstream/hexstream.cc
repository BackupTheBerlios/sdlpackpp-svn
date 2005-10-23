#include "hexstream.h"
#include <iomanip>
#include <cctype>
#include <string>

using namespace std;

hexstreambuf::hexstreambuf(hexstream &hs, bool rle, ostream &os)
	: hs(hs)
	, os(os)
	, offset(0)
	, rle(rle)
	, in_rle(false)
{
	setg(0,0,0);
	setp(buf,buf+16);
}

hexstreambuf::~hexstreambuf()
{
	sync();
}

hexstreambuf::int_type hexstreambuf::overflow(int_type c)
{
	if(!os.good())
		return EOF;
	if(sync())
		return EOF;
	sputc(c);
	return 0;
}

int hexstreambuf::sync()
{
	const int len=pptr()-pbase();
	if(!len)
		return 0;
	if(!rle || !offset || memcmp(old,buf,16))
	{
		ios_base::fmtflags flags=os.flags();
		in_rle=false;
		os<<hex<<setw(sizeof(offset)*2)<<setfill('0')<<offset<<": ";
		for(int i=0;i<len;i++)
		{
			if(i%4==0)
				os<<' ';
			os<<hex<<setw(2)<<setfill('0')<<((unsigned int)(unsigned char)(pbase()[i])&0xff);
		}
		os.flags(flags);
		os<<string(2*(16-len)+(16-len)/4,' ')<<"  ";
		for(int i=0;i<len;i++)
		{
			const char c=pbase()[i];
			if(isprint(c))
				os<<c;
			else
				os<<'.';
		}
		os<<endl;
		memcpy(old,buf,16);
	}
	else if(!in_rle)
	{
		in_rle=true;
		os<<'+'<<endl;
	}
	offset+=len;
	setp(pbase(),epptr());
	return !os.good()?-1:0;
}
