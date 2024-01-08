#pragma once

class IGameLoop
{
public:
	IGameLoop ();
	virtual ~IGameLoop ();

	virtual void Init () = 0;
	virtual void Shutdown () = 0;

	virtual void Update () = 0;
};
