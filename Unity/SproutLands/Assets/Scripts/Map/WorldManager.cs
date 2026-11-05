using System.Collections.Generic;

namespace SproutLands
{
    public class WorldManager : Service<WorldManager>
    {
        private readonly WorldGenerator _worldGenerator = new ();

        private readonly Dictionary<string, World> _worldMaps = new ();

        private World? _activeWorld;

        public World? ActiveWorld => _activeWorld;

        public World? GetWorld (string name) => _worldMaps.TryGetValue (name, out World world) ? world : null;

        public bool ChangeWorld (World world)
        {
            if (_activeWorld != null)
            {
                if (_activeWorld == world)
                {
                    return false;
                }

                _activeWorld.Release ();
                _activeWorld = null;
            }

            _activeWorld = world;
            _activeWorld.Initialize ();

            return true;
        }

        public World? GenerateWord (string name, WorldConfig config)
        {
            if (_worldMaps.ContainsKey (name))
            {
                return null;
            }

            World? world = _worldGenerator.GenerateWorld (name, config);
            if (world == null)
            {
                return null;
            }

            _worldMaps.Add (name, world);

            return world;
        }
    }
}
