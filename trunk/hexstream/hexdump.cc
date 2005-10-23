#include "hexstream.h"
#include <fstream>
#include <cerrno>

using namespace std;

int main(int argc, char **argv)
{
	istream *in=&cin;
	if(argc>1)
	{
		in=new ifstream(argv[1]);
		if(!*in)
		{
			perror(argv[1]);
			return 1;
		}
	}
	char buf[16];
	hexstream hex(true);
	while(in->read(buf,16) || in->gcount())
	{
		int n=in->gcount();
		hex.write(buf,n);
	}
	if(in!=&cin)
		delete in;
	return 0;
}
