#pragma once

template<typename TPacketInfo, uint32_t BUFFER_SIZE>
class SequenceBuffer
{
public:
	SequenceBuffer ();
	virtual ~SequenceBuffer ();

	constexpr uint32_t GetSize () { return BUFFER_SIZE; }

	TPacketInfo& Insert (uint32_t _nSequence);
	void Remove (uint32_t _nSequence);

	TPacketInfo* TryGet (uint32_t _nSequence);
	TPacketInfo* TryGetByIndex (uint32_t _nIndex, uint32_t& _rnSequence);

	bool IsExist (uint32_t _nSequence);

private:
	uint32_t m_kIndexes[BUFFER_SIZE];
	TPacketInfo m_kPacketInfos[BUFFER_SIZE];
};

template<typename TPacketInfo, uint32_t BUFFER_SIZE>
inline SequenceBuffer<TPacketInfo, BUFFER_SIZE>::SequenceBuffer ()
{
}

template<typename TPacketInfo, uint32_t BUFFER_SIZE>
inline SequenceBuffer<TPacketInfo, BUFFER_SIZE>::~SequenceBuffer ()
{
}

template<typename TPacketInfo, uint32_t BUFFER_SIZE>
inline TPacketInfo& SequenceBuffer<TPacketInfo, BUFFER_SIZE>::Insert (uint32_t _nSequence)
{
	uint32_t index = _nSequence % BUFFER_SIZE;
	m_kIndexes[index] = _nSequence;
	return m_kPacketInfos[index];
}

template<typename TPacketInfo, uint32_t BUFFER_SIZE>
inline void SequenceBuffer<TPacketInfo, BUFFER_SIZE>::Remove (uint32_t _nSequence)
{
	uint32_t index = _nSequence % BUFFER_SIZE;
	if (m_kIndexes[index] == _nSequence) {
		m_kIndexes[index] = BUFFER_SIZE;
	}
}

template<typename TPacketInfo, uint32_t BUFFER_SIZE>
inline TPacketInfo* SequenceBuffer<TPacketInfo, BUFFER_SIZE>::TryGet (uint32_t _nSequence)
{
	uint32_t index = _nSequence % BUFFER_SIZE;
	if (m_kIndexes[index] == _nSequence) {
		return &m_kPacketInfos[index];
	}
	else {
		return nullptr;
	}
}

template<typename TPacketInfo, uint32_t BUFFER_SIZE>
inline TPacketInfo* SequenceBuffer<TPacketInfo, BUFFER_SIZE>::TryGetByIndex (uint32_t _nIndex, uint32_t& _rnSequence)
{
	if (m_kIndexes[_nIndex] != BUFFER_SIZE) {
		_rnSequence = m_kIndexes[_nIndex];
		return m_kPacketInfos[_nIndex];
	}
	else {
		return nullptr;
	}
}

template<typename TPacketInfo, uint32_t BUFFER_SIZE>
inline bool SequenceBuffer<TPacketInfo, BUFFER_SIZE>::IsExist (uint32_t _nSequence)
{
	uint32_t index = _nSequence % BUFFER_SIZE;
	return m_kIndexes[index] == _nSequence;
}
