using System;
using UnityEngine;

namespace SproutLands
{
    public class GameManager : MonoSingleton<GameManager>
    {
        private TilesetManager _tilesetManager = default!;
        private WorldManager _worldManager = default!;

        public GameCamera gameCamera = default!;

        private PlayerEntity _player = default!;

        protected override void Awake ()
        {
            base.Awake ();

            ServiceLocator.Register<TilesetManager> ();
            ServiceLocator.Register<WorldManager> ();
        }

        private void Start ()
        {
            _tilesetManager = ServiceLocator.Resolve<TilesetManager> ();
            _tilesetManager.Initialize ();

            _worldManager = ServiceLocator.Resolve<WorldManager> ();

            WorldConfig config = new ()
            {
                Seed = Environment.TickCount,
                Width = 100,
                Height = 100,
                MinHeight = 0,
                MaxHeight = 1,
                ObjectRules = WorldRules.ObjectRules.Default
            };

            World? world = _worldManager.GenerateWord ("sprout_lands", config);
            if (world == null)
            {
                return;
            }

            _worldManager.ChangeWorld (world);

            GameObject teemo = Resources.Load<GameObject> ("teemo");
            GameObject player = Instantiate (teemo);
            _player = player.GetComponent<PlayerEntity> ();
            _player.EnterMap (world);

            gameCamera.MoveTo (player.transform.position);
            gameCamera.Follow (player.transform);
        }
    }
}
