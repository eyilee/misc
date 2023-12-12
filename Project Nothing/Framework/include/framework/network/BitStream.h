#pragma once

class ISerializable;

class CBitInStream
{
public:
	CBitInStream (const std::vector<uint8_t>& _rkBytes);
	CBitInStream (const uint8_t* _pnData, size_t _nSize);
	virtual ~CBitInStream ();

	void Read (bool& _rbValue);

	template<typename T>
	void Read (T& _rnValue) requires std::is_arithmetic<T>::value;

	template<typename T>
	void Read (T& _rkValue) requires std::is_base_of<ISerializable, T>::value;

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

template<typename T>
inline void CBitInStream::Read (T& _rnValue) requires std::is_arithmetic<T>::value
{
	constexpr size_t byteCount = sizeof (T);

	if constexpr (byteCount == sizeof (uint8_t)) {
		uint8_t* value = reinterpret_cast<uint8_t*> (&_rnValue);
		ReadValue (value, byteCount);
	}
	else if constexpr (byteCount == sizeof (uint16_t)) {
		uint16_t* value = reinterpret_cast<uint16_t*> (&_rnValue);
		ReadValue (value, byteCount);
		*value = ntohs (*value);
	}
	else if constexpr (byteCount == sizeof (uint32_t)) {
		uint32_t* value = reinterpret_cast<uint32_t*> (&_rnValue);
		ReadValue (value, byteCount);
		*value = ntohl (*value);
	}
	else if constexpr (byteCount == sizeof (uint64_t)) {
		uint64_t* value = reinterpret_cast<uint64_t*> (&_rnValue);
		ReadValue (value, byteCount);
		*value = ntohll (*value);
	}
}

template<typename T>
inline void CBitInStream::Read (T& _rkValue) requires std::is_base_of<ISerializable, T>::value
{
	_rkValue.Deserialize (*this);
}

class CBitOutStream
{
public:
	CBitOutStream ();
	virtual ~CBitOutStream ();

	const std::vector<uint8_t>& GetBytes () const { return m_kBytes; }

	void Write (bool _bValue);

	template<typename T>
	void Write (T _nValue) requires std::is_arithmetic<T>::value;

	template<typename T>
	void Write (const T& _rkValue) requires std::is_base_of<ISerializable, T>::value;

	void Write (const std::string& _rkValue);
	void Write (const std::wstring& _rkValue);

private:
	void WriteValue (void* _pValue, size_t _nByteCount);
	void WriteByte (uint8_t& _rnByte);
	void WriteBit (bool& _rbBit);

public:
	size_t m_nBitOffset;
	std::vector<uint8_t> m_kBytes;
};

template<typename T>
inline void CBitOutStream::Write (T _nValue) requires std::is_arithmetic<T>::value
{
	constexpr size_t byteCount = sizeof (T);

	if constexpr (byteCount == sizeof (uint8_t)) {
		uint8_t* value = reinterpret_cast<uint8_t*> (&_nValue);
		WriteValue (value, byteCount);
	}
	else if constexpr (byteCount == sizeof (uint16_t)) {
		uint16_t* value = reinterpret_cast<uint16_t*> (&_nValue);
		*value = htons (*value);
		WriteValue (value, byteCount);
	}
	else if constexpr (byteCount == sizeof (uint32_t)) {
		uint32_t* value = reinterpret_cast<uint32_t*> (&_nValue);
		*value = htonl (*value);
		WriteValue (value, byteCount);
	}
	else if constexpr (byteCount == sizeof (uint64_t)) {
		uint64_t* value = reinterpret_cast<uint64_t*> (&_nValue);
		*value = htonll (*value);
		WriteValue (value, byteCount);
	}
}

template<typename T>
inline void CBitOutStream::Write (const T& _rkValue) requires std::is_base_of<ISerializable, T>::value
{
	_rkValue.Serialize (*this);
}