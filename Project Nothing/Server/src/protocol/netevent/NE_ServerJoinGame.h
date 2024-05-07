#pragma once

class NE_ServerJoinGame : public INetEvent<NE_ServerJoinGame>
{
public:
	NE_ServerJoinGame ();
	virtual ~NE_ServerJoinGame ();

	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	uint32_t m_nGameID;
	unsigned short m_nPort;
};
