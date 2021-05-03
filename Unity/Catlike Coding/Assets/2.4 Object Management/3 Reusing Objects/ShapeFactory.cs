﻿using System.Collections.Generic;
using UnityEngine;

namespace ObjectManagement
{
    namespace ReusingObjects
    {
        [CreateAssetMenu (fileName = "ShapeFactory", menuName = "Object Management/Reusing Objects/ShapeFactory")]
        public class ShapeFactory : ScriptableObject
        {
            [SerializeField]
            Shape[] prefabs = null;

            [SerializeField]
            Material[] materials = null;

            [SerializeField]
            bool recycle = false;

            List<Shape>[] pools;

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