#include "stdafx.h"
#include "framework/network/BitStream.h"

IBitSerializable::IBitSerializable ()
{
}

IBitSerializable::~IBitSerializable ()
{
}

CBitInStream::CBitInStream (const std::vector<uint8_t>& _rkBytes)
	: m_nBitOffset (0)
	, m_kBytes (_rkBytes)
{
}

CBitInStream::CBitInStream (const uint8_t* _pnData, size_t _nSize)
	: m_nBitOffset (0)
	, m_kBytes (_pnData, _pnData + _nSize)
{
}

CBitInStream::~CBitInStream ()
{
}

void CBitInStream::Align ()
{
	size_t offset = m_nBitOffset % 8;
	if (offset > 0) {
		m_nBitOffset += (8 - offset);
	}
}

void CBitInStream::Read (bool& _rbValue)
{
	if (m_nBitOffset + 1 > m_kBytes.size () * 8)
	{
		_rbValue = false;
		return;
	}

	ReadBit (_rbValue);
}

void CBitInStream::Read (std::string& _rkValue)
{
	unsigned short size;
	Read (size);

	std::string value;
	value.reserve (size);

	for (unsigned short i = 0; i < size; i++)
	{
		char c;
		Read (c);
		value.push_back (c);
	}

	_rkValue.swap (value);
}

void CBitInStream::Read (std::wstring& _rkValue)
{
	unsigned short size;
	Read (size);

	std::wstring value;
	value.reserve (size);

	for (unsigned short i = 0; i < size; i++)
	{
		wchar_t c;
		Read (c);
		value.push_back (c);
	}

	_rkValue.swap (value);
}

void CBitInStream::ReadBytes (std::vector<uint8_t>& _rkValue, size_t _nPosition, size_t _nSize)
{
	Align ();

	size_t byteOffset = m_nBitOffset >> 3;
	auto it = m_kBytes.begin () + byteOffset;
	std::copy (it, it + _nSize, _rkValue.begin () + _nPosition);
}

void CBitInStream::ReadValue (void* _pValue, size_t _nByteCount)
{
	if (m_nBitOffset + _nByteCount * 8 > m_kBytes.size () * 8) {
		return;
	}

	uint8_t* byte = reinterpret_cast<uint8_t*> (_pValue);
	for (size_t i = 0; i < _nByteCount; i++)
	{
		ReadByte (*byte);
		byte++;
	}
}

void CBitInStream::ReadByte (uint8_t& _rnByte)
{
	size_t byteOffset = m_nBitOffset >> 3;
	size_t bitOffset = m_nBitOffset & 0x7;

	_rnByte = m_kBytes[byteOffset] << bitOffset;
	if (bitOffset > 0) {
		_rnByte |= m_kBytes[byteOffset + 1] >> (8 - bitOffset);
	}

	m_nBitOffset += 8;
}

void CBitInStream::ReadBit (bool& _rbBit)
{
	size_t byteOffset = m_nBitOffset >> 3;
	size_t bitOffset = m_nBitOffset & 0x7;

	_rbBit = (m_kBytes[byteOffset] & (1 << (7 - bitOffset))) != 0;

	m_nBitOffset += 1;
}

CBitOutStream::CBitOutStream ()
	: m_nBitOffset (0)
{
}

CBitOutStream::~CBitOutStream ()
{
}

const std::vector<uint8_t>& CBitOutStream::GetHeader ()
{
	uint16_t size = static_cast<uint16_t> (m_kBytes.size ());
	size = htons (size);

	m_kHeader.clear ();

	uint8_t* byte = reinterpret_cast<uint8_t*> (&size);
	for (size_t i = 0; i < sizeof (uint16_t); i++)
	{
		m_kHeader.emplace_back (*byte);
		byte++;
	}

	return m_kHeader;
}

void CBitOutStream::Align ()
{
	size_t offset = m_nBitOffset % 8;
	if (offset > 0) {
		m_nBitOffset += (8 - offset);
	}
}

void CBitOutStream::Write (bool _bValue)
{
	WriteBit (_bValue);
}

void CBitOutStream::Write (const std::string& _rkValue)
{
	unsigned short size = (unsigned short)_rkValue.size ();
	Write (size);

	for (unsigned short i = 0; i < size; i++) {
		Write (_rkValue[i]);
	}
}

void CBitOutStream::Write (const std::wstring& _rkValue)
{
	unsigned short size = (unsigned short)_rkValue.size ();
	Write (size);

	for (unsigned short i = 0; i < size; i++) {
		Write (_rkValue[i]);
	}
}

void CBitOutStream::WriteBytes (const std::vector<uint8_t>& _rkValue, size_t _nPosition, size_t _nSize)
{
	Align ();

	m_kBytes.insert (m_kBytes.end (), _rkValue.begin () + _nPosition, _rkValue.begin () + _nPosition + _nSize);
	m_nBitOffset += 8 * _nSize;
}

void CBitOutStream::WriteValue (void* _pValue, size_t _nByteCount)
{
	uint8_t* byte = reinterpret_cast<uint8_t*> (_pValue);
	for (size_t i = 0; i < _nByteCount; i++) {
		WriteByte (*byte);
		byte++;
	}
}

void CBitOutStream::WriteByte (uint8_t& _rnByte)
{
	size_t bitOffset = m_nBitOffset & 0x7;

	if (bitOffset == 0) {
		m_kBytes.emplace_back (_rnByte);
	}
	else
	{
		m_kBytes.back () |= (_rnByte >> bitOffset);
		m_kBytes.emplace_back (_rnByte << (8 - bitOffset));
	}

	m_nBitOffset += 8;
}

void CBitOutStream::WriteBits (uint8_t& _rnByte, size_t _nBitCount)
{
	size_t bitOffset = m_nBitOffset & 0x7;

	if (bitOffset == 0) {
		m_kBytes.emplace_back (_rnByte);
	}
	else
	{
		m_kBytes.back () |= (_rnByte >> bitOffset);

		if (bitOffset + _nBitCount > 8) {
			m_kBytes.emplace_back (_rnByte << (8 - bitOffset));
		}
	}

	m_nBitOffset += _nBitCount;
}

void CBitOutStream::WriteBit (bool& _rbBit)
{
	size_t bitOffset = m_nBitOffset & 0x7;

	uint8_t value = (_rbBit ? 1 : 0) << (7 - bitOffset);

	if (bitOffset == 0) {
		m_kBytes.push_back (value);
	}
	else {
		m_kBytes.back () |= value;
	}

	m_nBitOffset += 1;
}
