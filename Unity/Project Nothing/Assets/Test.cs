using UnityEngine;
using UnityEngine.AddressableAssets;
using UnityEngine.ResourceManagement.AsyncOperations;

public class Test : MonoBehaviour
{
    public GameObject m_MyGameObject;
    public AssetReference assetReference;

    void Start ()
    {
        Addressables.LoadAssetAsync<GameObject> ("Assets/Prefab/Cube.prefab").Completed += OnLoadDone;
    }

    private void OnLoadDone (AsyncOperationHandle<GameObject> obj)
    {
        m_MyGameObject = obj.Result;
        Instantiate (m_MyGameObject);
    }
}
