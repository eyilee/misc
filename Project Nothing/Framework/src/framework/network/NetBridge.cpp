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

CNetBridge::CNetBridge ()
	: m_pkTcpSession (nullptr)
	, m_nUdpIP (0)
	, m_nUdpKey (0)
	, m_pkEntity (nullptr)
{
}

CNetBridge::~CNetBridge ()
{
}

void CNetBridge::SetUdpEndPoint (const short& _nPort)
{
	const auto& address = m_pkTcpSession->m_kSocket.remote_endpoint ().address ();
	m_kUdpEndPoint = udp::endpoint (address, _nPort);
	m_nUdpIP = address.to_v4 ().to_uint ();
	m_nUdpKey = CRandom::GetValue<uint32_t> ();
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
	CSessionManager::Send (outStream, m_kUdpEndPoint);
}
