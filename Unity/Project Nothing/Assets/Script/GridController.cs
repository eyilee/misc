using UnityEngine;

namespace ProjectNothing
{
    public class GridController : MonoBehaviour
    {
        SelectionManager m_SelectionManager;
        PointerEventHandler m_OnPointerDownHandler;

        TileMapController m_TileMapController;

        public void Awake ()
        {
            m_SelectionManager = SelectionManager.Instance;
            m_OnPointerDownHandler = OnPointerDown;

            m_TileMapController = transform.Find ("Tilemap").GetComponent<TileMapController> ();
        }

        public void OnEnable ()
        {
            m_SelectionManager.AddLayerPointerDown (ELayerType.eLayer_Map, m_OnPointerDownHandler);
        }

        public void OnDisable ()
        {
            m_SelectionManager.RemoveLayerPointerDown (ELayerType.eLayer_Map, m_OnPointerDownHandler);
        }

        public void OnPointerDown (Vector2 point)
        {
            m_TileMapController.OnPointerDown (point);
        }
    }
}
