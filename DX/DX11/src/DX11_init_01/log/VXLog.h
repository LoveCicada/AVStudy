
#pragma once

#include <fstream>
#include <string>


class VXLog
{
public:
	static VXLog* GetInstance()
	{
		static VXLog instance;
		return &instance;
	}

public:
	void Write(std::string& str);
	void Write(const char* c);
	void Write(std::string& str, int nLine, std::string& strFun);
	void Write(const char* c, int nLine, std::string& strFun);
	void Write(const char* c, int nLine, const char* strFun);
	//void Write(const char* c, std::string& strLine, const char* strFun)

private:
	VXLog();
	~VXLog();

private:
	std::ofstream m_ofs;

};

#define LOG(str) VXLog::GetInstance()->Write(str);
#define LOGEX(str, n, strFun) VXLog::GetInstance()->Write(str, n, strFun);
#define LOGS(str) LOGEX(str, __LINE__, __FILE__);