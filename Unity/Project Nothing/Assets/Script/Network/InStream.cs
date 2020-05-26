using System;
using System.Collections.Generic;
using System.Net;

namespace ProjectNothing.Network
{
    public sealed class InStream
    {
        private readonly byte[] m_Data;
        private int m_Index;

        public InStream (byte[] bytes)
        {
            m_Data = bytes;
            m_Index = 0;
        }

        public InStream (byte[] bytes, int length)
        {
            m_Data = new byte[length];
            Array.Copy (bytes, m_Data, length);
            m_Index = 0;
        }

        public byte[] Data ()
        {
            return m_Data;
        }

        public InStream ReadBool (out bool b)
        {
            b = BitConverter.ToBoolean (m_Data, m_Index);
            m_Index += sizeof (bool);
            return this;
        }

        public InStream ReadChar (out char c)
        {
            c = Convert.ToChar (m_Data[m_Index]);
            m_Index += sizeof (byte);
            return this;
        }

        public InStream ReadUnsignedChar (out byte b)
        {
            b = m_Data[m_Index];
            m_Index += sizeof (byte);
            return this;
        }

        public InStream ReadShort (out short s)
        {
            s = BitConverter.ToInt16 (m_Data, m_Index);
            s = IPAddress.NetworkToHostOrder (s);
            m_Index += sizeof (short);
            return this;
        }

        public InStream ReadUnsignedShort (out ushort us)
        {
            short s = IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (m_Data, m_Index));
            us = BitConverter.ToUInt16 (BitConverter.GetBytes (s), 0);
            m_Index += sizeof (ushort);
            return this;
        }

        public InStream ReadInt (out int i)
        {
            i = BitConverter.ToInt32 (m_Data, m_Index);
            i = IPAddress.NetworkToHostOrder (i);
            m_Index += sizeof (int);
            return this;
        }

        public InStream ReadUnsignedInt (out uint ui)
        {
            int i = IPAddress.NetworkToHostOrder (BitConverter.ToInt32 (m_Data, m_Index));
            ui = BitConverter.ToUInt32 (BitConverter.GetBytes (i), 0);
            m_Index += sizeof (uint);
            return this;
        }

        public InStream ReadLong (out long l)
        {
            l = IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (m_Data, m_Index));
            m_Index += sizeof (long);
            return this;
        }

        public InStream ReadUnsignedLong (out ulong ul)
        {
            long l = IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (m_Data, m_Index));
            ul = BitConverter.ToUInt64 (BitConverter.GetBytes (l), 0);
            m_Index += sizeof (ulong);
            return this;
        }

        public InStream ReadFloat (out float f)
        {
            int i = IPAddress.NetworkToHostOrder (BitConverter.ToInt32 (m_Data, m_Index));
            f = BitConverter.ToSingle (BitConverter.GetBytes (i), 0);
            m_Index += sizeof (float);
            return this;
        }

        public InStream ReadDouble (out double d)
        {
            long l = IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (m_Data, m_Index));
            d = BitConverter.ToDouble (BitConverter.GetBytes (l), 0);
            m_Index += sizeof (double);
            return this;
        }

        public InStream ReadString (out string s)
        {
            List<char> letters = new List<char> ();

            int size;
            ReadInt (out size);
            for (int i = 0; i < size; i++)
            {
                char c;
                ReadChar (out c);
                letters.Add (c);
            }

            s = new string (letters.ToArray ());
            return this;
        }
    }
}
