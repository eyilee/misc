using System.Collections.Generic;
using UnityEngine;

namespace ProjectNothing.Utility
{
    public abstract class ObjectPool<TPool, TObject> : MonoBehaviour
       where TPool : ObjectPool<TPool, TObject>
       where TObject : PoolObject<TPool, TObject>, new()
    {
        GameObject prefab;
        readonly List<TObject> objectPool = new List<TObject> ();

        public virtual void Init (GameObject gameObject)
        {
            if (gameObject != null)
            {
                prefab = gameObject;
            }

            for (int i = 0; i < 10; i++)
            {
                TObject poolObject = Create ();
                objectPool.Add (poolObject);
            }
        }

        protected virtual TObject Create ()
        {
            TObject poolObject = new TObject ();
            poolObject.SetGameObject (Instantiate (prefab, transform));
            poolObject.SetPool (this as TPool);
            poolObject.InPool = true;
            poolObject.Sleep ();
            return poolObject;
        }

        public virtual TObject Pop ()
        {
            for (int i = 0; i < objectPool.Count; i++)
            {
                if (objectPool[i].InPool)
                {
                    objectPool[i].InPool = false;
                    objectPool[i].WakeUp ();
                    return objectPool[i];
                }
            }

            TObject poolObject = Create ();
            objectPool.Add (poolObject);
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
        GameObject poolObject;
        TPool objectPool;

        public bool InPool { get; set; }

        public virtual void SetGameObject (GameObject gameObject)
        {
            poolObject = gameObject;
        }

        public virtual void SetPool (TPool pool)
        {
            objectPool = pool;
        }

        public virtual void WakeUp ()
        {
            poolObject.SetActive (true);
        }

        public virtual void Sleep ()
        {
            poolObject.SetActive (false);
        }

        public virtual void PushToPool ()
        {
            objectPool.Push (this as TObject);
        }
    }

    public abstract class PoolObject<TPool, TObject, TInfo> : PoolObject<TPool, TObject>
        where TPool : ObjectPool<TPool, TObject, TInfo>
        where TObject : PoolObject<TPool, TObject, TInfo>, new()
    {
    }
}
