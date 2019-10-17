using System;
using UnityEngine;

namespace ProjectNothing
{
    public delegate void PointerEventHandler (Vector2 point);

    public class SelectionManager : MonoSingleton<SelectionManager>
    {
        Camera m_Camera;

        int m_LayerMask = 0;

        event PointerEventHandler OnPointerDown;
        event PointerEventHandler OnLayerMapPointerDown;

        bool m_IsLeftButtonPressed = false;

        public void Awake ()
        {
            m_Camera = Camera.main;

            foreach (ELayerType eLayerType in Enum.GetValues (typeof (ELayerType)))
            {
                m_LayerMask |= 1 << (int)eLayerType;
            }
        }

        public void Start ()
        {
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
                            if (OnLayerMapPointerDown != null)
                            {
                                OnLayerMapPointerDown (hit.point);
                            }
                            break;
                        default:
                            break;
                    }
                }

                if (OnPointerDown != null)
                {
                    OnPointerDown (Input.mousePosition);
                }
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
