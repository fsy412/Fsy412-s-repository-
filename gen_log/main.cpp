#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <map>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <module/thread/mutex.hpp>
#include <module/thread/semaphore.hpp>
#include <module/tinyxml/tinyxml.h>
#include <queue>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h> 
#include <pthread.h>
#include <ext/hash_map>


#ifdef WIN32
#include <Windows.h>
#endif

#define HEADER 16

#define TIME_LIMIT 15*60

typedef unsigned char UNCHAR;

enum type{IPADDR=1,STR,TIME,NUMBER};


typedef struct{
	std::string s;	
	int type;
}ValueType;

typedef std::vector<ValueType> AttrName;
typedef std::map<std::string,int> AttrDic;
typedef std::map<std::string,ValueType> AttrValue;
char peer0_0[] = {
	0x3c, 0x31, 0x33, 0x34, 0x3e, 0x20, 0x31, 0x20, 
	0x44, 0x65, 0x63, 0x20, 0x30, 0x33, 0x20, 0x31, 
	0x31, 0x3a, 0x32, 0x35, 0x3a, 0x30, 0x38, 0x20, 
	0x2d, 0x20, 0x48, 0x33, 0x43, 0x20, 0x2d, 0x20, 
	0x4e, 0x41, 0x54, 0x34, 0x34, 0x34, 0x3a, 0x75, 
	0x73, 0x65, 0x72, 0x62, 0x61, 0x73, 0x65, 0x64, 
	0x41, 0x20, 0x5b, 0x30, 0x20, 0x31, 0x30, 0x2e, 
	0x30, 0x2e, 0x30, 0x2e, 0x31, 0x20, 0x2d, 0x20, 
	0x32, 0x30, 0x2e, 0x30, 0x2e, 0x30, 0x2e, 0x32, 
	0x30, 0x30, 0x20, 0x2d, 0x20, 0x33, 0x30, 0x37, 
	0x33, 0x20, 0x34, 0x30, 0x39, 0x36, 0x5d };

int limit,timelimit;

std::string provice,prefix;

module::CMutex mutex;

module::CSemaphore sema;

unsigned long countnow;

unsigned long ID;

std::queue<std::string > content_buffer;

AttrName _packageAttrName;

AttrName _fileAttrName;

AttrDic _dic;

AttrValue _attrvalue;

int _packagetotallength;

int _filecount,_filetimeout;

int _logcount = 0,_logtime,_sn=0,_logidentify=0;

std::string _logfilename ="";

std::string _proviceidentify = "TJ";

std::ofstream _fileptr;

class str_hash
{
    public:
    size_t operator()(const std::string& str) const
    {
        unsigned long __h = 0;
        for (size_t i = 0 ; i < str.size() ; i ++)
            __h = 5*__h + str[i];
        return size_t(__h);
    }
};
 
class str_compare
{
    public:
        bool operator()(const std::string& str1,const std::string& str2)const
        {return   str1==str2;}
};
  
__gnu_cxx::hash_map<std::string, unsigned long, str_hash, str_compare> hashmap;
bool enable;

int GetMonthNumber(std::string month)
{
	int base=0;
	std::string mon=month;
	std::transform(mon.begin(),mon.end(),mon.begin(),::toupper);
	if(mon=="JAN")
	{
		return (base+0);

	}else if(mon=="FEB")
	{

		return (base+1);

	}else if(mon=="MAR")
	{

		return (base+2);
	}
	else if(mon=="APR")
	{

		return (base+3);
	}
	else if(mon=="MAY")
	{

		return (base+4);
	}
	else if(mon=="JUN")
	{

		return (base+5);
	}else if(mon=="JUL")
	{

		return (base+6);
	}
	else if(mon=="AUG")
	{
		return (base+7);

	}
	else if(mon=="SEP"||mon=="SEPT")
	{
		return (base+8);

	}
	else if(mon=="OCT")
	{
		return (base+9);

	}else if(mon=="NOV")
	{
		return (base+10);
	}
	else if(mon=="DEC")
	{
		return (base+11);
	}
	return -1;
}

time_t  convert_timestr_to_time_t(std::string mon,std::string day,std::string time_of_day)
{
	if((mon == "") || (day =="") || (time_of_day==""))
	{
		return 0;
	}

	struct tm*	now;
	time_t		ltime;
	time(&ltime);
#ifdef WIN32
	now = localtime(&ltime);
#else	//Unix下的多线程版本
# ifdef HP_UX
	now=localtime(&ltime);
# else
	struct tm tmpnow;
	now=localtime_r(&ltime,&tmpnow);
#endif
#endif
	int i_day=atoi(day.c_str());
	now->tm_mon=GetMonthNumber(mon);
	if(now->tm_mon == -1)
	{
		return 0;
	}
	now->tm_mday=i_day;
	std::string::size_type pos;
	std::string pattern=":";
	std::string str=time_of_day;
	str+=pattern;
	int size=str.size();
	int k=0;
	for(int i=0; i<size; i++)
	{
		pos=str.find(pattern,i);
		if(pos<size)
		{
			std::string s=str.substr(i,pos-i);
			if(k==0)//hour
			{
				sscanf(s.c_str(),"%02d",&(now->tm_hour));
				k++;
			}else if(k==1)//min
			{
				sscanf(s.c_str(),"%02d",&(now->tm_min));
				k++;
			}else if(k==2)//sec
			{
				sscanf(s.c_str(),"%02d",&(now->tm_sec));
				k++;
			}else
			{
				break;
			}
			i=pos+pattern.size()-1;
		}
	}
	time_t rettime=mktime(now);
	return rettime;
}

std::string findStr(std::string name)
{
	AttrValue::iterator iter = _attrvalue.find(name);

	if (iter != _attrvalue.end())
	{
		return iter->second.s;
	}
	else
	{
		return "";
	}
}

int praseXmlToAttrName(AttrName& attr,std::string filename)
{
	int index = 0;
	
	TiXmlDocument doc(filename.c_str());

	if(!doc.LoadFile())
	{
		std::cout<<"Load xml file err ["<<filename<<"]"<<std::endl;
			
		return -1;
	}

	TiXmlElement* root = doc.RootElement();

	if(!root) return -1;

	TiXmlElement *child = root->FirstChildElement();

	while(child!=NULL)
	{
		std::cout<<child->Value()<<std::endl;

		ValueType v ;

		v.s = child->Value();

		v.type = atoi(child->GetText());

		attr.push_back(v);

		index++;

		child = child->NextSiblingElement();
	}	
	return 0;
}

int encodeToFile(char* p)
{
	unsigned long diff;
	p[0] = 0x31;
	diff = 0;
	std::string s = findStr("type");

	if(s == "")
	{
		return -1;
	}

	if(s[s.length() - 1] == 'A' )
	{
		memcpy(p+1,"create",6);
		if (enable)
		{
			std::string sip = findStr("sip");
			if (sip == "" )
			{	
				return -1;
			}
			std::string sport = findStr("sport");
			if (sport =="")
			{
				return -1;
			}
			hashmap[sip + sport] = convert_timestr_to_time_t(findStr("mon"), findStr("day"), findStr("hms")) ;
		}
	}
	else
	{
		if (enable)
		{
			std::string sip = findStr("sip");
			if (sip == "" )
			{	
				return -1;
			}
			std::string sport = findStr("sport");
			if (sport =="")
			{
				return -1;
			}
			
			if(hashmap.find(sip + sport) != hashmap.end())
			{
				unsigned long value = hashmap[sip + sport];
				unsigned long now = convert_timestr_to_time_t(findStr("mon"), findStr("day"), findStr("hms"));
				unsigned long diff = difftime(now, value); 
				hashmap.erase(sip + sport);
			}
			else
			{
				diff = 0;
			}
		}
		memcpy(p+1,"close ",6);
	}
	
	s = findStr("sip");

	if(s == "")
	{
		return -1;
	}

	u_long ip = inet_addr(s.c_str());

	memcpy(p+7, &ip, 4);

	s = findStr("tip");

	if(s == "")
	{
		return -1;
	}

	ip = inet_addr(s.c_str());

	memcpy(p+11,&ip,4);

	s = findStr("sport");
	
	if(s == "")
	{
		return -1;
	}

	int a = atoi(s.c_str());

	memcpy(p+15, &a,2);

	s = findStr("tport");
	
	if(s == "")
	{
		return -1;
	}

	a = atoi(s.c_str());

	memcpy(p+17, &a, 2);

	time_t t = convert_timestr_to_time_t(_attrvalue.find("mon")->second.s,_attrvalue.find("day")->second.s,_attrvalue.find("hms")->second.s);

	if(t == 0)
	{
		return -1;
	}

	memcpy(p+19, &t, 8);
	if (!enable)
	{
		p[27] = p[28] =p[29] =p[30]=0x00;
	}else
	{
		memcpy(p+27, &diff, 4);
 	}

	return 0;
}

int split(const std::string& str, std::vector<std::string>& ret_, std::string sep = " ")
{
	if (str.empty())
	{
		return 0;
	}

	std::string tmp;

	std::string::size_type pos_begin = str.find_first_not_of(sep);

	std::string::size_type comma_pos = 0;

	int index = 0;

	while (pos_begin != std::string::npos)
	{
		if(index == _packageAttrName.size())
		{
			return -1;
		}
	
		comma_pos = str.find(sep, pos_begin);

		if (comma_pos != std::string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);

			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);

			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			//ret_.push_back(tmp);

			ValueType v;

			v.s = tmp;

			v.type = _packageAttrName[index].type;
			
			_attrvalue.insert(std::make_pair(_packageAttrName[index].s,v));

			index++;			

			tmp.clear();
		}
	}

	if(index != _packageAttrName.size())
	{
		return -1;
	}
	else
	{
		return 0;
	}

}

std::string replace(const std::string& str, const std::string& src, const std::string& dest)
{
	std::string ret;

	std::string::size_type pos_begin = 0;

	std::string::size_type pos       = str.find(src);

	while (pos != std::string::npos)
	{
		ret.append(str.data() + pos_begin, pos - pos_begin);

		ret += dest;

		pos_begin = pos + 1;

		pos       = str.find(src, pos_begin);

	}
	if (pos_begin < str.length())
	{
		ret.append(str.begin() + pos_begin, str.end());
	}

	return ret;
}

int praseSysLogPackage(char* buffer)
{
	std::string p(buffer);

	std::vector<std::string> valuevec;

	p = replace(p,"[","");

	p = replace(p,"]","");

	if(split(p,valuevec," ")!=0)
	{
			std::cout<<"package is incorrect."<<std::endl;

			return -1;
	}
	else
	{	
			return 0;
	}

}

void* generate_file(void* argv)
{
	FILE * pFile;
	static unsigned int SN;
	static char clast_time[16];
	struct tm* now;
	char ctime[17];
	char cid[11] = {'\0'};
	char csn[5] = {'\0'};

	while(1)
	{
			if(sema.Wait())
			{
					continue;
			}
			
			time_t locatime;
			time(&locatime);
			now = localtime(&locatime);
			strftime(ctime,16,"%Y%m%d%H%M%S",now);
			strcmp(clast_time, ctime) == 0 ? SN ++ : SN = 0;
			memcpy(clast_time, ctime, 16);
			ctime[16] = '\0';
			sprintf(cid, "%10d", ID++);
			for (int i = 0 ;i < 9 ; i++)
			{
				if (cid[i] == 32) cid[i] = '0';
			}
		
			sprintf(csn, "%4d", SN);
			for (int i = 0 ;i < 3 ; i++)
			{
				if (csn[i] == 32) csn[i] = '0';
			}
			std::string file_name = "..//log//" + prefix + "_" + provice + cid + "_"+ ctime + "_" + csn + ".data";
		
			pFile = fopen (file_name.c_str(), "wb");
		
			if(pFile == NULL)
			{
				std::cout<<"Can't open log file ["<<file_name<<"]"<<std::endl;
				
				exit(0); 
			}	
			for(int i = 0;i<countnow; i++)
			{
				mutex.Lock();

				if(ID==9999999999)
				{
					ID = 0;

				}

				if(content_buffer.empty())
				{
					break;
				}
				
				std::string str = content_buffer.front();
						
				content_buffer.pop();

				mutex.Unlock();
								
				if(praseSysLogPackage((char*)str.c_str())!=0)
				{
					continue;
				}
						
				char p[31] ={0};
						
				if(encodeToFile(p)!=0)
				{
					std::cout<<"package encode error."<<std::endl;
					
					_attrvalue.clear();

					continue;
				}

				_attrvalue.clear();
						
				fwrite (p, sizeof(char), 31, pFile);

			}
			
			fclose (pFile);  

	}
	
}

void write(std::string prefix, std::string province , const char* content)
{
	
	static unsigned long count;
 	static struct timeval start;
	static struct timeval end;
	static bool once = true;
	mutex.Lock();
	content_buffer.push(content);	
	count++;
	gettimeofday( &start, NULL );	
	unsigned long  duration = (1000000 * ( start.tv_sec - end.tv_sec ) + start.tv_usec - end.tv_usec )/1000000;
	
	if (once) 
	{
		duration = 0;
		once = false;
	}
	if (duration > TIME_LIMIT)
	{ 		
		//generate_file(prefix, province, ID++);
		countnow = count;
		sema.Post();
		count = 0;
	}
	else if (count == limit)
	{
		//generate_file(prefix, province, ID++);
		countnow = count;
		sema.Post();
		count = 0;
	}
	gettimeofday( &end, NULL );
	mutex.Unlock();
	
}




int main(int argc,char** argv)
{
	if(argc!= 6)
	{
		std::cout<<"Useage :  bind port, limit count, limit timeout,  provice identity ,encode timediff flag(0 or 1)."<<std::endl;

		return 0;
	}
	

	UNCHAR recvbuf[1024]={0};

	UNCHAR filebuf[1024] = {0};	

	if(praseXmlToAttrName(_fileAttrName,"..//dic//dic.xml") == -1)
	{
		return 0;
	}

	if(praseXmlToAttrName(_packageAttrName,"..//dic//prase.xml")==-1)
	{
		return 0;
	}

	
	int sin_len;
	char message[1024];

	int socket_descriptor;
	struct sockaddr_in sin;
	printf("Waiting for data form sender \n");

	bzero(&sin,sizeof(sin));
	sin.sin_family=AF_INET;
	sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_port=htons(atoi(argv[1]));
	sin_len=sizeof(sin);

	socket_descriptor=socket(AF_INET,SOCK_DGRAM,0);
	
	bind(socket_descriptor,(struct sockaddr *)&sin,sizeof(sin));

	limit = atoi(argv[2]);

	timelimit = atoi(argv[3])*60;

	provice = argv[4];
	
	enable = atoi(argv[5]);

	prefix="NATLOG";

	int c1 = 0;

	int c2 = 1;

	pthread_t ntid;

	int err;

	err = pthread_create(&ntid, NULL, &generate_file, NULL);
	
	if(err !=0)
		{
			std::cout<<"Can't create a generate file pthread!"<<std::endl;

			return 0;
		}

	while(1)
	{
	    	int ret = recvfrom(socket_descriptor, message, 1024, 0, NULL, NULL);

		c1++;

		if(c1 == 5000)
		{
			std::cout<<"Recv "<<c2*5000<<std::endl;
			c1 = 0;
			c2++;
		}

		write("NATLOG",provice ,message);	
	
		memset(message,0,1024);
	}

	close(socket_descriptor);
/*
	std::cout<<"Start to process"<<std::endl;

	struct timeval start,end;

	gettimeofday( &start, NULL );	

	for(int i=0;i<atoi(argv[1]);i++)
	{
		write("NATLOG","12",peer0_0);				
	}


	gettimeofday( &end, NULL );

	long result = (1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec)/1000000;

	std::cout<<"USED "<<result<<std::endl;
*/
	return 0;

}


