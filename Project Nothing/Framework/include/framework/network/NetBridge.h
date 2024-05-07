#pragma once

class INetEntity;
class INetProtocol;
class CTcpConnection;

class CNetBridge
{
public:
	CNetBridge (uint32_t _nID);
	virtual ~CNetBridge ();

	inline uint32_t GetID () const { return m_nID; }

	inline std::shared_ptr<CTcpConnection> GetConnection () const { return m_pkConnection; }
	inline void SetConnection (std::shared_ptr<CTcpConnection> _pkConnection) { m_pkConnection = _pkConnection; }

	template<typename T = INetEntity> requires std::is_base_of_v<INetEntity, T>
	std::shared_ptr<T> GetNetEntity ();

	inline void SetNetEntity (std::shared_ptr<INetEntity> _pkNetEntity) { m_pkNetEntity = _pkNetEntity; }

	void Init (std::shared_ptr<CTcpConnection> _pkTcpConnection);
	void Shutdown ();
	void OnDisconnect ();

	void ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol);

private:
	uint32_t m_nID;
	std::shared_ptr<CTcpConnection> m_pkConnection;
	std::shared_ptr<INetEntity> m_pkNetEntity;
};

template<typename T> requires std::is_base_of_v<INetEntity, T>
inline std::shared_ptr<T> CNetBridge::GetNetEntity ()
{
	return std::static_pointer_cast<T> (m_pkNetEntity);
}
