using UnityEngine;
namespace ProjectNothing
{
    public class ScrollViewPagination
    {
        protected GameObject m_GameObject;

        public bool IsValid
        {
            get { return m_GameObject != null; }
        }

        public virtual void Init (GameObject gameObject)
        {
            m_GameObject = gameObject;
        }

        public virtual void Enable ()
        {
        }

        public virtual void Disable ()
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
    }
}
