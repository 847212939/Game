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
        public string Data
        {
            get => m_StringBuilder.ToString();
        }
        public int Length
        {
            get => m_StringBuilder.Length;
        }
        public void Write<T>(T value) 
        {
            m_StringBuilder.Append(value);
            m_StringBuilder.Append('\n');
        }
        public T Read<T>()
        {
            Type t = typeof(T);
            if (t == typeof(float))
            {
                return (T)(ReadFloat());
            }
            if (t == typeof(double))
            {
                return (T)(ReadDouble());
            }
            if (t == typeof(string))
            {
                return (T)(ReadString());
            }
            if (t == typeof(short))
            {
                return (T)(ReadInt16());
            }
            if (t == typeof(int))
            {
                return (T)(ReadInt32());
            }
            if (t == typeof(long))
            {
                return (T)(ReadInt64());
            }
            if (t == typeof(ushort))
            {
                return (T)(ReadUInt16());
            }
            if (t == typeof(uint))
            {
                return (T)(ReadUInt32());
            }
            if (t == typeof(ulong))
            {
                return (T)(ReadUInt64());
            }
            if (t == typeof(char))
            {
                return (T)(ReadChar());
            }
            else
            {
                return (T)(ReadString());
            }
        }
        public object ReadFloat()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return float.Parse(m_dataArray[m_cnt++]);
        }
        public object ReadDouble()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return double.Parse(m_dataArray[m_cnt++]);
        }
        public object ReadString()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return "";
            }
            return m_dataArray[m_cnt++];
        }
        public object ReadInt16()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return short.Parse(m_dataArray[m_cnt++]);
        }
        public object ReadInt32()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return int.Parse(m_dataArray[m_cnt++]);
        }
        public object ReadInt64()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return long.Parse(m_dataArray[m_cnt++]);
        }
        public object ReadUInt16()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return ushort.Parse(m_dataArray[m_cnt++]);
        }
        public object ReadUInt32()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return uint.Parse(m_dataArray[m_cnt++]);
        }
        public object ReadUInt64()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return 0;
            }

            return ulong.Parse(m_dataArray[m_cnt++]);
        }
        public object ReadChar()
        {
            if (m_dataArray.Length < m_cnt)
            {
                return '0';
            }

            return char.Parse(m_dataArray[m_cnt++]);
        }
    }
}
