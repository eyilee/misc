#pragma once

class NE_ServerEcho : public INetEvent<NE_ServerEcho>
{
public:
	NE_ServerEcho ();
	virtual ~NE_ServerEcho ();

	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	unsigned short m_nSequence;
	long long m_nSendTime;
};
