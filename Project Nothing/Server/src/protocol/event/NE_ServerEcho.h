#pragma once

class NE_ServerEcho : public CNetProtocol<NE_ServerEcho>
{
public:
	NE_ServerEcho ();
	virtual ~NE_ServerEcho ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	std::wstring m_kString;
};