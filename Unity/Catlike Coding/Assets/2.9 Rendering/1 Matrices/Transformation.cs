using UnityEngine;

namespace Rendering
{
    namespace Matrices
    {
        public abstract class Transformation : MonoBehaviour
        {
            public abstract Matrix4x4 Matrix { get; }
        }
    }
}