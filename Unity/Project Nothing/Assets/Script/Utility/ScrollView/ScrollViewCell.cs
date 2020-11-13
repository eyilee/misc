using UnityEngine;

namespace ProjectNothing.Utility
{
    public class ScrollViewCell
    {
        protected GameObject m_GameObject;
        protected RectTransform m_RectTransform;

        public bool IsValid {
            get { return m_GameObject.activeSelf; }
        }

        public virtual void Init (GameObject gameObject)
        {
            m_GameObject = gameObject;
            m_RectTransform = m_GameObject.GetComponent<RectTransform> ();
        }

        public virtual void SetView ()
        {
        }

        public virtual void Sleep ()
        {
            m_GameObject.SetActive (false);
        }

        public virtual void Wakeup ()
        {
            m_GameObject.SetActive (true);
        }

        public void SetAnchoredPositionX (float x)
        {
            m_RectTransform.anchoredPosition = new Vector2 (x, m_RectTransform.anchoredPosition.y);
        }

        public void SetAnchoredPositionY (float y)
        {
            m_RectTransform.anchoredPosition = new Vector2 (m_RectTransform.anchoredPosition.x, y);
        }

        public void SetAnchoredPosition (Vector2 position)
        {
            m_RectTransform.anchoredPosition = position;
        }
    }
}
