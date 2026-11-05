using UnityEngine;

namespace SproutLands
{
    public class PlayerActor
    {
        private readonly PlayerEntity _player;

        private readonly SpriteRenderer _spriteRenderer;
        private readonly Animator _animator;

        public PlayerActor (PlayerEntity player)
        {
            _player = player;
            _spriteRenderer = player.GetComponent<SpriteRenderer> ();
            _animator = player.GetComponent<Animator> ();
        }

        public void Initialize ()
        {
            _player.Mover.OnMoveBegin += OnMoveBegin;
            _player.Mover.OnMoveEnd += OnMoveEnd;
        }

        private void OnMoveBegin (Vector3Int beginCoord, Vector3Int endCoord)
        {
            if (_player.Map != null)
            {
                TileData? tileData = _player.Map.GetTileData (endCoord);
                if (tileData != null)
                {
                    _spriteRenderer.sortingOrder = Mathf.Max (_spriteRenderer.sortingOrder, tileData.Height + 1);
                }
            }

            _animator.SetBool ("isMoving", _player.Mover.IsMoving);
            _animator.SetFloat ("x", _player.Mover.Movement.x);
            _animator.SetFloat ("y", _player.Mover.Movement.y);
            _animator.SetFloat ("speed", _player.Mover.Speed);
        }

        private void OnMoveEnd (Vector3Int endCoord)
        {
            if (_player.Map != null)
            {
                TileData? tileData = _player.Map.GetTileData (endCoord);
                if (tileData != null)
                {
                    _spriteRenderer.sortingOrder = tileData.Height + 1;
                }
            }

            _animator.SetBool ("isMoving", _player.Mover.IsMoving);
        }
    }
}
