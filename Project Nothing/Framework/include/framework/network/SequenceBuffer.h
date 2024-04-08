#pragma once

template<typename T, size_t BUFFER_SIZE>
class SequenceBuffer
{
public:
	SequenceBuffer ();
	virtual ~SequenceBuffer ();

	constexpr size_t GetSize () { return BUFFER_SIZE; }

	T& Insert (int _nSequence);
	void Remove (int _nSequence);
	void Clear ();

	T* TryGet (int _nSequence);
	T* TryGetByIndex (int _nIndex, int _nSequence);

	bool IsExist (int _nSequence);

private:
	std::array<int, BUFFER_SIZE> m_kIndexes;
	std::array<T, BUFFER_SIZE> m_kPackets;
};

template<typename T, size_t BUFFER_SIZE>
inline SequenceBuffer<T, BUFFER_SIZE>::SequenceBuffer ()
{
	Clear ();
}

template<typename T, size_t BUFFER_SIZE>
inline SequenceBuffer<T, BUFFER_SIZE>::~SequenceBuffer ()
{
}

template<typename T, size_t BUFFER_SIZE>
inline T& SequenceBuffer<T, BUFFER_SIZE>::Insert (int _nSequence)
{
	int index = _nSequence % BUFFER_SIZE;
	m_kIndexes[index] = _nSequence;
	return m_kPackets[index];
}

template<typename T, size_t BUFFER_SIZE>
inline void SequenceBuffer<T, BUFFER_SIZE>::Remove (int _nSequence)
{
	int index = _nSequence % BUFFER_SIZE;
	if (m_kIndexes[index] == _nSequence) {
		m_kIndexes[index] = BUFFER_SIZE;
	}
}

template<typename T, size_t BUFFER_SIZE>
inline void SequenceBuffer<T, BUFFER_SIZE>::Clear ()
{
	m_kIndexes.fill (BUFFER_SIZE);
}

template<typename T, size_t BUFFER_SIZE>
inline T* SequenceBuffer<T, BUFFER_SIZE>::TryGet (int _nSequence)
{
	int index = _nSequence % BUFFER_SIZE;
	if (m_kIndexes[index] == _nSequence) {
		return &m_kPackets[index];
	}
	else {
		return nullptr;
	}
}

template<typename T, size_t BUFFER_SIZE>
inline T* SequenceBuffer<T, BUFFER_SIZE>::TryGetByIndex (int _nIndex, int _nSequence)
{
	if (m_kIndexes[_nIndex] != BUFFER_SIZE) {
		_nSequence = m_kIndexes[_nIndex];
		return m_kPackets[_nIndex];
	}
	else {
		return nullptr;
	}
}

template<typename T, size_t BUFFER_SIZE>
inline bool SequenceBuffer<T, BUFFER_SIZE>::IsExist (int _nSequence)
{
	int index = _nSequence % BUFFER_SIZE;
	return m_kIndexes[index] == _nSequence;
}
