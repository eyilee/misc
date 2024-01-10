#pragma once

class CServerGame;
class CPlayerEntity;

class CServerConnection : public CUdpConnection
{
public:
	CServerConnection (std::shared_ptr<CUdpSession> _pkUdpSession, std::shared_ptr<CServerGame> _pkServerGame, std::shared_ptr<CPlayerEntity> _pkPlayerEntity);
	virtual ~CServerConnection ();

	uint32_t GetPlayerID ();

	inline std::shared_ptr<CPlayerEntity> GetPlayerEntity () const { return m_pkPlayerEntity; };
	inline void SetPlayerEntity (std::shared_ptr<CPlayerEntity> _pkPlayerEntity) { m_pkPlayerEntity = _pkPlayerEntity; };

protected:
	virtual void ResolvePackage (CBitInStream& _rkInStream) override;
	virtual void OnPacketAcked (uint32_t _nSequence, SOutPacket& _rkOutPacket) override;

protected:
	std::shared_ptr<CServerGame> m_pkServerGame;
	std::shared_ptr<CPlayerEntity> m_pkPlayerEntity;
};