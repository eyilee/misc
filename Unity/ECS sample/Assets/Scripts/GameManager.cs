using Unity.Collections;
using Unity.Entities;
using Unity.Mathematics;
using Unity.Transforms;
using UnityEngine;
using Random = UnityEngine.Random;

public class GameManager : MonoBehaviour
{
    static GameManager Instance;
    public static GameManager GM { get { return Instance; } }

    public GameObject cubePrefab;

    public float topBound = 5.0f;
    public float bottomBound = -5.0f;
    public float leftBound = -5.0f;
    public float rightBound = 5.0f;

    EntityManager manager;

    void Awake ()
    {
        if (Instance == null)
        {
            Instance = GetComponent<GameManager> ();
        }
    }

    void Start ()
    {
        manager = World.Active.GetOrCreateManager<EntityManager> ();

        AddCubes (2000);
    }

    void Update ()
    {
        if (Input.GetKeyDown ("space"))
        {
            AddCubes (2000);
        }
    }

    void AddCubes (int amount)
    {
        NativeArray<Entity> entities = new NativeArray<Entity> (amount, Allocator.Temp);
        manager.Instantiate (cubePrefab, entities);

        for (int i = 0; i < amount; i++)
        {
            float xVal = Random.Range (leftBound, rightBound);
            float yVal = Random.Range (topBound, bottomBound);

            manager.SetComponentData (entities[i], new Position { Value = new float3 (xVal, yVal, 0f) });
            manager.SetComponentData (entities[i], new Rotation { Value = Quaternion.identity });
            manager.SetComponentData (entities[i], new Scale { Value = new float3 (0.1f, 0.1f, 1f) });
            manager.SetComponentData (entities[i], new MoveSpeed { Value = 1f, x = Random.Range (-1f, 1f), y = Random.Range (-1f, 1f) });
        }

        entities.Dispose ();
    }
}
