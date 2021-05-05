using UnityEngine;

namespace ObjectManagement
{
    namespace ConfiguringShapes
    {
        public abstract class SpawnZone : PersistableObject
        {
            [System.Serializable]
            public struct SpawnConfiguration
            {

                public enum MovementDirection
                {
                    Forward,
                    Upward,
                    Outward,
                    Random
                }

                public MovementDirection movementDirection;

                public FloatRange speed;

                public FloatRange angularSpeed;

                public FloatRange scale;

                public ColorRangeHSV color;
            }

            [SerializeField]
            SpawnConfiguration spawnConfig;

            public abstract Vector3 SpawnPoint { get; }

            public virtual void ConfigureSpawn (Shape shape)
            {
                Transform t = shape.transform;
                t.localPosition = SpawnPoint;
                t.localRotation = Random.rotation;
                t.localScale = Vector3.one * spawnConfig.scale.RandomValueInRange;
                shape.SetColor (spawnConfig.color.RandomInRange);
                shape.AngularVelocity = Random.onUnitSphere * spawnConfig.angularSpeed.RandomValueInRange;

                Vector3 direction;
                switch (spawnConfig.movementDirection)
                {
                    case SpawnConfiguration.MovementDirection.Upward:
                        direction = Vector3.up;
                        break;
                    case SpawnConfiguration.MovementDirection.Outward:
                        direction = (t.localPosition - transform.position).normalized;
                        break;
                    case SpawnConfiguration.MovementDirection.Random:
                        direction = Random.onUnitSphere;
                        break;
                    default:
                        direction = Vector3.forward;
                        break;
                }
                shape.Velocity = direction * spawnConfig.speed.RandomValueInRange;
            }
        }
    }
}
