#pragma once

class CRandom
{
public:
	CRandom ();
	virtual ~CRandom ();

	template<typename T>
	static T GetValue () requires std::is_integral<T>::value;

	template<typename T>
	static T GetValue () requires std::is_floating_point<T>::value;

	template<typename T>
	static T GetValue (T _nMin, T _nMax) requires std::is_integral<T>::value;

	template<typename T>
	static T GetValue (T _nMin, T _nMax) requires std::is_floating_point<T>::value;

private:
	static void Init ();

private:
	static bool m_bIsInit;
	static std::mt19937 m_kMT19937;
	static std::mt19937_64 m_kMT19937_64;
};

template<typename T>
inline T CRandom::GetValue () requires std::is_integral<T>::value
{
	if (!m_bIsInit) {
		Init ();
	}

	std::uniform_int_distribution<T> distribution;

	constexpr size_t byteCount = sizeof (T);
	if constexpr (byteCount <= sizeof (uint32_t)) {
		return distribution (m_kMT19937);
	}
	else {
		return distribution (m_kMT19937_64);
	}
}

template<typename T>
inline T CRandom::GetValue () requires std::is_floating_point<T>::value
{
	if (!m_bIsInit) {
		Init ();
	}

	std::uniform_real_distribution<T> distribution;

	constexpr size_t byteCount = sizeof (T);
	if constexpr (byteCount <= sizeof (uint32_t)) {
		return distribution (m_kMT19937);
	}
	else {
		return distribution (m_kMT19937_64);
	}
}

template<typename T>
inline T CRandom::GetValue (T _nMin, T _nMax) requires std::is_integral<T>::value
{
	if (!m_bIsInit) {
		Init ();
	}

	std::uniform_int_distribution<T> distribution (_nMin, _nMax);

	constexpr size_t byteCount = sizeof (T);
	if constexpr (byteCount <= sizeof (uint32_t)) {
		return distribution (m_kMT19937);
	}
	else {
		return distribution (m_kMT19937_64);
	}
}

template<typename T>
inline T CRandom::GetValue (T _nMin, T _nMax) requires std::is_floating_point<T>::value
{
	if (!m_bIsInit) {
		Init ();
	}

	std::uniform_real_distribution<T> distribution (_nMin, _nMax);

	constexpr size_t byteCount = sizeof (T);
	if constexpr (byteCount <= sizeof (uint32_t)) {
		return distribution (m_kMT19937);
	}
	else {
		return distribution (m_kMT19937_64);
	}
}
