using UnityEngine;
using UnityEngine.EventSystems;

namespace ProjectNothing
{
    public class SelectionManager : MonoSingleton<SelectionManager>
    {
        public delegate void PointerEventHandler (Vector2 point);
        //event PointerEventHandler OnLongPress;
        //event PointerEventHandler OnLongPressEnd;
        //event PointerEventHandler OnClick;
        event PointerEventHandler OnLayerMapPointerDown;

        // current scene
        // current state
        bool isLeftButtonPressed = false;

        public void Start ()
        {

        }

        public void Update ()
        {
            if (!isLeftButtonPressed && Input.GetMouseButton (0))
            {
                isLeftButtonPressed = true;

                RaycastHit2D hit = Physics2D.Raycast (Input.mousePosition, Vector2.zero);
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
                            OnLayerMapPointerDown (hit.point);
                            break;
                        default:
                            break;
                    }
                }
            }
            else if (isLeftButtonPressed && !Input.GetMouseButton (0))
            {
                isLeftButtonPressed = false;
            }
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

        public void OnSelect ()
        {
        }

        public void OnDeSelect ()
        {
        }
    }
}
