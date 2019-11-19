using UnityEngine;

namespace ProjectNothing
{
    public class GridController : MonoBehaviour
    {
        private TileMapController m_TileMapController;

        public void Awake ()
        {
            m_TileMapController = transform.Find ("Tilemap").GetComponent<TileMapController> ();
        }

        public void OnEnable ()
        {
            SelectionManager.Instance.AddLayerPointerDown (ELayerType.eLayer_Map, OnPointerDown);
        }

        public void OnDisable ()
        {
            SelectionManager.Instance.RemoveLayerPointerDown (ELayerType.eLayer_Map, OnPointerDown);
        }

        private void OnPointerDown (Vector2 point)
        {
            m_TileMapController.OnPointerDown (point);
        }
    }
}
