#include "stdafx.h"

#include "CInStream.h"

CInStream::CInStream (const std::vector<char>& _kData)
	:m_kData (_kData)
{
}

CInStream::~CInStream ()
{
}

CInStream& CInStream::operator >> (bool& _b)
{
	_b = *(bool*)(&*m_kData.begin ());
	m_kData.erase (m_kData.begin ());
	return *this;
}

CInStream& CInStream::operator >> (char& _c)
{
	_c = *(char*)(&*m_kData.begin ());
	m_kData.erase (m_kData.begin ());
	return *this;
}

CInStream& CInStream::operator >> (unsigned char& _uc)
{
	_uc = *(unsigned char*)(&*m_kData.begin ());
	m_kData.erase (m_kData.begin ());
	return *this;
}

CInStream& CInStream::operator >> (short& _s)
{
	_s = *(short*)(&*m_kData.begin ());
	_s = ntohs (_s);
	m_kData.erase (m_kData.begin (), m_kData.begin () + sizeof (_s));
	return *this;
}

CInStream& CInStream::operator >> (unsigned short& _us)
{
	_us = *(unsigned short*)(&*m_kData.begin ());
	_us = ntohs (_us);
	m_kData.erase (m_kData.begin (), m_kData.begin () + sizeof (_us));
	return *this;
}

CInStream& CInStream::operator >> (int& _i)
{
	_i = *(int*)(&*m_kData.begin ());
	_i = ntohl (_i);
	m_kData.erase (m_kData.begin (), m_kData.begin () + sizeof (_i));
	return *this;
}

CInStream& CInStream::operator >> (unsigned int& _ui)
{
	_ui = *(unsigned int*)(&*m_kData.begin ());
	_ui = ntohl (_ui);
	m_kData.erase (m_kData.begin (), m_kData.begin () + sizeof (_ui));
	return *this;
}

CInStream& CInStream::operator >> (long& _l)
{
	_l = *(long*)(&*m_kData.begin ());
	_l = ntohl (_l);
	m_kData.erase (m_kData.begin (), m_kData.begin () + sizeof (_l));
	return *this;
}

CInStream& CInStream::operator >> (unsigned long& _ul)
{
	_ul = *(unsigned long*)(&*m_kData.begin ());
	_ul = ntohl (_ul);
	m_kData.erase (m_kData.begin (), m_kData.begin () + sizeof (_ul));
	return *this;
}

CInStream& CInStream::operator >> (long long& _ll)
{
	unsigned int* pTemp = (unsigned int*)(&*m_kData.begin ());
	pTemp[0] = ntohl (pTemp[0]);
	pTemp[1] = ntohl (pTemp[1]);
	_ll = *(long long*)(&*m_kData.begin ());
	m_kData.erase (m_kData.begin (), m_kData.begin () + sizeof (_ll));
	return *this;
}

CInStream& CInStream::operator >> (unsigned long long& _ull)
{
	unsigned int* pTemp = (unsigned int*)(&*m_kData.begin ());
	pTemp[0] = ntohl (pTemp[0]);
	pTemp[1] = ntohl (pTemp[1]);
	_ull = *(unsigned long long*)(&*m_kData.begin ());
	m_kData.erase (m_kData.begin (), m_kData.begin () + sizeof (_ull));
	return *this;
}

CInStream& CInStream::operator >> (float& _f)
{
	unsigned int* pTemp = (unsigned int*)(&*m_kData.begin ());
	pTemp[0] = ntohl (pTemp[0]);
	_f = *(float*)(&*m_kData.begin ());
	m_kData.erase (m_kData.begin (), m_kData.begin () + sizeof (_f));
	return *this;
}

CInStream& CInStream::operator >> (double& _d)
{
	unsigned int* pTemp = (unsigned int*)(&*m_kData.begin ());
	pTemp[0] = ntohl (pTemp[0]);
	pTemp[1] = ntohl (pTemp[1]);
	_d = *(double*)(&*m_kData.begin ());
	m_kData.erase (m_kData.begin (), m_kData.begin () + sizeof (_d));
	return *this;
}
