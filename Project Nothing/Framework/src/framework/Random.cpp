#include "stdafx.h"
#include "framework/Random.h"

bool CRandom::m_bIsInit = false;
std::mt19937 CRandom::m_kMT19937;
std::mt19937_64 CRandom::m_kMT19937_64;

CRandom::CRandom ()
{
}

CRandom::~CRandom ()
{
}

void CRandom::Init ()
{
	if (m_bIsInit) {
		return;
	}

	std::random_device randomDevice;
	m_kMT19937 = std::mt19937 (randomDevice ());
	m_kMT19937_64 = std::mt19937_64 (randomDevice ());
	m_bIsInit = true;
}
