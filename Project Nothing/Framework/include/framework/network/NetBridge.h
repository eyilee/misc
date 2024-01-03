#pragma once

using boost::asio::ip::udp;

class IEntity;
class INetProtocol;
class CBitInStream;
class CTcpConnection;
class CUdpConnection;

class CNetBridge : public std::enable_shared_from_this<CNetBridge>
{
public:
	CNetBridge ();
	virtual ~CNetBridge ();

	inline uint32_t GetID () const { return m_nID; }
	inline void SetID (uint32_t _nID) { m_nID = _nID; }

	inline uint32_t GetIP () const { return m_nIP; }

	inline uint32_t GetKey () const { return m_nKey; }
	inline void SetKey (uint32_t _nKey) { m_nKey = _nKey; }

	inline std::shared_ptr<IEntity> GetEntity () const { return m_pkEntity; }
	inline void SetEntity (std::shared_ptr<IEntity> _pkEntity) { m_pkEntity = _pkEntity; }

	void Shutdown ();

	void SetTcpConnection (std::shared_ptr<CTcpConnection> _pkTcpConnection);
	void SetUdpConnection (std::shared_ptr<CUdpConnection> _pkUdpConnection);

	void ResolveUdpInput (uint32_t _nIP, uint32_t _nKey, CBitInStream& _rkInStream);
	void ComposeTcpOutput (std::shared_ptr<INetProtocol> _pkProtocol) const;
	void ComposeUdpOutput (std::shared_ptr<INetProtocol> _pkProtocol) const;

private:
	uint32_t m_nID;
	uint32_t m_nIP;
	uint32_t m_nKey;

	std::shared_ptr<CTcpConnection> m_pkTcpConnection;
	std::shared_ptr<CUdpConnection> m_pkUdpConnection;
	std::shared_ptr<IEntity> m_pkEntity;
};