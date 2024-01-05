#pragma once

template<typename TPacketInfo, uint16_t BUFFER_SIZE>
class SequenceBuffer
{
public:
	SequenceBuffer ()
	{
	}

	virtual ~SequenceBuffer ()
	{
	}

	constexpr uint16_t GetSize () { return BUFFER_SIZE; }

	TPacketInfo& Insert (uint16_t _nSequence)
	{
		uint16_t index = _nSequence % BUFFER_SIZE;
		m_kIndexes[index] = _nSequence;
		return m_kPacketInfos[index];
	}

	void Remove (uint16_t _nSequence)
	{
		uint16_t index = _nSequence % BUFFER_SIZE;
		if (m_kIndexes[index] = _nSequence) {
			m_kIndexes[index] = BUFFER_SIZE;
		}
	}

	TPacketInfo* TryGet (uint16_t _nSequence)
	{
		uint16_t index = _nSequence % BUFFER_SIZE;
		if (m_kIndexes[index] == _nSequence) {
			return &m_kPacketInfos[index];
		}
		else {
			return nullptr;
		}
	}

	TPacketInfo* TryGetByIndex (uint16_t _nIndex, uint16_t& _rnSequence)
	{
		if (m_kIndexes[_nIndex] != BUFFER_SIZE) {
			_rnSequence = m_kIndexes[_nIndex];
			return m_kPacketInfos[_nIndex];
		}
		else {
			return nullptr;
		}
	}

	bool IsExist (uint16_t _nSequence)
	{
		uint16_t index = _nSequence % BUFFER_SIZE;
		return m_kIndexes[index] == _nSequence;
	}

private:
	uint16_t m_kIndexes[BUFFER_SIZE];
	TPacketInfo m_kPacketInfos[BUFFER_SIZE];
};
