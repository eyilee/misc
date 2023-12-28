#pragma once

class CRandom
{
public:
	CRandom ();
	virtual ~CRandom ();

	template<typename T> requires std::is_integral<T>::value
		static T GetValue ();

	template<typename T> requires std::is_floating_point<T>::value
		static T GetValue ();

	template<typename T> requires std::is_integral<T>::value
		static T GetValue (T _nMin, T _nMax);

	template<typename T> requires std::is_floating_point<T>::value
		static T GetValue (T _nMin, T _nMax);

private:
	static void Init ();

private:
	static bool m_bIsInit;
	static std::mt19937 m_kMT19937;
	static std::mt19937_64 m_kMT19937_64;
};

template<typename T> requires std::is_integral<T>::value
inline T CRandom::GetValue ()
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

template<typename T> requires std::is_floating_point<T>::value
inline T CRandom::GetValue ()
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

template<typename T> requires std::is_integral<T>::value
inline T CRandom::GetValue (T _nMin, T _nMax)
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

template<typename T> requires std::is_floating_point<T>::value
inline T CRandom::GetValue (T _nMin, T _nMax)
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
