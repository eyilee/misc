namespace SproutLands
{
    public class ObjectRule
    {
        public string Name = string.Empty;
        public int Priority;
        public ObjectType Type;

        public bool UseHeightLimit;
        public int MinHeight;
        public int MaxHeight;

        public bool RequireWalkable;

        public float BaseChance;

        public bool UseCluster;
        public int MinClusterSize;
        public int MaxClusterSize;
    }
}