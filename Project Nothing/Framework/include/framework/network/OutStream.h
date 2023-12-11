#pragma once

class COutStream
{
public:
	COutStream ();
	virtual ~COutStream ();

	const std::vector<char>& data () const { return m_kData; }

	COutStream& operator << (const bool& _b);
	COutStream& operator << (const char& _c);
	COutStream& operator << (const unsigned char& _uc);
	COutStream& operator << (const short& _s);
	COutStream& operator << (const unsigned short& _us);
	COutStream& operator << (const int& _i);
	COutStream& operator << (const unsigned int& _ui);
	COutStream& operator << (const long& _l);
	COutStream& operator << (const unsigned long& _ul);
	COutStream& operator << (const long long& _ll);
	COutStream& operator << (const unsigned long long& _ull);
	COutStream& operator << (const float& _f);
	COutStream& operator << (const double& _d);
	COutStream& operator << (const std::string& _s);
	COutStream& operator << (const wchar_t& _wc);
	COutStream& operator << (const std::wstring& _s);

private:
	std::vector<char> m_kData;
};