using UnityEngine;

public class GameCamera : MonoBehaviour
{
    private bool _isFollowing = false;
    private Transform? _followObject = null;
    private Vector3 _velocity = Vector3.zero;
    private Vector3 _offset = new (0, 0, -10);

    private void LateUpdate ()
    {
        if (_isFollowing && _followObject != null)
        {
            transform.position = Vector3.SmoothDamp (transform.position, _followObject.position + _offset, ref _velocity, 0.3f);
        }
    }

    public void Follow (Transform target)
    {
        _isFollowing = true;
        _followObject = target;
    }

    public void Unfollow ()
    {
        _isFollowing = false;
        _followObject = null;
    }

    public void MoveTo (Vector3 position)
    {
        _velocity = Vector3.zero;

        transform.position = position + _offset;
    }
}
