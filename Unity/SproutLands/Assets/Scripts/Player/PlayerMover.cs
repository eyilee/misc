using System;
using UnityEngine;

namespace SproutLands
{
    public class PlayerMover
    {
        private const float WalkingTime = 0.45f;
        private const float RunningTime = 0.3f;

        private readonly PlayerEntity _player;
        private readonly CommandBuffer<PlayerMoveCommand> _commandBuffer;

        private Vector3Int _beginCoord = Vector3Int.zero;
        private Vector3Int _endCoord = Vector3Int.zero;
        private Vector3 _beginPosition = Vector3.zero;
        private Vector3 _endPosition = Vector3.zero;
        private Vector3 _movement = Vector3.zero;
        private float _movingTime = WalkingTime;
        private float _movingProgress = 0f;
        private float _movingSpeed = 1f;

        public Vector3Int BeginCoord => _beginCoord;

        public Vector3Int EndCoord => _endCoord;

        public bool IsMoving => _movingProgress > 0f;

        public Vector3 Movement => _movingProgress > 0f ? _movement : Vector3.zero;

        public float Speed => _movingSpeed;

        public event Action<Vector3Int, Vector3Int>? OnMoveBegin;

        public event Action<Vector3Int>? OnMoveEnd;

        public PlayerMover (PlayerEntity player)
        {
            _player = player;
            _commandBuffer = new CommandBuffer<PlayerMoveCommand> (2);
        }

        public void MoveTo (Vector3Int coord)
        {
            if (_player.Map == null || !_player.Map.CanEnter (coord))
            {
                return;
            }

            _beginCoord = coord;
            _endCoord = coord;
            _beginPosition = _player.Map.GetCellCenterWorld (coord);
            _endPosition = _player.Map.GetCellCenterWorld (coord);
            _movement = MapDirection.Down.ToVector3 ();
            _movingTime = WalkingTime;
            _movingProgress = 0f;
            _movingSpeed = 1f;

            OnMoveBegin?.Invoke (coord, coord);

            _player.Coord = coord;
            _player.transform.position = _endPosition;

            OnMoveEnd?.Invoke (coord);
        }

        public void Update (float deltaTime)
        {
            if (_movingProgress > 0f)
            {
                ProcessMovement (deltaTime);
            }
            else
            {
                ExecuteCommand (_commandBuffer.ConsumeCommand ());
            }
        }

        public void EnqueueCommand (PlayerMoveCommand command) => _commandBuffer.BufferCommand (command);

        private void ExecuteCommand (PlayerMoveCommand command)
        {
            if (command.Direction == MapDirection.None)
            {
                return;
            }

            Vector3Int beginCoord = _player.Coord;
            Vector3Int endCoord = beginCoord + command.Direction.ToVector3Int ();

            if (_player.Map == null || !_player.Map.CanEnter (endCoord))
            {
                endCoord = beginCoord;
            }

            _beginCoord = beginCoord;
            _endCoord = endCoord;

            if (_player.Map == null)
            {
                _beginPosition = _player.transform.position;
                _endPosition = _beginPosition;
            }
            else
            {
                _beginPosition = _player.Map.GetCellCenterWorld (beginCoord);
                _endPosition = _player.Map.GetCellCenterWorld (endCoord);
            }

            _movement = command.Direction.ToVector3 ();
            _movingTime = command.IsRunning ? RunningTime : WalkingTime;
            _movingProgress = _movingTime;
            _movingSpeed = WalkingTime / _movingTime;

            OnMoveBegin?.Invoke (_beginCoord, _endCoord);
        }

        private void ProcessMovement (float deltaTime)
        {
            if (_movingProgress <= 0f)
            {
                return;
            }

            _movingProgress -= deltaTime;

            float t = Mathf.Clamp01 ((_movingTime - _movingProgress) / _movingTime);
            _player.transform.position = Vector3.Lerp (_beginPosition, _endPosition, t);

            if (_movingProgress <= 0f)
            {
                _player.Coord = _endCoord;
                _player.transform.position = _endPosition;

                OnMoveEnd?.Invoke (_endCoord);

                ExecuteCommand (_commandBuffer.ConsumeCommand ());
            }
        }
    }
}
