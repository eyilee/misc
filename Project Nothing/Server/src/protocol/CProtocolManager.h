#pragma once

class CProtocolManager
{
public:
	CProtocolManager ();
	virtual ~CProtocolManager ();

	void init ();

	void register_protocol ();
};