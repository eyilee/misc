using System.Collections.Generic;

namespace SproutLands
{
    public class TilesetDefinition
    {
        public string Name = string.Empty;
        public TilesetType Type;
        public string Path = string.Empty;
        public List<TileRule> Rules = new ();
    }
}
