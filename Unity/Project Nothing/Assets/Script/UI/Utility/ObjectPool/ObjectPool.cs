using System.Collections.Generic;
using UnityEngine;

namespace ProjectNothing
{
    public abstract class ObjectPool<TPool, TObject> : MonoBehaviour
       where TPool : ObjectPool<TPool, TObject>
       where TObject : PoolObject<TPool, TObject>, new()
    {
        private GameObject m_Prefab;
        private readonly List<TObject> m_ObjectPool = new List<TObject> ();

        public virtual void Init (GameObject gameObject)
        {
            if (gameObject != null)
            {
                m_Prefab = gameObject;
            }

            for (int i = 0; i < 10; i++)
            {
                TObject poolObject = Create ();
                m_ObjectPool.Add (poolObject);
            }
        }

        protected virtual TObject Create ()
        {
            TObject poolObject = new TObject ();
            poolObject.SetGameObject (Instantiate (m_Prefab, transform));
            poolObject.SetPool (this as TPool);
            poolObject.InPool = true;
            poolObject.Sleep ();
            return poolObject;
        }

        public virtual TObject Pop ()
        {
            for (int i = 0; i < m_ObjectPool.Count; i++)
            {
                if (m_ObjectPool[i].InPool)
                {
                    m_ObjectPool[i].InPool = false;
                    m_ObjectPool[i].WakeUp ();
                    return m_ObjectPool[i];
                }
            }

            TObject poolObject = Create ();
            m_ObjectPool.Add (poolObject);
            poolObject.InPool = false;
            poolObject.WakeUp ();
            return poolObject;
        }

        public void Push (TObject poolObject)
        {
            poolObject.InPool = true;
            poolObject.Sleep ();
        }
    }

    public abstract class ObjectPool<TPool, TObject, TInfo> : ObjectPool<TPool, TObject>
        where TPool : ObjectPool<TPool, TObject, TInfo>
        where TObject : PoolObject<TPool, TObject, TInfo>, new()
    {
    }

    public abstract class PoolObject<TPool, TObject>
        where TPool : ObjectPool<TPool, TObject>
        where TObject : PoolObject<TPool, TObject>, new()
    {
        private GameObject m_PoolObject;
        private TPool m_ObjectPool;

        public bool InPool { get; set; }

        public virtual void SetGameObject (GameObject gameObject)
        {
            m_PoolObject = gameObject;
        }

        public virtual void SetPool (TPool pool)
        {
            m_ObjectPool = pool;
        }

        public virtual void WakeUp ()
        {
            m_PoolObject.SetActive (true);
        }

        public virtual void Sleep ()
        {
            m_PoolObject.SetActive (false);
        }

        public virtual void PushToPool ()
        {
            m_ObjectPool.Push (this as TObject);
        }
    }

    public abstract class PoolObject<TPool, TObject, TInfo> : PoolObject<TPool, TObject>
        where TPool : ObjectPool<TPool, TObject, TInfo>
        where TObject : PoolObject<TPool, TObject, TInfo>, new()
    {
    }
}
