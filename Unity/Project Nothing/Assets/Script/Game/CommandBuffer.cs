namespace ProjectNothing
{
    public sealed class CommandBuffer<TCommand> where TCommand : struct
    {
        readonly uint m_Size;
        readonly TCommand[] m_Commands;

        uint m_Tick;
        uint m_Count;

        public CommandBuffer (uint bufferSize)
        {
            m_Size = bufferSize;
            m_Commands = new TCommand[bufferSize];
            m_Tick = 0;
            m_Count = 0;
        }

        public void Add (uint tick, ref TCommand command)
        {
            m_Commands[tick % m_Size] = command;

            if (tick != m_Tick + 1)
            {
                m_Count = 1;
            }
            else if (m_Count < m_Size)
            {
                m_Count++;
            }

            m_Tick = tick;
        }

        public void Set (uint tick, ref TCommand command)
        {
            m_Commands[tick % m_Size] = command;
        }

        public void Clear ()
        {
            m_Count = 0;
        }

        public bool TryGetFirstTick (ref uint firstTick)
        {
            if (m_Count > 0 && m_Tick >= m_Count)
            {
                firstTick = m_Tick - m_Count + 1;
                return true;
            }

            return false;
        }

        public bool TryGetLastTick (ref uint lastTick)
        {
            if (m_Count > 0)
            {
                lastTick = m_Tick;
                return true;
            }

            return false;
        }

        public bool TryGetValue (uint tick, ref TCommand command)
        {
            if (IsValidTick (tick))
            {
                command = m_Commands[tick % m_Size];
                return true;
            }

            return false;
        }

        public bool IsValidTick (uint tick)
        {
            uint firstTick = 0;
            uint lastTick = 0;
            if (TryGetFirstTick (ref firstTick) && TryGetLastTick (ref lastTick))
            {
                return tick >= firstTick && tick <= lastTick;
            }

            return false;
        }
    }
}
