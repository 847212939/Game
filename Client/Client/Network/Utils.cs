namespace Client.Utils
{
    internal class Util
    {
        public Util()
        {
            m_data = "";
        }
        public Util(string data)
        {
            m_data = data;
        }
        ~Util()
        {
            m_data = "";
        }
        public void WriteChar(char value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteInt16(Int16 value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteInt32(Int32 value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteInt64(Int64 value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteUInt16(UInt16 value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteUInt32(UInt32 value)
        {
            m_data += value.ToString() + "\n";
        }
        public void WriteUInt64(UInt64 value)
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
        public Int16 ReadInt16()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return Int16.Parse(str);
        }
        public Int32 ReadInt32()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return Int32.Parse(str);
        }
        public Int64 ReadInt64()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return Int64.Parse(str);
        }
        public UInt16 ReadUInt16()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return UInt16.Parse(str);
        }
        public UInt32 ReadUInt32()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return UInt32.Parse(str);
        }
        public UInt64 ReadUInt64()
        {
            string str = GetSubString();
            if (str.Length <= 0)
            {
                return 0;
            }

            return UInt64.Parse(str);
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
