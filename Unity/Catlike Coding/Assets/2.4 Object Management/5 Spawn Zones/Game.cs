﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace ObjectManagement
{
    namespace SpawnZones
    {
        public class Game : PersistableObject
        {
            const int saveVerion = 2;

            [SerializeField]
            ShapeFactory shapeFactory = null;

            [SerializeField]
            KeyCode createKey = KeyCode.C;

            [SerializeField]
            KeyCode destoryKey = KeyCode.X;

            [SerializeField]
            KeyCode newGameKey = KeyCode.N;

            [SerializeField]
            KeyCode saveKey = KeyCode.S;

            [SerializeField]
            KeyCode loadKey = KeyCode.L;

            [SerializeField]
            PersistentStorage storage = null;

            List<Shape> shapes;

            public float CreationSpeed { get; set; }
            float creationProgress = 0f;

            public float DestructionSpeed { get; set; }
            float destructionProgress = 0f;

            [SerializeField]
            int levelCount = 0;

            int loadedLevelBuildIndex;

            public SpawnZone SpawnZoneOfLevel { get; set; }

            public static Game Instance { get; private set; }

            void OnEnable ()
            {
                Instance = this;
            }

            void Start ()
            {
                Instance = this;

                shapes = new List<Shape> ();

                if (Application.isEditor)
                {
                    for (int i = 0; i < SceneManager.sceneCount; i++)
                    {
                        Scene loadedScene = SceneManager.GetSceneAt (i);
                        if (loadedScene.name.Contains ("Level "))
                        {
                            loadedLevelBuildIndex = loadedScene.buildIndex;
                            SceneManager.SetActiveScene (loadedScene);
                            return;
                        }
                    }
                }

                StartCoroutine (LoadLevel (1));
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
                else
                {
                    for (int i = 1; i <= levelCount; i++)
                    {
                        if (Input.GetKeyDown (KeyCode.Alpha0 + i))
                        {
                            BeginNewGame ();
                            StartCoroutine (LoadLevel (i));
                            return;
                        }
                    }
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

            IEnumerator LoadLevel (int levelBuildIndex)
            {
                enabled = false;
                if (loadedLevelBuildIndex > 0)
                {
                    yield return SceneManager.UnloadSceneAsync (loadedLevelBuildIndex);
                }
                yield return SceneManager.LoadSceneAsync (levelBuildIndex, LoadSceneMode.Additive);
                SceneManager.SetActiveScene (SceneManager.GetSceneByBuildIndex (levelBuildIndex));
                loadedLevelBuildIndex = levelBuildIndex;
                enabled = true;
            }

            void CreateShape ()
            {
                Shape instance = shapeFactory.GetRandom ();
                Transform t = instance.transform;
                t.localPosition = SpawnZoneOfLevel.SpawnPoint;
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
                writer.Write (loadedLevelBuildIndex);
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
                StartCoroutine (LoadLevel (version < 2 ? 1 : reader.ReadInt ()));
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

