namespace Client.Util
{
    internal class Netmsg
    {
        public Netmsg()
        {
            m_data = "";
        }
        public Netmsg(string data)
        {
            m_data = data;
        }
        ~Netmsg()
        {
            m_data = "";
        }
        public void WriteChar(char value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteInt16(short value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteInt32(int value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteInt64(long value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteUInt16(ushort value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteUInt32(uint value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteUInt64(ulong value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteFloat(float value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteDouble(double value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteString(string value)
        {
            m_data += value + "\n";
        }
        public float ReadFloat()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return float.Parse(str);
        }
        public double ReadDouble()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return double.Parse(str);
        }
        public string ReadString()
        {
            return GetSubString();
        }
        public short ReadInt16()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return short.Parse(str);
        }
        public int ReadInt32()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return int.Parse(str);
        }
        public long ReadInt64()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return long.Parse(str);
        }
        public ushort ReadUInt16()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return ushort.Parse(str);
        }
        public uint ReadUInt32()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return uint.Parse(str);
        }
        public ulong ReadUInt64()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return ulong.Parse(str);
        }
        public char ReadChar()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return '0';
            }

            return char.Parse(str);
        }
        public string Data
        {
            get => m_data;
        }
        public int Length
        {
            get => m_data.Length;
        }
        private string GetSubString()
        {
            if (Length <= 0)
            {
                return "";
            }
            int cnt = m_data.IndexOf("\n");
            if (cnt <= 0)
            {
                return "";
            }
            string sub = m_data.Substring(0, cnt);

            ++cnt;
            m_data = m_data.Substring(cnt, m_data.Length - cnt);

            return sub;
        }

        string m_data;
    }
}
