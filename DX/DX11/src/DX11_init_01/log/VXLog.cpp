
#include "VXLog.h"
#include <iostream>

VXLog::VXLog()
{
	m_ofs.open("DX.log", std::ios_base::app);
}

VXLog::~VXLog()
{
	m_ofs.close();
}

void VXLog::Write(std::string& str)
{
	m_ofs.write(str.c_str(), str.length());
	m_ofs << std::endl;
	m_ofs.flush();
}

void VXLog::Write(const char* c)
{
	if (!c)
		return;

	int nLen = strlen(c);
	m_ofs.write(c, nLen);
	m_ofs << std::endl;
	m_ofs.flush();
}

void VXLog::Write(std::string& str, int nLine, std::string& strFun)
{
	std::string strDst = str + " " + std::to_string(nLine) + " " + strFun;
	m_ofs.write(strDst.c_str(), strDst.length());
	m_ofs << std::endl;
	m_ofs.flush();
}

void VXLog::Write(const char* c, int nLine, std::string& strFun)
{
	if (!c)
		return;

	int nLen = strlen(c);
	m_ofs.write(c, nLen);

	std::string strDst = " " + std::to_string(nLine) + " " + strFun;
	m_ofs.write(strDst.c_str(), strDst.length());

	m_ofs << std::endl;
	m_ofs.flush();
}

void VXLog::Write(const char* c, int nLine, const char* strFun)
{
	if (!c)
		return;

	int nLen = strlen(c);
	m_ofs.write(c, nLen);

	std::string strDst = " " + std::to_string(nLine) + " " + strFun;
	m_ofs.write(strDst.c_str(), strDst.length());

	m_ofs << std::endl;
	m_ofs.flush();
}