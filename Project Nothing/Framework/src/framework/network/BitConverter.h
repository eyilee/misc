#pragma once

class BitConverter
{
public:
	BitConverter ();
	virtual ~BitConverter ();

	static uint16_t ToUInt16 (const void* _pValue);
	static uint32_t ToUInt32 (const void* _pValue);
	static uint64_t ToUInt64 (const void* _pValue);
};
