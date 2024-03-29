﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

namespace ObjectManagement
{
    namespace ConfiguringShapes
    {
        public class Game : PersistableObject
        {
            const int saveVerion = 4;

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

            Random.State mainRandomState;

            [SerializeField]
            bool reseedOnLoad = false;

            [SerializeField]
            Slider creationSpeedSlider = null;

            [SerializeField]
            Slider destructionSpeedSlider = null;

            void Start ()
            {
                mainRandomState = Random.state;

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

                BeginNewGame ();
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
                    StartCoroutine (LoadLevel (loadedLevelBuildIndex));
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
            }

            void FixedUpdate ()
            {
                for (int i = 0; i < shapes.Count; i++)
                {
                    shapes[i].GameUpdate ();
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
                GameLevel.Current.ConfigureSpawn (instance);
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
                Random.state = mainRandomState;
                int seed = Random.Range (0, int.MaxValue) ^ (int)Time.unscaledTime;
                Random.InitState (seed);

                CreationSpeed = 0;
                creationSpeedSlider.value = 0;
                DestructionSpeed = 0;
                destructionSpeedSlider.value = 0;

                for (int i = 0; i < shapes.Count; i++)
                {
                    shapeFactory.Reclaim (shapes[i]);
                }
                shapes.Clear ();
            }

            public override void Save (GameDataWriter writer)
            {
                writer.Write (shapes.Count);
                writer.Write (Random.state);
                writer.Write (CreationSpeed);
                writer.Write (creationProgress);
                writer.Write (DestructionSpeed);
                writer.Write (destructionProgress);
                writer.Write (loadedLevelBuildIndex);
                GameLevel.Current.Save (writer);
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

                StartCoroutine (LoadGame (reader));
            }

            IEnumerator LoadGame (GameDataReader reader)
            {
                int version = reader.Version;
                int count = version <= 0 ? -version : reader.ReadInt ();

                if (version >= 3)
                {
                    Random.State state = reader.ReadRandomState ();
                    if (!reseedOnLoad)
                    {
                        Random.state = state;
                    }
                    CreationSpeed = reader.ReadFloat ();
                    creationSpeedSlider.value = CreationSpeed;
                    creationProgress = reader.ReadFloat ();
                    DestructionSpeed = reader.ReadFloat ();
                    destructionSpeedSlider.value = DestructionSpeed;
                    destructionProgress = reader.ReadFloat ();
                }

                yield return LoadLevel (version < 2 ? 1 : reader.ReadInt ());

                if (version >= 3)
                {
                    GameLevel.Current.Load (reader);
                }

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

