using System.Collections;
using UnityEngine;
using UnityEngine.Tilemaps;
using UnityEngine.EventSystems;

namespace ProjectNothing
{
    public class PlayerController : MonoBehaviour, IPointerClickHandler
    {
        public TileMapController m_TileMapController;
        private Vector3Int m_Position;
        private PointerEventHandler m_OnPointerDownHandler;

        private void Awake ()
        {
            m_OnPointerDownHandler = OnTargetClick;
        }

        private void Start ()
        {
            m_Position = m_TileMapController.Tilemap.WorldToCell (transform.position);
        }

        private void MoveToPosition (Vector3Int target)
        {
            Tilemap tilemap = m_TileMapController.Tilemap;

            var targetPosition = tilemap.CellToWorld (target);

            var fromPosition = new Vector3 (transform.position.x, transform.position.y, transform.position.z);

            StartCoroutine (MoveToTarget (fromPosition, targetPosition));
        }

        private IEnumerator MoveToTarget (Vector3 from, Vector3 to)
        {
            Vector3 path = to - from;

            float time = 0.0f;
            while (time < 2.0f)
            {
                transform.position = from + (path * (time / 2.0f));

                time += Time.deltaTime;

                yield return null;
            }
        }

        public void OnPointerClick (PointerEventData eventData)
        {
            Vector3Int cell = m_TileMapController.Tilemap.WorldToCell (eventData.position);

            SelectionManager.Instance.AddPointDown (OnTargetClick);
        }

        public void OnTargetClick (Vector2 point)
        {
            Vector3Int cell = m_TileMapController.Tilemap.WorldToCell (Camera.main.ScreenToWorldPoint (point));

            MoveToPosition (cell);

            SelectionManager.Instance.RemovePointDown (OnTargetClick);
        }
    }
}
