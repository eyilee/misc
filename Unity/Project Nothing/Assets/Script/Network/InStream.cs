using System;
using System.Collections.Generic;
using System.Net;

namespace ProjectNothing.Network
{
    sealed class InStream
    {
        private readonly byte[] data;
        private int index;

        public InStream (byte[] bytes)
        {
            data = bytes;
            index = 0;
        }

        public InStream (byte[] bytes, int length)
        {
            data = new byte[length];
            Array.Copy (bytes, data, length);
            index = 0;
        }

        public byte[] Data ()
        {
            return data;
        }

        public InStream ReadBool (out bool b)
        {
            b = BitConverter.ToBoolean (data, index);
            index += sizeof (bool);
            return this;
        }

        public InStream ReadChar (out char c)
        {
            c = Convert.ToChar (data[index]);
            index += sizeof (byte);
            return this;
        }

        public InStream ReadUnsignedChar (out byte b)
        {
            b = data[index];
            index += sizeof (byte);
            return this;
        }

        public InStream ReadShort (out short s)
        {
            s = BitConverter.ToInt16 (data, index);
            s = IPAddress.NetworkToHostOrder (s);
            index += sizeof (short);
            return this;
        }

        public InStream ReadUnsignedShort (out ushort us)
        {
            short s = IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (data, index));
            us = BitConverter.ToUInt16 (BitConverter.GetBytes (s), 0);
            index += sizeof (ushort);
            return this;
        }

        public InStream ReadInt (out int i)
        {
            i = BitConverter.ToInt32 (data, index);
            i = IPAddress.NetworkToHostOrder (i);
            index += sizeof (int);
            return this;
        }

        public InStream ReadUnsignedInt (out uint ui)
        {
            int i = IPAddress.NetworkToHostOrder (BitConverter.ToInt32 (data, index));
            ui = BitConverter.ToUInt32 (BitConverter.GetBytes (i), 0);
            index += sizeof (uint);
            return this;
        }

        public InStream ReadLong (out long l)
        {
            l = IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (data, index));
            index += sizeof (long);
            return this;
        }

        public InStream ReadUnsignedLong (out ulong ul)
        {
            long l = IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (data, index));
            ul = BitConverter.ToUInt64 (BitConverter.GetBytes (l), 0);
            index += sizeof (ulong);
            return this;
        }

        public InStream ReadFloat (out float f)
        {
            int i = IPAddress.NetworkToHostOrder (BitConverter.ToInt32 (data, index));
            f = BitConverter.ToSingle (BitConverter.GetBytes (i), 0);
            index += sizeof (float);
            return this;
        }

        public InStream ReadDouble (out double d)
        {
            long l = IPAddress.NetworkToHostOrder (BitConverter.ToInt64 (data, index));
            d = BitConverter.ToDouble (BitConverter.GetBytes (l), 0);
            index += sizeof (double);
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
