#pragma once

class CInStream
{
public:
	CInStream (const std::vector<char>& _kData);
	CInStream (const char* _pData, size_t _nSize);
	virtual ~CInStream ();

	const std::vector<char>& data () const { return m_kData; }

	CInStream& operator >> (bool& _b);
	CInStream& operator >> (char& _c);
	CInStream& operator >> (unsigned char& _uc);
	CInStream& operator >> (short& _s);
	CInStream& operator >> (unsigned short& _us);
	CInStream& operator >> (int& _i);
	CInStream& operator >> (unsigned int& _ui);
	CInStream& operator >> (long& _l);
	CInStream& operator >> (unsigned long& _ul);
	CInStream& operator >> (long long& _ll);
	CInStream& operator >> (unsigned long long& _ull);
	CInStream& operator >> (float& _f);
	CInStream& operator >> (double& _d);

private:
	std::vector<char> m_kData;
};