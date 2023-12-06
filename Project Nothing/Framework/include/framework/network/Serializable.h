#pragma once

class CBitInStream;
class CBitOutStream;

class ISerializable
{
public:
	ISerializable ();
	virtual ~ISerializable ();

	virtual void Serialize (CBitOutStream& _rkOutStream) = 0;
	virtual void Deserialize (CBitInStream& _rkInStream) = 0;
};