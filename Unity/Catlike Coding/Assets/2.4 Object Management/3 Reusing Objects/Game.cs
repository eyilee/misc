﻿using System.Collections.Generic;
using UnityEngine;

namespace ObjectManagement
{
    namespace ReusingObjects
    {
        public class Game : PersistableObject
        {
            const int saveVerion = 1;

            public ShapeFactory shapeFactory;
            public KeyCode createKey = KeyCode.C;
            public KeyCode destoryKey = KeyCode.X;
            public KeyCode newGameKey = KeyCode.N;
            public KeyCode saveKey = KeyCode.S;
            public KeyCode loadKey = KeyCode.L;

            public PersistentStorage storage;

            List<Shape> shapes;

            public float CreationSpeed { get; set; }
            float creationProgress = 0f;

            public float DestructionSpeed { get; set; }
            float destructionProgress = 0f;

            void Awake ()
            {
                shapes = new List<Shape> ();
            }

            void Update ()
            {
                if (Input.GetKeyDown (createKey))
                {
                    CreateShape ();
                }
                else if (Input.GetKeyDown (destoryKey))
                {
                    DestroyShape ();
                }
                else if (Input.GetKeyDown (newGameKey))
                {
                    BeginNewGame ();
                }
                else if (Input.GetKeyDown (saveKey))
                {
                    storage.Save (this, saveVerion);
                }
                else if (Input.GetKeyDown (loadKey))
                {
                    BeginNewGame ();
                    storage.Load (this);
                }

                creationProgress += Time.deltaTime * CreationSpeed;
                while (creationProgress >= 1f)
                {
                    creationProgress -= 1f;
                    CreateShape ();
                }

                destructionProgress += Time.deltaTime * DestructionSpeed;
                while (destructionProgress >= 1f)
                {
                    destructionProgress -= 1f;
                    DestroyShape ();
                }
            }

            void CreateShape ()
            {
                Shape instance = shapeFactory.GetRandom ();
                Transform t = instance.transform;
                t.localPosition = Random.insideUnitSphere * 5f;
                t.localRotation = Random.rotation;
                t.localScale = Vector3.one * Random.Range (0.1f, 1f);
                instance.SetColor (Random.ColorHSV (
                    hueMin: 0f, hueMax: 1f,
                    saturationMin: 0.5f, saturationMax: 1f,
                    valueMin: 0.25f, valueMax: 1f,
                    alphaMin: 1f, alphaMax: 1f
                ));
                shapes.Add (instance);
            }

            void DestroyShape ()
            {
                if (shapes.Count > 0)
                {
                    int index = Random.Range (0, shapes.Count);
                    shapeFactory.Reclaim (shapes[index]);
                    int lastIndex = shapes.Count - 1;
                    shapes[index] = shapes[lastIndex];
                    shapes.RemoveAt (lastIndex);
                }
            }

            void BeginNewGame ()
            {
                for (int i = 0; i < shapes.Count; i++)
                {
                    shapeFactory.Reclaim (shapes[i]);
                }
                shapes.Clear ();
            }

            public override void Save (GameDataWriter writer)
            {
                writer.Write (shapes.Count);
                for (int i = 0; i < shapes.Count; i++)
                {
                    writer.Write (shapes[i].ShapeId);
                    writer.Write (shapes[i].MaterialId);
                    shapes[i].Save (writer);
                }
            }

            public override void Load (GameDataReader reader)
            {
                int version = reader.Version;
                if (version > saveVerion)
                {
                    Debug.LogError ("Unsupported future save version " + version);
                    return;
                }
                int count = version <= 0 ? -version : reader.ReadInt ();
                for (int i = 0; i < count; i++)
                {
                    int shapeId = version > 0 ? reader.ReadInt () : 0;
                    int materialId = version > 0 ? reader.ReadInt () : 0;
                    Shape instance = shapeFactory.Get (shapeId, materialId);
                    instance.Load (reader);
                    shapes.Add (instance);
                }
            }
        }
    }
}

