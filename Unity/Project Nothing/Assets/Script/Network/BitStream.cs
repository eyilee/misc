using System;
using System.Collections.Generic;
using System.Net;

namespace ProjectNothing
{
    public class BitInStream
    {
        private readonly byte[] m_Bytes;
        private int m_BitOffset;

        public BitInStream (byte[] bytes)
        {
            m_Bytes = new byte[bytes.Length];
            bytes.CopyTo (m_Bytes, 0);
            m_BitOffset = 0;
        }

        public void Read (out bool outValue)
        {
            if (m_BitOffset + 1 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            ReadBit (out outValue);
        }

        public void Read (out sbyte outValue)
        {
            if (m_BitOffset + sizeof (sbyte) * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            ReadByte (out byte outByte);
            outValue = unchecked((sbyte)outByte);
        }

        public void Read (out byte outValue)
        {
            if (m_BitOffset + sizeof (byte) * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            ReadByte (out outValue);
        }

        public void Read (out char outValue)
        {
            int byteCount = sizeof (char);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            outValue = BitConverter.ToChar (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (bytes))));
        }

        public void Read (out short outValue)
        {
            int byteCount = sizeof (short);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            outValue = BitConverter.ToInt16 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (bytes))));
        }

        public void Read (out ushort outValue)
        {
            int byteCount = sizeof (ushort);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            outValue = BitConverter.ToUInt16 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (bytes))));
        }

        public void Read (out int outValue)
        {
            int byteCount = sizeof (int);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            outValue = BitConverter.ToInt32 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt32 (bytes))));
        }

        public void Read (out uint outValue)
        {
            int byteCount = sizeof (uint);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            outValue = BitConverter.ToUInt32 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt32 (bytes))));
        }

        public void Read (out long outValue)
        {
            int byteCount = sizeof (long);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            outValue = BitConverter.ToInt64 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (bytes))));
        }

        public void Read (out ulong outValue)
        {
            int byteCount = sizeof (ulong);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            outValue = BitConverter.ToUInt64 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (bytes))));
        }

        public void Read (out float outValue)
        {
            int byteCount = sizeof (float);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            outValue = BitConverter.ToSingle (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt32 (bytes))));
        }

        public void Read (out double outValue)
        {
            int byteCount = sizeof (double);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                outValue = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            outValue = BitConverter.ToDouble (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (bytes))));
        }

        public void Read (out string outValue)
        {
            Read (out ushort size);

            List<char> list = new (size);
            for (ushort i = 0; i < size; i++)
            {
                Read (out char c);
                list.Add (c);
            }

            outValue = new string (list.ToArray ());
        }

        private void ReadByte (out byte outValue)
        {
            int byteOffset = m_BitOffset >> 3;
            int bitOffset = m_BitOffset & 0x7;

            outValue = unchecked((byte)(m_Bytes[byteOffset] << bitOffset));
            if (bitOffset > 0)
            {
                outValue |= unchecked((byte)(m_Bytes[byteOffset + 1] >> (8 - bitOffset)));
            }

            m_BitOffset += 8;
        }

        private void ReadBit (out bool outValue)
        {
            int byteOffset = m_BitOffset >> 3;
            int bitOffset = m_BitOffset & 0x7;

            outValue = (m_Bytes[byteOffset] & (1 << (7 - bitOffset))) != 0;

            m_BitOffset += 1;
        }
    }

    public class BitOutStream
    {
        private readonly List<byte> m_Bytes;
        private int m_BitOffset;

        public BitOutStream ()
        {
            m_Bytes = new List<byte> ();
            m_BitOffset = 0;
        }

        public byte[] GetHeader ()
        {
            byte[] bytes = BitConverter.GetBytes (Convert.ToUInt16 (m_Bytes.Count));
            return BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt16 (bytes)));
        }

        public byte[] GetBytes ()
        {
            return m_Bytes.ToArray ();
        }

        public int GetSize ()
        {
            return m_Bytes.Count;
        }

        public void Write (bool value)
        {
            WriteBit (ref value);
        }

        public void Write (sbyte value)
        {
            byte refByte = unchecked((byte)value);
            WriteByte (ref refByte);
        }

        public void Write (byte value)
        {
            WriteByte (ref value);
        }

        public void Write (char value)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt16 (BitConverter.GetBytes (value))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (short value)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt16 (BitConverter.GetBytes (value))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (ushort value)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt16 (BitConverter.GetBytes (value))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (int value)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt32 (BitConverter.GetBytes (value))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (uint value)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt32 (BitConverter.GetBytes (value))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (long value)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt64 (BitConverter.GetBytes (value))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (ulong value)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt64 (BitConverter.GetBytes (value))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (float value)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt32 (BitConverter.GetBytes (value))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (double value)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt64 (BitConverter.GetBytes (value))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (string value)
        {
            ushort size = unchecked((ushort)value.Length);

            Write (size);

            for (int i = 0; i < size; i++)
            {
                Write (value[i]);
            }
        }

        private void WriteByte (ref byte refValue)
        {
            int bitOffset = m_BitOffset & 0x7;

            if (bitOffset == 0)
            {
                m_Bytes.Add (refValue);
            }
            else
            {
                m_Bytes[^1] |= unchecked((byte)(refValue >> bitOffset));
                m_Bytes.Add (unchecked((byte)(refValue << (8 - bitOffset))));
            }

            m_BitOffset += 8;
        }

        private void WriteBit (ref bool refValue)
        {
            int bitOffset = m_BitOffset & 0x7;

            byte value = unchecked((byte)((refValue ? 1 : 0) << (7 - bitOffset)));

            if (bitOffset == 0)
            {
                m_Bytes.Add (value);
            }
            else
            {
                m_Bytes[^1] |= value;
            }

            m_BitOffset += 1;
        }
    }
}
