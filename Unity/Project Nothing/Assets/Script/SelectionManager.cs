using System;
using UnityEngine;

namespace ProjectNothing
{
    public delegate void PointerEventHandler (Vector2 point);

    public class SelectionManager : MonoSingleton<SelectionManager>
    {
        private Camera m_Camera;
        private int m_LayerMask = 0;

        private bool m_IsLeftButtonPressed = false;

        private event PointerEventHandler OnPointerDown;
        private event PointerEventHandler OnLayerMapPointerDown;

        public void Awake ()
        {
            m_Camera = Camera.main;

            foreach (ELayerType eLayerType in Enum.GetValues (typeof (ELayerType)))
            {
                m_LayerMask |= 1 << (int)eLayerType;
            }
        }

        public void Update ()
        {
            if (!m_IsLeftButtonPressed && Input.GetMouseButton (0))
            {
                m_IsLeftButtonPressed = true;

                RaycastHit2D hit = Physics2D.Raycast (m_Camera.ScreenToWorldPoint (Input.mousePosition), Vector2.zero, 1.0f, m_LayerMask);
                if (hit.collider != null)
                {
                    switch ((ELayerType)hit.collider.gameObject.layer)
                    {
                        case ELayerType.eLayer_Default:
                            break;
                        case ELayerType.eLayer_TransparentFX:
                            break;
                        case ELayerType.eLayer_IgnoreRaycast:
                            break;
                        case ELayerType.eLayer_Water:
                            break;
                        case ELayerType.eLayer_UI:
                            break;
                        case ELayerType.eLayer_Map:
                            OnLayerMapPointerDown?.Invoke (hit.point);
                            break;
                        default:
                            break;
                    }
                }

                OnPointerDown?.Invoke (Input.mousePosition);
            }
            else if (m_IsLeftButtonPressed && !Input.GetMouseButton (0))
            {
                m_IsLeftButtonPressed = false;
            }
        }

        public void AddPointDown (PointerEventHandler pointerEventHandler)
        {
            OnPointerDown += pointerEventHandler;
        }

        public void RemovePointDown (PointerEventHandler pointerEventHandler)
        {
            OnPointerDown -= pointerEventHandler;
        }

        public void AddLayerPointerDown (ELayerType eLayerType, PointerEventHandler pointerEventHandler)
        {
            switch (eLayerType)
            {
                case ELayerType.eLayer_Default:
                    break;
                case ELayerType.eLayer_TransparentFX:
                    break;
                case ELayerType.eLayer_IgnoreRaycast:
                    break;
                case ELayerType.eLayer_Water:
                    break;
                case ELayerType.eLayer_UI:
                    break;
                case ELayerType.eLayer_Map:
                    OnLayerMapPointerDown += pointerEventHandler;
                    break;
                default:
                    break;
            }
        }

        public void RemoveLayerPointerDown (ELayerType eLayerType, PointerEventHandler pointerEventHandler)
        {
            switch (eLayerType)
            {
                case ELayerType.eLayer_Default:
                    break;
                case ELayerType.eLayer_TransparentFX:
                    break;
                case ELayerType.eLayer_IgnoreRaycast:
                    break;
                case ELayerType.eLayer_Water:
                    break;
                case ELayerType.eLayer_UI:
                    break;
                case ELayerType.eLayer_Map:
                    OnLayerMapPointerDown -= pointerEventHandler;
                    break;
                default:
                    break;
            }
        }
    }
}
