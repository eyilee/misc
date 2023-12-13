#include "stdafx.h"
#include "framework/Random.h"
#include "framework/network/BitStream.h"
#include "framework/network/Entity.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/TcpSession.h"
#include "framework/network/UdpSession.h"
#include "framework/manager/ProtocolManager.h"
#include "framework/manager/SessionManager.h"
#include "framework/network/NetBridge.h"

CNetBridge::CNetBridge (std::shared_ptr<CTcpSession> _pkTcpSession, const uint32_t& _rnIP)
	: m_pkTcpSession (_pkTcpSession)
	, m_nIP (_rnIP)
	, m_nUdpKey (0)
	, m_pkEntity (nullptr)
{
}

CNetBridge::~CNetBridge ()
{
}

void CNetBridge::SetUdpEndPoint ()
{
	std::shared_ptr<CUdpSession> udpSession = CSessionManager::GetUdpSession ();
	if (udpSession == nullptr) {
		return;
	}

	m_kUdpEndPoint = udpSession->m_kEndpoint;
}

void CNetBridge::ResolveInput (CBitInStream& _rkInStream)
{
	unsigned short protocolID;
	_rkInStream.Read (protocolID);

	std::shared_ptr<INetProtocol> protocol = CProtocolManager::GenerateProtocol (protocolID);
	if (protocol != nullptr)
	{
		protocol->SetNetBridge (shared_from_this ());
		protocol->Deserialize (_rkInStream);
		protocol->Excute ();
	}
}

void CNetBridge::ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol)
{
	CBitOutStream outStream;
	_pkProtocol->OnSerialize (outStream);
	m_pkTcpSession->OnWrite (outStream);
}

void CNetBridge::UdpOutput (std::shared_ptr<INetProtocol> _pkProtocol)
{
	CBitOutStream outStream;
	outStream.Write (m_nUdpKey);
	_pkProtocol->OnSerialize (outStream);

	std::shared_ptr<CUdpSession> udpSession = CSessionManager::GetUdpSession ();
	if (udpSession != nullptr) {
		udpSession->OnSend (outStream, m_kUdpEndPoint);
	}
}
