using System.Collections.Generic;

namespace SproutLands
{
    public class CommandBuffer<TCommand> where TCommand : struct
    {
        private readonly int _capacity;
        private readonly TCommand[] _commands;
        private int _index;
        private int _count;

        public CommandBuffer (int capacity)
        {
            _capacity = capacity;
            _commands = new TCommand[capacity];
        }

        public void BufferCommand (TCommand command)
        {
            _commands[(_index + _count) % _capacity] = command;

            if (_count < _capacity)
            {
                _count++;
            }
            else
            {
                _index = (_index + 1) % _capacity;
            }
        }

        public TCommand ConsumeCommand ()
        {
            TCommand command = default;

            for (int i = 0; i < _count; i++)
            {
                int index = (_index + i) % _capacity;

                if (!EqualityComparer<TCommand>.Default.Equals (_commands[index], default))
                {
                    command = _commands[index];
                    break;
                }
            }

            _index = 0;
            _count = 0;

            return command;
        }
    }
}
