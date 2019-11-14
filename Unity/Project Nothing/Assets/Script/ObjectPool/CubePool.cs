using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class CubePool : ObjectPool<CubePool, CubeObject>
{
}

public class CubeObject : PoolObject<CubePool, CubeObject>
{
}