#pragma once

class NC_ClientEchoResult : public INetCommand<NC_ClientEchoResult>
{
public:
	NC_ClientEchoResult ();
	NC_ClientEchoResult (unsigned short _nSequence, long long _nSendTime);
	virtual ~NC_ClientEchoResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;

private:
	unsigned short m_nSequence;
	long long m_nSendTime;
};