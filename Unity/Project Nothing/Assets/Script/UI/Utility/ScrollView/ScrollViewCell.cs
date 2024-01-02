using UnityEngine;

namespace ProjectNothing
{
    public class ScrollViewCell
    {
        protected GameObject m_GameObject;
        protected RectTransform m_RectTransform;

        public float Width
        {
            get
            {
                if (m_RectTransform != null)
                {
                    return m_RectTransform.rect.width;
                }
                return 0f;
            }
        }

        public float Height
        {
            get
            {
                if (m_RectTransform != null)
                {
                    return m_RectTransform.rect.height;
                }
                return 0f;
            }
        }

        public bool IsValid
        {
            get { return m_GameObject != null; }
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

        public void Destory ()
        {
            if (m_GameObject != null)
            {
                Object.Destroy (m_GameObject);
            }
        }

        public void SetAnchorAndPivot ()
        {
            m_RectTransform.anchorMin = new Vector2 (0f, 1f);
            m_RectTransform.anchorMax = new Vector2 (0f, 1f);
            m_RectTransform.pivot = new Vector2 (0.5f, 0.5f);
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
