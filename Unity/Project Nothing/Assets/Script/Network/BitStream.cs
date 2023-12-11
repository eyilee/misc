using System;
using System.Collections.Generic;
using System.Net;

namespace ProjectNothing.Network
{
    public class BitInStream
    {
        private readonly byte[] m_Bytes;
        private int m_BitOffset;

        public BitInStream (byte[] _bytes)
        {
            m_Bytes = _bytes;
            m_BitOffset = 0;
        }

        public BitInStream (byte[] _bytes, int _length)
        {
            m_Bytes = new byte[_length];
            Array.Copy (_bytes, m_Bytes, _length);
            m_BitOffset = 0;
        }

        public void Read (out bool _outBool)
        {
            if (m_BitOffset + 1 > m_Bytes.Length * 8)
            {
                _outBool = default;
                return;
            }

            ReadBit (out _outBool);
        }

        public void Read (out sbyte _outSByte)
        {
            if (m_BitOffset + sizeof (sbyte) * 8 > m_Bytes.Length * 8)
            {
                _outSByte = default;
                return;
            }

            ReadByte (out byte outByte);
            _outSByte = unchecked((sbyte)outByte);
        }

        public void Read (out byte _outByte)
        {
            if (m_BitOffset + sizeof (byte) * 8 > m_Bytes.Length * 8)
            {
                _outByte = default;
                return;
            }

            ReadByte (out _outByte);
        }

        public void Read (out char _outChar)
        {
            int byteCount = sizeof (char);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                _outChar = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            _outChar = BitConverter.ToChar (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (bytes))));
        }

        public void Read (out short _outShort)
        {
            int byteCount = sizeof (short);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                _outShort = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            _outShort = BitConverter.ToInt16 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (bytes))));
        }

        public void Read (out ushort _outUShort)
        {
            int byteCount = sizeof (ushort);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                _outUShort = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            _outUShort = BitConverter.ToUInt16 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (bytes))));
        }

        public void Read (out int _outInt)
        {
            int byteCount = sizeof (int);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                _outInt = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            _outInt = BitConverter.ToInt32 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt32 (bytes))));
        }

        public void Read (out uint _outUInt)
        {
            int byteCount = sizeof (uint);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                _outUInt = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            _outUInt = BitConverter.ToUInt32 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt32 (bytes))));
        }

        public void Read (out long _outLong)
        {
            int byteCount = sizeof (long);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                _outLong = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            _outLong = BitConverter.ToInt64 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (bytes))));
        }

        public void Read (out ulong _outULong)
        {
            int byteCount = sizeof (ulong);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                _outULong = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            _outULong = BitConverter.ToUInt64 (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (bytes))));
        }

        public void Read (out float _outFloat)
        {
            int byteCount = sizeof (float);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                _outFloat = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            _outFloat = BitConverter.ToSingle (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt32 (bytes))));
        }

        public void Read (out double _outDouble)
        {
            int byteCount = sizeof (double);
            if (m_BitOffset + byteCount * 8 > m_Bytes.Length * 8)
            {
                _outDouble = default;
                return;
            }

            byte[] bytes = new byte[byteCount];
            for (int i = 0; i < byteCount; i++)
            {
                ReadByte (out bytes[i]);
            }

            _outDouble = BitConverter.ToDouble (BitConverter.GetBytes (IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (bytes))));
        }

        public void Read (out string _outString)
        {
            Read (out ushort size);

            List<char> list = new (size);
            for (ushort i = 0; i < size; i++)
            {
                Read (out char c);
                list.Add (c);
            }

            _outString = new string (list.ToArray ());
        }

        private void ReadByte (out byte _outByte)
        {
            int byteOffset = m_BitOffset >> 3;
            int bitOffset = m_BitOffset & 0x7;

            _outByte = unchecked((byte)(m_Bytes[byteOffset] << bitOffset));
            if (bitOffset > 0)
            {
                _outByte |= unchecked((byte)(m_Bytes[byteOffset + 1] >> (8 - bitOffset)));
            }

            m_BitOffset += 8;
        }

        private void ReadBit (out bool _outBit)
        {
            int byteOffset = m_BitOffset >> 3;
            int bitOffset = m_BitOffset & 0x7;

            _outBit = (m_Bytes[byteOffset] & (1 << (7 - bitOffset))) != 0;

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

        public byte[] GetBytes ()
        {
            return m_Bytes.ToArray ();
        }

        public void Write (bool _bool)
        {
            WriteBit (ref _bool);
        }

        public void Write (sbyte _sbyte)
        {
            byte refByte = unchecked((byte)_sbyte);
            WriteByte (ref refByte);
        }

        public void Write (byte _byte)
        {
            WriteByte (ref _byte);
        }

        public void Write (char _char)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt16 (BitConverter.GetBytes (_char))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (short _short)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt16 (BitConverter.GetBytes (_short))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (ushort _ushort)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt16 (BitConverter.GetBytes (_ushort))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (int _int)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt32 (BitConverter.GetBytes (_int))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (uint _uint)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt32 (BitConverter.GetBytes (_uint))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (long _long)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt64 (BitConverter.GetBytes (_long))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (ulong _ulong)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt64 (BitConverter.GetBytes (_ulong))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (float _float)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt32 (BitConverter.GetBytes (_float))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (double _double)
        {
            byte[] bytes = BitConverter.GetBytes (IPAddress.HostToNetworkOrder (BitConverter.ToInt64 (BitConverter.GetBytes (_double))));
            for (int i = 0; i < bytes.Length; i++)
            {
                WriteByte (ref bytes[i]);
            }
        }

        public void Write (string _string)
        {
            ushort size = unchecked((ushort)_string.Length);

            Write (size);

            for (int i = 0; i < size; i++)
            {
                Write (_string[i]);
            }
        }

        private void WriteByte (ref byte _refByte)
        {
            int bitOffset = m_BitOffset & 0x7;

            if (bitOffset == 0)
            {
                m_Bytes.Add (_refByte);
            }
            else
            {
                m_Bytes[^1] |= unchecked((byte)(_refByte >> bitOffset));
                m_Bytes.Add (unchecked((byte)(_refByte << (8 - bitOffset))));
            }

            m_BitOffset += 8;
        }

        private void WriteBit (ref bool _refBit)
        {
            int bitOffset = m_BitOffset & 0x7;

            byte value = unchecked((byte)((_refBit ? 1 : 0) << (7 - bitOffset)));

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
