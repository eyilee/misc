#pragma once

class IGameObject;

class CGameWorld
{
public:
	CGameWorld ();
	virtual ~CGameWorld ();

	void CreatePlayer (uint32_t _nID);
	void RemovePlayer (uint32_t _nID);

private:
	std::map<uint32_t, std::shared_ptr<IGameObject>> m_kGameObjects;
};