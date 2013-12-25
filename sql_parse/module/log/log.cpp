#include "log.hpp"

namespace module{

	CLog::CLog(const char* logpath,const char* filename/* = LOG_NAME*/,int nlv/* = LOG_1*/,int nscan/* = DEFAULT_TIME*/)
		:m_bquit(false),
		 m_pwrite(&m_quelog1),
		 m_pread(&m_quelog2)
	{
		Init(logpath,filename,nlv,nscan);
	}

	CLog::CLog()
		:m_bquit(false),m_nprlve(DEBUG),m_nscan(DEFAULT_TIME),m_pwrite(&m_quelog1)
	{}

	CLog::~CLog()
	{
		m_bquit = true;
	}

	int CLog::DoRunning()
	{
		std::string strlog;

		std::string strpath = m_strpath;

		std::string strlogpath;

		if(!strpath.empty())
		{
			if(strpath.find_last_of('/') != (strpath.size() - 1))
			{
				strpath += '/';
			}
		}

		// strpath += GetDate();

		// strpath += m_strfilename.c_str();

		while(!m_bquit)
		{
			strlogpath = strpath + GetDate() + m_strfilename;

			static bool change = true;

			m_mutex.Lock();
			if(change)
			{
				m_pwrite = &m_quelog1;
				m_pread = &m_quelog2;
			}
			else
			{
				m_pwrite = &m_quelog2;
				m_pread = &m_quelog1;
			}
			m_mutex.Unlock();

			change = !change;

			std::ofstream fout(strlogpath.c_str(),std::ios::out | std::ios::app);

			while(!m_pread->empty())
			{
				strlog = m_pread->front();
				m_pread->pop();
				fout << strlog << std::endl;
			}
			fout.close();

			module::Sleep(m_nscan);
		}
		return 0;
	}

	std::string CLog::GetTimeString(int type)
	{

		struct tm* now = module::GetLocalTime();

		char buf[256];

		switch(type)
		{
		case enumDATE:
			{
				strftime(buf,256,"%Y-%m-%d",now);
			}
			break;

		case enumTIME:
			{
				strftime(buf,256,"%H:%M:%S",now);
			}
			break;

		case enumALL:
			{
				strftime(buf,256,"%Y-%m-%d %H:%M:%S",now);
			}
			break;

		default:
			return "";
		}

		return buf;
	}

	std::string CLog::GetDate()
	{
		return GetTimeString(enumDATE);
	}

	std::string CLog::GetTime()
	{
		return GetTimeString(enumTIME);
	}

	int CLog::Init(const char* logpath,const char* filename/* = LOG_NAME*/,int nlv/* = LOG_1*/,int nscan/* = DEFAULT_TIME*/)
	{
		if(logpath != NULL)
		{
			m_strpath = logpath;
		}
		else
			return -1;

		if(filename != NULL)
		{
			m_strfilename = filename;
		}
		else
			return -1;

		m_nprlve = nlv;

		m_nscan = nscan;

		this->CreateThread();

		return 0;
	}

	// Ñ¹ÈëÈÕÖ¾
	int CLog::PushLog(int nlv,const char* fmt,...)
	{

		if(m_nprlve >= nlv)
		{

			std::string data = GetTimeString();

			char buf[1024];	

			va_list ap;

			va_start(ap,fmt);

			vsnprintf(buf,1024,fmt,ap);

			va_end(ap);

			data += " ";

			data += buf;

			m_mutex.Lock();

			m_pwrite->push(data);

			m_mutex.Unlock();

			return 0;
		}

		return -1;
	}

}
