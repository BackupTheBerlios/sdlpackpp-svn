#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

enum ElementType
{
	ET_bool,	//              bool
	ET_Sint8,	//              char
	ET_Uint8,	//unsigned      char
	ET_Sint16,	//              short
	ET_Uint16,	//unsigned      short
	ET_Sint32,	//              long
	ET_Uint32,	//unsigned      long
	ET_Sint64,	//         long long
	ET_Uint64,	//unsigned long long
	ET_Float32, //              float
	ET_Float64, //              double
	ET_Float96, //         long double
	ET_String,	//              string
	ET_Unknown
};

struct PacketElement
{
	ElementType type;
	int length;	//default=1
	string name;
};

struct Packet
{
	string name;
	vector<PacketElement> elements;
};

vector<string> include_files;
vector<Packet> packets;

struct ET_map
{
	ElementType type;
	const char **names;
};

const char *bool_names[]={"bool", 0};
const char *Sint8_names[]={"Sint8", "char", "signed char", 0};
const char *Uint8_names[]={"Uint8", "byte", "unsigned char", 0};
const char *Sint16_names[]={"Sint16", "short", "signed short", 0};
const char *Uint16_names[]={"Uint16", "unsigned short", 0};
const char *Sint32_names[]={"Sint32", "int", "long", "signed long", 0};
const char *Uint32_names[]={"Uint32", "unsigned int", "unsigned long", "unsigned", 0};
const char *Sint64_names[]={"Sint64", "long long", "signed long long", "long long signed", 0};
const char *Uint64_names[]={"Uint64", "unsigned long long", "long long unsigned", 0};
const char *Float32_names[]={"Float32", "float", 0};
const char *Float64_names[]={"Float64", "double", 0};
const char *Float96_names[]={"Float96", "long double", 0};
const char *String_names[]={"std::string", "string", "String", 0};

ET_map typemap[]={
	{ET_bool, bool_names},
	{ET_Sint8, Sint8_names},
	{ET_Uint8, Uint8_names},
	{ET_Sint16, Sint16_names},
	{ET_Uint16, Uint16_names},
	{ET_Sint32, Sint32_names},
	{ET_Uint32, Uint32_names},
	{ET_Sint64, Sint64_names},
	{ET_Uint64, Uint64_names},
	{ET_Float32, Float32_names},
	{ET_Float64, Float64_names},
	{ET_Float96, Float96_names},
	{ET_String, String_names},
	{ET_Unknown, 0}
};

ET_map *map_type(const string &t)
{
	for(int i=0; typemap[i].names; ++i)
	{
		for(const char **n=typemap[i].names; *n; ++n)
			if(t==*n)
				return &typemap[i];
	}
	return 0;
}

inline void output_toupper(ostream &os, const string &str)
{
	transform(str.begin(), str.end(),
			ostream_iterator<char>(os),
			std::ptr_fun<int,int>(std::toupper));
}

int main(int argc, char **argv)
{
	if(argc!=2)
	{
		cerr<<argv[0]<<" file.pack\n";
		return 1;
	}
	ifstream f(argv[1]);
	string line;
	Packet *pack;
	while(getline(f,line))
	{
		istringstream iss(line);
		string cmd;
		if(!(iss>>cmd))
			continue;
		//cerr<<"cmd = "<<cmd<<endl;

		if(cmd=="include")
		{
			string inc;
			iss>>inc;
			include_files.push_back(inc);
		}
		else if(cmd=="pack")
		{
			packets.resize(packets.size()+1);
			pack=&packets[packets.size()-1];
			iss>>pack->name;
			//cerr<<"name = "<<pack->name<<endl;
		}
		else if(cmd=="elem")
		{
			string type,name,word;
			while(iss>>word)
			{
				if(!word.size())
					continue;
				type=(type.size()?(type+" "):string(""))+name;
				name=word;
			}
			int len=1;
			{
				unsigned int a=name.find_last_of('[');
				unsigned int b=name.find_last_of(']');
				if(a>b || 
						(a!=string::npos && b==string::npos) ||
						(b!=string::npos && a==string::npos)
				  )
				{
					cerr<<name<<": mismatched []'s!\n";
					return 1;
				}
				if(a!=string::npos)
				{
					string num=name.substr(a+1,b);
					name=name.substr(0,a);
					if(!num.size())
						len=-1;
					else
					{
						
						istringstream iss2(num);
						if(!(iss2>>len))
						{
							cerr<<num<<": integer parse error!\n";
						}
					}
				}
			}
			//cerr<<"type = "<<type<<endl;
			//cerr<<"name = "<<name<<endl;
			ET_map *et=map_type(type);
			if(!et)
			{
				cerr<<type<<": unknown type!\n";
				return 1;
			}
			//cerr<<"et = "<<et->type<<" "<<et->names[0]<<endl;
			PacketElement pe;
			pe.name=name;
			pe.type=et->type;
			pe.length=len;
			pack->elements.push_back(pe);
		}
		else
		{
			cerr<<cmd<<": command unknown!\n";
			return 1;
		}
	}
	f.close();
	string basename=string(argv[1]).substr(0,string(argv[1]).find_last_of('.'));
	//cerr<<"basename='"<<basename<<"'\n";
	ofstream cc((basename+".cc").c_str());
	if(cc.bad())
	{
		cerr<<basename<<".cc: failed to open!\n";
		return 2;
	}
	ofstream h((basename+".h").c_str());
	if(h.bad())
	{
		cerr<<basename<<".h: failed to open!\n";
		return 2;
	}
	ofstream html((basename+".html").c_str());
	if(html.bad())
	{
		cerr<<basename<<".html: failed to open!\n";
		return 2;
	}
	cc<<"#include \""<<basename<<".h\"\n\n";
	h<< "#ifndef pack_"<<basename<<"_h\n"
		"#define pack_"<<basename<<"_h\n\n"
		"#include \"pack++.h\"\n";
	html<<"<HTML><HEAD><TITLE>"<<basename<<"</TITLE></HEAD><BODY><H1>"<<basename<<"</H1>\n";
	for(vector<string>::iterator i=include_files.begin();
			i!=include_files.end(); ++i)
	h<< "#include "<<*i<<endl<<endl;
	h<< "namespace "<<basename<<"\n"
		"{\n\n";
	cc<<"\n"
		"namespace "<<basename<<"\n"
		"{\n\n";
	h<<"struct Pack : public Packer::Pack\n";
	h<<"{\n";
	h<<"\tenum Type {\n";
	for(vector<Packet>::iterator i=packets.begin(); i!=packets.end(); )
	{
		h<<"\t\t";
		output_toupper(h,i->name);
		if(++i!=packets.end())
			h<<',';
		h<<'\n';
	}
	h<<"\t};\n\n";
	h<<"\tvirtual Type pack_type() const=0;\n";
	h<<"};\n\n";
	h<<"Pack *unpack(std::istream &is);\n\n";
	cc<<"Pack *unpack(std::istream &is)\n"
		"{\n"
		"\tUint32 type;\n"
		"\tPacker::unpack(is,type);\n"
		"\tPack *ptr=0;\n"
		"\tswitch(type)\n"
		"\t{\n";
	for(vector<Packet>::iterator i=packets.begin(); i!=packets.end(); ++i)
	{
		cc<<"\t\tcase "<<basename<<"::Pack::";
		output_toupper(cc,i->name);
		cc<<":\n"
			"\t\t\t{\n"
			"\t\t\t\tptr=new "<<i->name<<";\n"
			"\t\t\t\tif(!ptr->unpack(is).good())\n"
			"\t\t\t\t{\n"
			"\t\t\t\t\tdelete ptr;\n"
			"\t\t\t\t\tptr=0;\n"
			"\t\t\t\t}\n"
			"\t\t\t}\n"
			"\t\t\tbreak;\n";
	}
	cc<<"\t}\n"
		"\treturn ptr;\n"
		"}\n\n";
	for(vector<Packet>::iterator i=packets.begin(); i!=packets.end(); ++i)
	{
		cc<<string(80,'/')<<"\n\n";
		h<<string(80,'/')<<"\n\n";
		h<< "struct "<<i->name<<" : public "<<basename<<"::Pack\n";
		h<<"{\n";
		h<<"\t"<<i->name<<"() {}\n";
		h<<"\tvirtual ~"<<i->name<<"() {}\n";
		h<<"\t"<<i->name<<"(";
		for(vector<PacketElement>::iterator e=i->elements.begin();
				e!=i->elements.end(); ++e)
		{
			if(e!=i->elements.begin())
				h<<',';
			h<<'\n';
			h<<"\t\t\tconst ";
			if(e->length<0)
				h<<"std::vector<";
			h<<typemap[e->type].names[0];
			if(e->length<0)
				h<<">";
			h<<" ";
			if(e->length<2)
				h<<'&';
			h<<e->name;
			if(e->length>1)
				h<<"["<<e->length<<"]";
		}
		h<<")";
		bool first=true;
		for(vector<PacketElement>::iterator e=i->elements.begin();
				e!=i->elements.end(); ++e)
		{
			if(e->length>1)
				continue;
			h<<"\n\t\t";
			if(first)
			{
				h<<": ";
				first=false;
			}
			else
				h<<", ";
			h<<e->name<<'('<<e->name<<')';
		}
		h<<"\n\t{\n";
		for(vector<PacketElement>::iterator e=i->elements.begin();
				e!=i->elements.end(); ++e)
		{
			if(e->length<2)
				continue;
			h<<"\t\t";
			h<<"std::copy("<<e->name<<", &"<<e->name<<'['<<e->length<<"], this->"<<e->name<<");\n";
		}
		h<<"\t}\n";
		h<<"\tvirtual inline "<<basename<<"::Pack::Type pack_type() const {return ";
		output_toupper(h,i->name);
		h<<";}\n";
		
		html<<"<H2>"<<i->name<<"</H2>\n"
			<<"Type=";
		output_toupper(html,i->name);
		html<<"<TABLE border=1 cellspacing=0>\n"
			<<"<TR><TH>Name</TH><TH>Type</TH><TH>Length</TH></TR>\n";
		for(vector<PacketElement>::iterator e=i->elements.begin();
				e!=i->elements.end(); ++e)
		{
			html<<"<TR>"
				<<"<TD>"<<e->name<<"</TD>"
				<<"<TD>"<<typemap[e->type].names[0]<<"</TD>"
				<<"<TD>";
			if(e->length==-1)
				html<<"Variable";
			else
				html<<e->length;
			html<<"</TD>\n";
		}
		html<<"</TABLE>\n";

		// print
		h<<"\tvirtual std::ostream &print(std::ostream &os) const;\n";
		cc<<"std::ostream &"<<i->name<<"::print(std::ostream &os) const\n"
			"{\n"
			"\tos<<\""<<i->name<<":\"<<std::endl;\n";
		for(vector<PacketElement>::iterator e=i->elements.begin();
				e!=i->elements.end(); ++e)
		{
			cc<<"\tos<<\"\\t"<<e->name<<"=\";";
			if(e->length<=1)
				cc<<" Packer::print(os,"<<e->name<<")<<std::endl;\n";
			else
				cc<<" Packer::print(os,"<<e->name<<","<<e->length<<")<<std::endl;\n";
		}
		cc<<"\treturn os;\n"
			"}\n\n";
		
		// pack
		h<<"\tvirtual std::ostream &pack(std::ostream &os) const;\n";
		cc<<"std::ostream &"<<i->name<<"::pack(std::ostream &os) const\n"
			"{\n"
			"\tPacker::pack<Uint32>(os,pack_type());\n";
		for(vector<PacketElement>::iterator e=i->elements.begin();
				e!=i->elements.end(); ++e)
		{
			if(e->length>1)
				cc<<"\tPacker::pack(os,"<<e->name<<","<<e->length<<");\n";
			else
				cc<<"\tPacker::pack(os,"<<e->name<<");\n";
		}
		cc<<"\treturn os;\n"
			"}\n\n";

		// unpack
		h<<"\tvirtual std::istream &unpack(std::istream &is);\n";
		cc<<"std::istream &"<<i->name<<"::unpack(std::istream &is)\n"
			"{\n"
			"\t//Packer::unpack<unsigned long>(is,pack_type());\n";
		for(vector<PacketElement>::iterator e=i->elements.begin();
				e!=i->elements.end(); ++e)
		{
			if(e->length>1)
				cc<<"\tPacker::unpack(is,"<<e->name<<","<<e->length<<");\n";
			else
				cc<<"\tPacker::unpack(is,"<<e->name<<");\n";
		}
		cc<<"\treturn is;\n"
			"}\n\n";

		// elements
		for(vector<PacketElement>::iterator e=i->elements.begin();
				e!=i->elements.end(); ++e)
		{
			h<<"\t";
			if(e->length<0)
				h<<"std::vector<";
			h<<typemap[e->type].names[0];
			if(e->length<0)
				h<<">";
			h<<" "<<e->name;
			if(e->length>1)
				h<<"["<<e->length<<"]";
			h<<";\n";
		}
		h<<"};\n\n";
	}
	cc<<"};\n";
	h<<string(80,'/')<<"\n\n"
		"};\n"
		"#endif\n";
	html<<"</BODY></HTML>";
}
