using UnityEngine;

namespace SproutLands
{
    public class PlayerEntity : MonoBehaviour
    {
        public PlayerController Controller { get; private set; } = default!;
        public PlayerActor Actor { get; private set; } = default!;
        public PlayerMover Mover { get; private set; } = default!;

        public IGridMap? Map;
        public Vector3Int Coord;

        private void Awake ()
        {
            Controller = new PlayerController (this);
            Actor = new PlayerActor (this);
            Mover = new PlayerMover (this);

            Initialize ();
        }

        private void Initialize ()
        {
            Actor.Initialize ();
        }

        private void OnEnable ()
        {
            Controller.Enable ();
        }

        private void OnDisable ()
        {
            Controller.Disable ();
        }

        private void Update ()
        {
            float deltaTime = Time.deltaTime;
            Controller.Update ();
            Mover.Update (deltaTime);
        }

        public void EnterMap (IGridMap map)
        {
            if (Map != null)
            {
                // Exit map
            }

            Map = map;

            Coord = Map.GetSpawnCoord ();
            Mover.MoveTo (Coord);
            // Enter map
        }
    }
}
