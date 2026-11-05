namespace SproutLands
{
    public struct WorldConfig
    {
        public int Seed;
        public int Width;
        public int Height;
        public int MinHeight;
        public int MaxHeight;
        public ObjectRule[] ObjectRules;

        public static WorldConfig Default = new ()
        {
            Seed = 0,
            Width = 32,
            Height = 32,
            MinHeight = 1,
            MaxHeight = 1,
            ObjectRules = WorldRules.ObjectRules.Default,
        };
    }
}
