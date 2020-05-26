namespace ProjectNothing.Utility
{
    public class CubePool : ObjectPool<CubePool, CubeObject>
    {
    }

    public class CubeObject : PoolObject<CubePool, CubeObject>
    {
    }
}