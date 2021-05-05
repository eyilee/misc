﻿using UnityEngine;

namespace ObjectManagement
{
    namespace ConfiguringShapes
    {
        public class RotatingObject : PersistableObject
        {
            [SerializeField]
            Vector3 angularVelocity = Vector3.zero;

            void FixedUpdate ()
            {
                transform.Rotate (angularVelocity * Time.deltaTime);
            }
        }
    }
}
