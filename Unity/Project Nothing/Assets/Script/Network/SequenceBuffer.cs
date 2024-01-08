using System;

namespace ProjectNothing
{
    public sealed class SequenceBuffer<TPacket> where TPacket : class, new()
    {
        readonly uint m_Size;
        readonly uint[] m_Indexes;
        readonly TPacket[] m_Packets;

        public SequenceBuffer (uint bufferSize)
        {
            m_Size = bufferSize;
            m_Indexes = new uint[bufferSize];
            Array.Fill (m_Indexes, m_Size);
            m_Packets = new TPacket[bufferSize];
            Array.Fill (m_Packets, new TPacket ());
        }

        public uint GetSize () { return m_Size; }

        public TPacket Insert (uint sequence)
        {
            uint index = sequence % m_Size;
            m_Indexes[index] = sequence;
            return m_Packets[index];
        }

        public void Remove (uint sequence)
        {
            uint index = sequence % m_Size;
            if (m_Indexes[index] == sequence)
            {
                m_Indexes[index] = m_Size;
            }
        }

        public void Clear ()
        {
            Array.Fill (m_Indexes, m_Size);
        }

        public TPacket TryGet (uint sequence)
        {
            uint index = sequence % m_Size;
            if (m_Indexes[index] == sequence)
            {
                return m_Packets[index];
            }
            else
            {
                return null;
            }
        }

        public TPacket TryGetByIndex (uint index, ref uint sequence)
        {
            if (m_Indexes[index] != m_Size)
            {
                sequence = m_Indexes[index];
                return m_Packets[index];
            }
            else
            {
                return null;
            }
        }

        public bool IsExist (uint sequence)
        {
            uint index = sequence % m_Size;
            return m_Indexes[index] == sequence;
        }
    }
}
