#include "stdafx.h"
#include "framework/network/BitConverter.h"

BitConverter::BitConverter ()
{
}

BitConverter::~BitConverter ()
{
}

uint16_t BitConverter::ToUInt16 (const void* _pValue)
{
	return *reinterpret_cast<const uint16_t*> (_pValue);
}

uint32_t BitConverter::ToUInt32 (const void* _pValue)
{
	return *reinterpret_cast<const uint32_t*> (_pValue);
}

uint64_t BitConverter::ToUInt64 (const void* _pValue)
{
	return *reinterpret_cast<const uint64_t*> (_pValue);
}
