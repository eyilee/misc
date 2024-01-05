#pragma once

class ISerializable;

class CBitInStream
{
public:
	CBitInStream (const std::vector<uint8_t>& _rkBytes);
	CBitInStream (const uint8_t* _pnData, size_t _nSize);
	virtual ~CBitInStream ();

	void Read (bool& _rbValue);

	template<typename T> requires std::is_arithmetic_v<T>
	void Read (T& _rnValue);

	template<typename T> requires std::is_base_of_v<ISerializable, T>
	void Read (T& _rkValue);

	void Read (std::string& _rkValue);
	void Read (std::wstring& _rkValue);

private:
	void ReadValue (void* _pValue, size_t _nByteCount);
	void ReadByte (uint8_t& _rnByte);
	void ReadBit (bool& _rbBit);

public:
	size_t m_nBitOffset;
	std::vector<uint8_t> m_kBytes;
};

template<typename T> requires std::is_arithmetic_v<T>
inline void CBitInStream::Read (T& _rnValue)
{
	constexpr size_t byteCount = sizeof (T);

	if constexpr (byteCount == sizeof (uint8_t))
	{
		uint8_t* value = reinterpret_cast<uint8_t*> (&_rnValue);
		ReadValue (value, byteCount);
	}
	else if constexpr (byteCount == sizeof (uint16_t))
	{
		uint16_t* value = reinterpret_cast<uint16_t*> (&_rnValue);
		ReadValue (value, byteCount);
		*value = ntohs (*value);
	}
	else if constexpr (byteCount == sizeof (uint32_t))
	{
		uint32_t* value = reinterpret_cast<uint32_t*> (&_rnValue);
		ReadValue (value, byteCount);
		*value = ntohl (*value);
	}
	else if constexpr (byteCount == sizeof (uint64_t))
	{
		uint64_t* value = reinterpret_cast<uint64_t*> (&_rnValue);
		ReadValue (value, byteCount);
		*value = ntohll (*value);
	}
}

template<typename T> requires std::is_base_of_v<ISerializable, T>
inline void CBitInStream::Read (T& _rkValue)
{
	_rkValue.Deserialize (*this);
}

class CBitOutStream
{
public:
	CBitOutStream ();
	virtual ~CBitOutStream ();

	const std::vector<uint8_t>& GetHeader ();
	const std::vector<uint8_t>& GetBytes () const { return m_kBytes; }
	size_t GetSize () const { return m_kBytes.size (); }

	void Write (bool _bValue);

	template<typename T> requires std::is_arithmetic_v<T>
	void Write (T _nValue);

	template<typename T> requires std::is_base_of_v<ISerializable, T>
	void Write (T& _rkValue);

	void Write (const std::string& _rkValue);
	void Write (const std::wstring& _rkValue);

private:
	void WriteValue (void* _pValue, size_t _nByteCount);
	void WriteByte (uint8_t& _rnByte);
	void WriteBit (bool& _rbBit);

public:
	size_t m_nBitOffset;
	std::vector<uint8_t> m_kHeader;
	std::vector<uint8_t> m_kBytes;
};

template<typename T> requires std::is_arithmetic_v<T>
inline void CBitOutStream::Write (T _nValue)
{
	constexpr size_t byteCount = sizeof (T);

	if constexpr (byteCount == sizeof (uint8_t))
	{
		uint8_t* value = reinterpret_cast<uint8_t*> (&_nValue);
		WriteValue (value, byteCount);
	}
	else if constexpr (byteCount == sizeof (uint16_t))
	{
		uint16_t* value = reinterpret_cast<uint16_t*> (&_nValue);
		*value = htons (*value);
		WriteValue (value, byteCount);
	}
	else if constexpr (byteCount == sizeof (uint32_t))
	{
		uint32_t* value = reinterpret_cast<uint32_t*> (&_nValue);
		*value = htonl (*value);
		WriteValue (value, byteCount);
	}
	else if constexpr (byteCount == sizeof (uint64_t))
	{
		uint64_t* value = reinterpret_cast<uint64_t*> (&_nValue);
		*value = htonll (*value);
		WriteValue (value, byteCount);
	}
}

template<typename T> requires std::is_base_of_v<ISerializable, T>
inline void CBitOutStream::Write (T& _rkValue)
{
	_rkValue.Serialize (*this);
}
