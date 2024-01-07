using System;

namespace ProjectNothing
{
    public sealed class SequenceBuffer<TPacketInfo> where TPacketInfo : class, new()
    {
        readonly uint m_Size;
        readonly uint[] m_Indexes;
        readonly TPacketInfo[] m_PacketInfos;

        public SequenceBuffer (uint bufferSize)
        {
            m_Size = bufferSize;
            m_Indexes = new uint[bufferSize];
            Array.Fill (m_Indexes, m_Size);
            m_PacketInfos = new TPacketInfo[bufferSize];
            Array.Fill (m_PacketInfos, new TPacketInfo ());
        }

        public uint GetSize () { return m_Size; }

        public TPacketInfo Insert (uint sequence)
        {
            uint index = sequence % m_Size;
            m_Indexes[index] = sequence;
            return m_PacketInfos[index];
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

        public TPacketInfo TryGet (uint sequence)
        {
            uint index = sequence % m_Size;
            if (m_Indexes[index] == sequence)
            {
                return m_PacketInfos[index];
            }
            else
            {
                return null;
            }
        }

        public TPacketInfo TryGetByIndex (uint index, ref uint sequence)
        {
            if (m_Indexes[index] != m_Size)
            {
                sequence = m_Indexes[index];
                return m_PacketInfos[index];
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
