namespace ProjectNothing
{
    public struct GameTime
    {
        public ushort m_TickRate;
        public ulong m_TickInterval;

        public uint m_Tick;
        public ulong m_TickDuration;

        public GameTime (ushort tickRate)
        {
            m_TickRate = tickRate;
            m_TickInterval = (ulong)(1000 / tickRate);

            m_Tick = 0;
            m_TickDuration = 0;
        }

        public void SetTime (uint tick, ulong tickDuration)
        {
            m_Tick = tick;
            m_TickDuration = tickDuration;
        }

        public void AddDuration (ulong duration)
        {
            m_TickDuration += duration;
            m_Tick += (uint)(m_TickDuration / m_TickInterval);
            m_TickDuration %= m_TickInterval;
        }
    }
}
