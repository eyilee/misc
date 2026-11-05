namespace SproutLands
{
    public static class WorldRules
    {
        public static class ObjectRules
        {
            public static ObjectRule[] Default =
            {
                new ()
                {
                    Name = "grass",
                    Priority = 0,
                    Type = ObjectType.Grass,
                    UseHeightLimit = false,
                    RequireWalkable = true,
                    BaseChance = 0.3f,
                    UseCluster = true,
                    MinClusterSize = 2,
                    MaxClusterSize = 4
                },
                new ()
                {
                    Name = "rock",
                    Priority = 1,
                    Type = ObjectType.Rock,
                    UseHeightLimit = false,
                    RequireWalkable = true,
                    BaseChance = 0.1f,
                    UseCluster = false,
                }
            };
        }
    }
}
