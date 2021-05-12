#include "stdafx.h"

#include "framework/network/OutStream.h"

COutStream::COutStream ()
{
}

COutStream::~COutStream ()
{
}

COutStream& COutStream::operator << (const bool& _b)
{
	m_kData.push_back ((char)_b);
	return *this;
}

COutStream& COutStream::operator << (const char& _c)
{
	m_kData.push_back (_c);
	return *this;
}

COutStream& COutStream::operator << (const unsigned char& _uc)
{
	m_kData.push_back (_uc);
	return *this;
}

COutStream& COutStream::operator << (const short& _s)
{
	short nTemp = htons (_s);
	char* pPostion = (char*)&nTemp;
	m_kData.insert (m_kData.end (), pPostion, pPostion + sizeof (_s));
	return *this;
}

COutStream& COutStream::operator << (const unsigned short& _us)
{
	unsigned short nTemp = htons (_us);
	char* pPostion = (char*)&nTemp;
	m_kData.insert (m_kData.end (), pPostion, pPostion + sizeof (_us));
	return *this;
}

COutStream& COutStream::operator << (const int& _i)
{
	int nTemp = htonl (_i);
	char* pPostion = (char*)&nTemp;
	m_kData.insert (m_kData.end (), pPostion, pPostion + sizeof (_i));
	return *this;
}

COutStream& COutStream::operator << (const unsigned int& _ui)
{
	unsigned int nTemp = htonl (_ui);
	char* pPostion = (char*)&nTemp;
	m_kData.insert (m_kData.end (), pPostion, pPostion + sizeof (_ui));
	return *this;
}

COutStream& COutStream::operator << (const long& _l)
{
	long nTemp = htonl (_l);
	char* pPostion = (char*)&nTemp;
	m_kData.insert (m_kData.end (), pPostion, pPostion + sizeof (_l));
	return *this;
}

COutStream& COutStream::operator << (const unsigned long& _ul)
{
	unsigned long nTemp = htonl (_ul);
	char* pPostion = (char*)&nTemp;
	m_kData.insert (m_kData.end (), pPostion, pPostion + sizeof (_ul));
	return *this;
}

COutStream& COutStream::operator << (const long long& _ll)
{
	unsigned int* pTemp = (unsigned int*)(&_ll);
	pTemp[0] = htonl (pTemp[0]);
	pTemp[1] = htonl (pTemp[1]);
	char* pPostion = (char*)pTemp;
	m_kData.insert (m_kData.end (), pPostion, pPostion + sizeof (_ll));
	return *this;
}

COutStream& COutStream::operator << (const unsigned long long& _ull)
{
	unsigned int* pTemp = (unsigned int*)(&_ull);
	pTemp[0] = htonl (pTemp[0]);
	pTemp[1] = htonl (pTemp[1]);
	char* pPostion = (char*)pTemp;
	m_kData.insert (m_kData.end (), pPostion, pPostion + sizeof (_ull));
	return *this;
}

COutStream& COutStream::operator << (const float& _f)
{
	unsigned int* pTemp = (unsigned int*)(&_f);
	pTemp[0] = htonl (pTemp[0]);
	char* pPostion = (char*)pTemp;
	m_kData.insert (m_kData.end (), pPostion, pPostion + sizeof (_f));
	return *this;
}

COutStream& COutStream::operator << (const double& _d)
{
	unsigned int* pTemp = (unsigned int*)(&_d);
	pTemp[0] = htonl (pTemp[0]);
	pTemp[1] = htonl (pTemp[1]);
	char* pPostion = (char*)pTemp;
	m_kData.insert (m_kData.end (), pPostion, pPostion + sizeof (_d));
	return *this;
}

COutStream& COutStream::operator << (const std::string& _s)
{
	size_t nSize = _s.size ();
	*this << (unsigned int)nSize;
	for (size_t i = 0; i < nSize; i++) {
		*this << _s[i];
	}
	return *this;
}
