using UnityEngine;

namespace ProjectNothing
{
    public class GridController : MonoBehaviour
    {
        PointerEventHandler m_OnPointerDownHandler;

        TileMapController m_TileMapController;

        public void Awake ()
        {
            m_OnPointerDownHandler = OnPointerDown;

            m_TileMapController = transform.Find ("Tilemap").GetComponent<TileMapController> ();
        }

        public void OnEnable ()
        {
            SelectionManager.Instance.AddLayerPointerDown (ELayerType.eLayer_Map, m_OnPointerDownHandler);
        }

        public void OnDisable ()
        {
            SelectionManager.Instance.RemoveLayerPointerDown (ELayerType.eLayer_Map, m_OnPointerDownHandler);
        }

        public void OnPointerDown (Vector2 point)
        {
            m_TileMapController.OnPointerDown (point);
        }
    }
}
