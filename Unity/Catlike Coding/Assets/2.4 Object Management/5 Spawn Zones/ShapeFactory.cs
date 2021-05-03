﻿using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace ObjectManagement
{
    namespace SpawnZones
    {
        [CreateAssetMenu (fileName = "ShapeFactory", menuName = "Spawn Zones/ShapeFactory")]
        public class ShapeFactory : ScriptableObject
        {
            [SerializeField]
            Shape[] prefabs = null;

            [SerializeField]
            Material[] materials = null;

            [SerializeField]
            bool recycle = false;

            List<Shape>[] pools;

            Scene poolScene;

            public Shape Get (int shapeId = 0, int materialId = 0)
            {
                Shape instance;
                if (recycle)
                {
                    if (pools == null)
                    {
                        CreatPools ();
                    }
                    List<Shape> pool = pools[shapeId];
                    int lastIndex = pool.Count - 1;
                    if (lastIndex >= 0)
                    {
                        instance = pool[lastIndex];
                        instance.gameObject.SetActive (true);
                        pool.RemoveAt (lastIndex);
                    }
                    else
                    {
                        instance = Instantiate (prefabs[shapeId]);
                        instance.ShapeId = shapeId;
                        SceneManager.MoveGameObjectToScene (instance.gameObject, poolScene);
                    }
                }
                else
                {
                    instance = Instantiate (prefabs[shapeId]);
                    instance.ShapeId = shapeId;
                }
                instance.SetMaterial (materials[materialId], materialId);
                return instance;
            }

            public Shape GetRandom ()
            {
                return Get (
                    Random.Range (0, prefabs.Length),
                    Random.Range (0, materials.Length)
                );
            }

            void CreatPools ()
            {
                pools = new List<Shape>[prefabs.Length];
                for (int i = 0; i < pools.Length; i++)
                {
                    pools[i] = new List<Shape> ();
                }

                if (Application.isEditor)
                {
                    poolScene = SceneManager.GetSceneByName (name);
                    if (poolScene.isLoaded)
                    {
                        GameObject[] rootObjects = poolScene.GetRootGameObjects ();
                        for (int i = 0; i < rootObjects.Length; i++)
                        {
                            Shape pooledShape = rootObjects[i].GetComponent<Shape> ();
                            if (!pooledShape.gameObject.activeSelf)
                            {
                                pools[pooledShape.ShapeId].Add (pooledShape);
                            }
                        }
                        return;
                    }
                }

                poolScene = SceneManager.CreateScene (name);
            }

            public void Reclaim (Shape shapeToRecycle)
            {
                if (recycle)
                {
                    if (pools == null)
                    {
                        CreatPools ();
                    }
                    pools[shapeToRecycle.ShapeId].Add (shapeToRecycle);
                    shapeToRecycle.gameObject.SetActive (false);
                }
            }
        }
    }
}