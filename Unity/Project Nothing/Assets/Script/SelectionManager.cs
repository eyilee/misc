using UnityEngine;
using UnityEngine.EventSystems;

namespace ProjectNothing
{
    public class SelectionManager : MonoSingleton<SelectionManager>
    {
        public delegate void PointerEventHandler (BaseEventData baseEventData);
        event PointerEventHandler OnLongPress;
        event PointerEventHandler OnLongPressEnd;
        event PointerEventHandler OnClick;

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
                    switch (hit.collider.gameObject.layer)
                    {
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

        public void OnSelect ()
        {
        }

        public void OnDeSelect ()
        {
        }
    }
}
