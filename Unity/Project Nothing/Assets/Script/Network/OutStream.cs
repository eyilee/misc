using System;
using System.Collections.Generic;
using System.Net;

namespace ProjectNothing.Network
{
    public sealed class OutStream
    {
        private readonly List<byte> m_Data;

        public OutStream ()
        {
            m_Data = new List<byte> ();
        }

        public byte[] Data ()
        {
            return m_Data.ToArray ();
        }

        public OutStream WriteBool (bool b)
        {
            m_Data.AddRange (BitConverter.GetBytes (b));
            return this;
        }

        public OutStream WriteChar (char c)
        {
            m_Data.Add (Convert.ToByte (c));
            return this;
        }

        public OutStream WriteUnsignedChar (byte b)
        {
            m_Data.AddRange (BitConverter.GetBytes (b));
            return this;
        }

        public OutStream WriteShort (short s)
        {
            s = IPAddress.NetworkToHostOrder (s);
            m_Data.AddRange (BitConverter.GetBytes (s));
            return this;
        }

        public OutStream WriteUnsignedShort (ushort us)
        {
            short s = BitConverter.ToInt16 (BitConverter.GetBytes (us), 0);
            s = IPAddress.NetworkToHostOrder (s);
            us = BitConverter.ToUInt16 (BitConverter.GetBytes (s), 0);
            m_Data.AddRange (BitConverter.GetBytes (us));
            return this;
        }

        public OutStream WriteInt (int i)
        {
            i = IPAddress.NetworkToHostOrder (i);
            m_Data.AddRange (BitConverter.GetBytes (i));
            return this;
        }

        public OutStream WriteUnsignedInt (uint ui)
        {
            int i = BitConverter.ToInt32 (BitConverter.GetBytes (ui), 0);
            i = IPAddress.NetworkToHostOrder (i);
            ui = BitConverter.ToUInt32 (BitConverter.GetBytes (i), 0);
            m_Data.AddRange (BitConverter.GetBytes (ui));
            return this;
        }

        public OutStream WriteLong (long l)
        {
            l = IPAddress.NetworkToHostOrder (l);
            m_Data.AddRange (BitConverter.GetBytes (l));
            return this;
        }

        public OutStream WriteUnsignedLong (ulong ul)
        {
            long l = BitConverter.ToInt64 (BitConverter.GetBytes (ul), 0);
            l = IPAddress.NetworkToHostOrder (l);
            ul = BitConverter.ToUInt64 (BitConverter.GetBytes (l), 0);
            m_Data.AddRange (BitConverter.GetBytes (ul));
            return this;
        }

        public OutStream WriteFloat (float f)
        {
            int i = BitConverter.ToInt32 (BitConverter.GetBytes (f), 0);
            i = IPAddress.HostToNetworkOrder (i);
            f = BitConverter.ToSingle (BitConverter.GetBytes (i), 0);
            m_Data.AddRange (BitConverter.GetBytes (f));
            return this;
        }

        public OutStream WriteDouble (double d)
        {
            long l = BitConverter.ToInt64 (BitConverter.GetBytes (d), 0);
            l = IPAddress.HostToNetworkOrder (l);
            d = BitConverter.ToDouble (BitConverter.GetBytes (l), 0);
            m_Data.AddRange (BitConverter.GetBytes (d));
            return this;
        }

        public OutStream WriteString (string s)
        {
            int size = s.Length;
            WriteInt (size);
            for (int i = 0; i < size; i++)
            {
                WriteChar (s[i]);
            }
            return this;
        }
    }
}
