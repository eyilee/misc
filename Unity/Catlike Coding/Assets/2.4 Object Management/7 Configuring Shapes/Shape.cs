using UnityEngine;

namespace ObjectManagement
{
    namespace ConfiguringShapes
    {
        public class Shape : PersistableObject
        {
            static int colorPropertyId = Shader.PropertyToID ("_Color");
            static MaterialPropertyBlock sharedPropertyBlock;

            int shapeId = int.MinValue;

            public int ShapeId {
                get {
                    return shapeId;
                }
                set {
                    if (shapeId == int.MinValue && value != int.MinValue)
                    {
                        shapeId = value;
                    }
                    else
                    {
                        Debug.LogError ("Not allowed to change shapeId.");
                    }
                }
            }

            public int MaterialId {
                get; private set;
            }

            Color color;
            MeshRenderer meshRenderer;

            public Vector3 AngularVelocity { get; set; }
            public Vector3 Velocity { get; set; }

            void Awake ()
            {
                meshRenderer = GetComponent<MeshRenderer> ();
            }

            public void GameUpdate ()
            {
                transform.Rotate (AngularVelocity * Time.deltaTime);
                transform.localPosition += Velocity * Time.deltaTime;
            }

            public void SetMaterial (Material material, int materialId)
            {
                meshRenderer.material = material;
                MaterialId = materialId;
            }

            public void SetColor (Color color)
            {
                this.color = color;
                if (sharedPropertyBlock == null)
                {
                    sharedPropertyBlock = new MaterialPropertyBlock ();
                }
                sharedPropertyBlock.SetColor (colorPropertyId, color);
                meshRenderer.SetPropertyBlock (sharedPropertyBlock);
            }

            public override void Save (GameDataWriter writer)
            {
                base.Save (writer);
                writer.Write (color);
                writer.Write (AngularVelocity);
                writer.Write (Velocity);
            }

            public override void Load (GameDataReader reader)
            {
                base.Load (reader);
                SetColor (reader.Version > 0 ? reader.ReadColor () : Color.white);
                AngularVelocity = reader.Version >= 4 ? reader.ReadVector3 () : Vector3.zero;
                Velocity = reader.Version >= 4 ? reader.ReadVector3 () : Vector3.zero;
            }
        }
    }
}
