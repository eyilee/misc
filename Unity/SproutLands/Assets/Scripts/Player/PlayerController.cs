using SproutLands.InputActions;
using UnityEngine;
using UnityEngine.InputSystem;

namespace SproutLands
{
    public class PlayerController : PlayerInputAction.IPlayerActions
    {
        private readonly PlayerEntity _player;

        private readonly PlayerInputAction _inputAction;
        private Vector2 _moveInput;
        private bool _runningInput;

        public PlayerController (PlayerEntity player)
        {
            _player = player;

            _inputAction = new PlayerInputAction ();
            _inputAction.Player.SetCallbacks (this);
        }

        public void Enable ()
        {
            _inputAction.Player.Enable ();
        }

        public void Disable ()
        {
            _inputAction.Player.Disable ();
        }

        public void Update ()
        {
            _player.Mover?.EnqueueCommand (InterpretInput ());
        }

        private PlayerMoveCommand InterpretInput ()
        {
            PlayerMoveCommand command = default;

            if (_moveInput.sqrMagnitude <= 0.5f)
            {
                return command;
            }

            if (Mathf.Abs (_moveInput.x) > Mathf.Abs (_moveInput.y))
            {
                command.Direction = _moveInput.x > 0 ? MapDirection.Right : MapDirection.Left;
            }
            else
            {
                command.Direction = _moveInput.y > 0 ? MapDirection.Up : MapDirection.Down;
            }

            command.IsRunning = _runningInput;

            return command;
        }

        public void OnMove (InputAction.CallbackContext context)
        {
            if (context.performed)
            {
                _moveInput = context.ReadValue<Vector2> ();
            }
            else if (context.canceled)
            {
                _moveInput = Vector2.zero;
            }
        }

        public void OnRun (InputAction.CallbackContext context)
        {
            if (context.performed)
            {
                _runningInput = context.ReadValueAsButton ();
            }
            else if (context.canceled)
            {
                _runningInput = false;
            }
        }
    }
}
