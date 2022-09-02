using System.Text;

namespace Client.Util
{
    internal class Netmsg
    {
        int m_cnt;
        string[] m_dataArray = null!;
        StringBuilder m_StringBuilder = null!;

        public Netmsg()
        {
            m_cnt = 0;
            m_StringBuilder = new StringBuilder();
        }
        public Netmsg(string data)
        {
            m_cnt = 0;
            m_dataArray = data.Split("\n");
        }
        ~Netmsg()
        {
        }
        public void Write<T>(T value) 
        {
            m_StringBuilder.Append(value);
            m_StringBuilder.Append('\n');
        }
        public float ReadFloat()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return float.Parse(m_dataArray[m_cnt++]);
        }
        public double ReadDouble()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return double.Parse(m_dataArray[m_cnt++]);
        }
        public string ReadString()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return "";
            }
            return m_dataArray[m_cnt++];
        }
        public short ReadInt16()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return short.Parse(m_dataArray[m_cnt++]);
        }
        public int ReadInt32()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return int.Parse(m_dataArray[m_cnt++]);
        }
        public long ReadInt64()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return long.Parse(m_dataArray[m_cnt++]);
        }
        public ushort ReadUInt16()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return ushort.Parse(m_dataArray[m_cnt++]);
        }
        public uint ReadUInt32()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return uint.Parse(m_dataArray[m_cnt++]);
        }
        public ulong ReadUInt64()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return ulong.Parse(m_dataArray[m_cnt++]);
        }
        public char ReadChar()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return '0';
            }

            return char.Parse(m_dataArray[m_cnt++]);
        }
        public string Data
        {
            get => m_StringBuilder.ToString();
        }
        public int Length
        {
            get => m_StringBuilder.Length;
        }
    }
}
