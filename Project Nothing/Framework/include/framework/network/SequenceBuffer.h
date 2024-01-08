#pragma once

template<typename TPacket, uint32_t BUFFER_SIZE>
class SequenceBuffer
{
public:
	SequenceBuffer ();
	virtual ~SequenceBuffer ();

	constexpr uint32_t GetSize () { return BUFFER_SIZE; }

	TPacket& Insert (uint32_t _nSequence);
	void Remove (uint32_t _nSequence);
	void Clear ();

	TPacket* TryGet (uint32_t _nSequence);
	TPacket* TryGetByIndex (uint32_t _nIndex, uint32_t& _rnSequence);

	bool IsExist (uint32_t _nSequence);

private:
	std::array<uint32_t, BUFFER_SIZE> m_kIndexes;
	std::array<TPacket, BUFFER_SIZE> m_kPackets;
};

template<typename TPacket, uint32_t BUFFER_SIZE>
inline SequenceBuffer<TPacket, BUFFER_SIZE>::SequenceBuffer ()
{
	Clear ();
}

template<typename TPacket, uint32_t BUFFER_SIZE>
inline SequenceBuffer<TPacket, BUFFER_SIZE>::~SequenceBuffer ()
{
}

template<typename TPacket, uint32_t BUFFER_SIZE>
inline TPacket& SequenceBuffer<TPacket, BUFFER_SIZE>::Insert (uint32_t _nSequence)
{
	uint32_t index = _nSequence % BUFFER_SIZE;
	m_kIndexes[index] = _nSequence;
	return m_kPackets[index];
}

template<typename TPacket, uint32_t BUFFER_SIZE>
inline void SequenceBuffer<TPacket, BUFFER_SIZE>::Remove (uint32_t _nSequence)
{
	uint32_t index = _nSequence % BUFFER_SIZE;
	if (m_kIndexes[index] == _nSequence) {
		m_kIndexes[index] = BUFFER_SIZE;
	}
}

template<typename TPacket, uint32_t BUFFER_SIZE>
inline void SequenceBuffer<TPacket, BUFFER_SIZE>::Clear ()
{
	m_kIndexes.fill (BUFFER_SIZE);
}

template<typename TPacket, uint32_t BUFFER_SIZE>
inline TPacket* SequenceBuffer<TPacket, BUFFER_SIZE>::TryGet (uint32_t _nSequence)
{
	uint32_t index = _nSequence % BUFFER_SIZE;
	if (m_kIndexes[index] == _nSequence) {
		return &m_kPackets[index];
	}
	else {
		return nullptr;
	}
}

template<typename TPacket, uint32_t BUFFER_SIZE>
inline TPacket* SequenceBuffer<TPacket, BUFFER_SIZE>::TryGetByIndex (uint32_t _nIndex, uint32_t& _rnSequence)
{
	if (m_kIndexes[_nIndex] != BUFFER_SIZE) {
		_rnSequence = m_kIndexes[_nIndex];
		return m_kPackets[_nIndex];
	}
	else {
		return nullptr;
	}
}

template<typename TPacket, uint32_t BUFFER_SIZE>
inline bool SequenceBuffer<TPacket, BUFFER_SIZE>::IsExist (uint32_t _nSequence)
{
	uint32_t index = _nSequence % BUFFER_SIZE;
	return m_kIndexes[index] == _nSequence;
}
