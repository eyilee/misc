using System;

namespace ProjectNothing
{
    public sealed class SequenceBuffer<T> where T : class, new()
    {
        readonly int m_Size;
        readonly int[] m_Indexes;
        readonly T[] m_Packets;

        public SequenceBuffer (int bufferSize)
        {
            m_Size = bufferSize;
            m_Indexes = new int[bufferSize];
            Array.Fill (m_Indexes, -1);
            m_Packets = new T[bufferSize];
            Array.Fill (m_Packets, new T ());
        }

        public int GetSize () { return m_Size; }

        public T Insert (int sequence)
        {
            int index = sequence % m_Size;
            m_Indexes[index] = sequence;
            return m_Packets[index];
        }

        public void Remove (int sequence)
        {
            int index = sequence % m_Size;
            if (m_Indexes[index] == sequence)
            {
                m_Indexes[index] = -1;
            }
        }

        public void Clear ()
        {
            Array.Fill (m_Indexes, -1);
        }

        public T TryGet (int sequence)
        {
            int index = sequence % m_Size;
            if (m_Indexes[index] == sequence)
            {
                return m_Packets[index];
            }
            else
            {
                return null;
            }
        }

        public T TryGetByIndex (int index, ref int sequence)
        {
            if (m_Indexes[index] != -1)
            {
                sequence = m_Indexes[index];
                return m_Packets[index];
            }
            else
            {
                return null;
            }
        }

        public bool IsExist (int sequence)
        {
            int index = sequence % m_Size;
            return m_Indexes[index] == sequence;
        }
    }
}
