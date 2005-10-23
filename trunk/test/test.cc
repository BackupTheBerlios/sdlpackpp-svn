#include "t1.h"
#include <sstream>
#include "hexstream.h"

using namespace std;
using namespace t1;

#define MK32IDC(a,b,c,d) ((a)<<24|(b)<<16|(c)<<8|(d))
#define MK32IDS(a) ((a)[0]<<24|(a)[1]<<16|(a)[2]<<8|(a)[3])

int main(int argc, char **argv)
{
	hexstream hex;
	
	Net_Ping ping;
	ping.time=MK32IDC('A','B','C','D');
	cout<<ping;

	cout<<"packed=\n";
	ping.pack(hex)<<flush;

	stringstream ss;
	ping.pack(ss);
	istringstream iss(ss.str());
	t1::Pack *p=t1::unpack(iss);
	cout<<*p;
	delete p;

	hex.reset();
	Net_Chat chat;
	chat.time=MK32IDS("time");
	chat.name="LIM";
	chat.message="This is a test!";
	ss.str("");
	chat.pack(ss);
	//ss.seekg(0);
	cout<<"packed=\n";
	hex<<ss.str()<<flush;
	p=t1::unpack(ss);
	cout<<*p;
	delete p;

	hex.reset();
	Net_Names names;
	names.names.push_back("Albert");
	names.names.push_back("Bonnie");
	names.names.push_back("Charlie");
	names.names.push_back("Deborah");
	names.triple[0]=M_PI;
	names.triple[1]=M_E;
	names.triple[2]=22/7.0;
	cout<<"size="<<names.size()<<endl;
	ss.str("");
	names.pack(ss);
	cout<<"packed=\n";
	hex<<ss.str()<<flush;
	p=t1::unpack(ss);
	cout<<*p;
	delete p;
}
