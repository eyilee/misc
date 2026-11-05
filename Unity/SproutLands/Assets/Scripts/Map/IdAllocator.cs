using System.Collections.Generic;

public class IdAllocator
{
    private int _nextId = 1;
    private readonly Queue<int> _freeIds = new ();

    public int Allocate ()
    {
        if (_freeIds.Count > 0)
        {
            return _freeIds.Dequeue ();
        }

        return _nextId++;
    }

    public void Release (int id)
    {
        _freeIds.Enqueue (id);
    }
}