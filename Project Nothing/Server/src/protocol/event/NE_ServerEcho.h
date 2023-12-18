#pragma once

class NE_ServerEcho : public CNetEvent<NE_ServerEcho>
{
public:
	NE_ServerEcho ();
	virtual ~NE_ServerEcho ();

	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	std::wstring m_kString;
};