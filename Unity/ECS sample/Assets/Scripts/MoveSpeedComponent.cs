using Unity.Entities;

[System.Serializable]
public struct MoveSpeed : IComponentData
{
    public float Value;
    public float x;
    public float y;
}

public class MoveSpeedComponent : ComponentDataProxy<MoveSpeed>
{
}
