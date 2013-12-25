#include "common.hpp"

namespace module{

	void Sleep(unsigned long long usec)
	{
		unsigned long long nTime = 0;
#ifdef WIN32
		LARGE_INTEGER nStartCounter;
		::QueryPerformanceCounter(&nStartCounter);

		LARGE_INTEGER nFrequency;
		::QueryPerformanceFrequency(&nFrequency);

		LARGE_INTEGER nStopCounter;

		do
		{
			::QueryPerformanceCounter(&nStopCounter);
			::Sleep(1);
			nTime = (1000*(nStopCounter.QuadPart - nStartCounter.QuadPart)) / nFrequency.QuadPart;
		}
		while (nTime < usec);
#else
		struct timeval start;
		gettimeofday( &start, NULL );

		struct timeval end;

		do
		{
			usleep(10);
			gettimeofday( &end, NULL );
			nTime = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
		}
		while(nTime < (usec*1000));
#endif
	}
	
	unsigned long long GetTickTime()
	{
		unsigned long long utime = 0;
#ifdef WIN32
		LARGE_INTEGER nStartCounter;
		::QueryPerformanceCounter(&nStartCounter);
		LARGE_INTEGER nFrequency;
		::QueryPerformanceFrequency(&nFrequency);

		utime = ( 1000 * nStartCounter.QuadPart ) / nFrequency.QuadPart;
#else
		struct timeval start;
		gettimeofday( &start, NULL );
		utime = 1000000 * ( start.tv_sec ) + start.tv_usec;

		utime = utime/1000;
#endif
		return utime;
	}

	tm* GetLocalTime()
	{	
		struct tm* now;

		time_t locatime;

		time(&locatime);

		now = localtime(&locatime);

		return now;
	}
	/*
	char * strupr(char *pIn) 
	{
		char *p = pIn;
		while (*p != '\0')
		{
			if(*p >= 'a' && *p <= 'z')
				*p -= 0x20;
			p++;
		}
		return pIn;
	}
	*/
	int stricmp(char const *pIn1,char const *pIn2)
	{	
		char ch1 = 0,ch2 = 0;
		int ret = 0,len = 0,len1 = 0,len2 = 0,i = 0;

		len1 = strlen(pIn1);
		len2 = strlen(pIn2);

		if(len1 > len2)
			len = len2;
		else
			len = len1;

		for(i = 0;i < len;i++)
		{
			ch1 = pIn1[i];
			ch1 = tolower(ch1);
			ch2 = pIn2[i];
			ch2 = tolower(ch2);
			ret = ch1 - ch2;
			if(ret != 0)
				i = len;
		}	

		if(ret == 0)
		{
			if (len1 < len2)
				ret = -1;
			else if(len1 > len2)
				ret = 1;
		}
		return ret;
	}

	int strnicmp(char const* pIn1,char const *pIn2, unsigned int cnt)
	{
		char ch1 = 0,ch2 = 0;
		int ret = 0,len = 0,len1 = 0,len2 = 0,i = 0;


		len1 = strlen(pIn1);
		len2 = strlen(pIn2);
		if(len1 > len2)
			len = len2;
		else
			len = len1;

		if(len > (int)cnt)
			len = cnt;

		for(i = 0;i < len;i++)
		{
			ch1 = pIn1[i];
			ch1 = tolower(ch1);
			ch2 = pIn2[i];
			ch2 = tolower(ch2);
			ret = ch1 - ch2;
			if(ret != 0)
				i = len;
		}	

		if(ret == 0)
		{
			if (len1 < len2)
				ret =- 1;
			else if(len1 > len2)
				ret = 1;
		}
		return ret;
	}
}
