#include "stdafx.h"
#include "framework/network/Serializable.h"

#include "framework/network/BitStream.h"

CBitInStream::CBitInStream (const std::vector<uint8_t>& _rkBytes)
	: m_nBitOffset {0}
	, m_kBytes {_rkBytes}
{
}

CBitInStream::~CBitInStream ()
{
}

void CBitInStream::Read (bool& _rbValue)
{
	if (m_nBitOffset + 1 > m_kBytes.size () * 8) {
		return;
	}

	ReadBit (_rbValue);
}

void CBitInStream::ReadValue (void* _pValue, size_t _nByteCount)
{
	if (m_nBitOffset + _nByteCount * 8 > m_kBytes.size () * 8) {
		return;
	}

	uint8_t* byte = reinterpret_cast<uint8_t*> (_pValue);
	for (size_t i = 0; i < _nByteCount; i++) {
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
	: m_nBitOffset {0}
{
}

CBitOutStream::~CBitOutStream ()
{
}

void CBitOutStream::Write (bool _bValue)
{
	WriteBit (_bValue);
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
	else {
		m_kBytes.back () |= (_rnByte >> bitOffset);
		m_kBytes.emplace_back (_rnByte << (8 - bitOffset));
	}

	m_nBitOffset += 8;
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
