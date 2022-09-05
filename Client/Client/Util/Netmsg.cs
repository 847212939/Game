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
            m_cnt = 0;
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
            if (m_dataArray.Length > m_cnt)
            {
                if (typeof(T) == typeof(char)) return (T)((object)char.Parse(m_dataArray[m_cnt++]));
                else if (typeof(T) == typeof(short)) return (T)((object)short.Parse(m_dataArray[m_cnt++]));
                else if (typeof(T) == typeof(ushort)) return (T)((object)ushort.Parse(m_dataArray[m_cnt++]));
                else if (typeof(T) == typeof(int)) return (T)((object)int.Parse(m_dataArray[m_cnt++]));
                else if (typeof(T) == typeof(uint)) return (T)((object)uint.Parse(m_dataArray[m_cnt++]));
                else if (typeof(T) == typeof(string)) return (T)((object)m_dataArray[m_cnt++]);
                else if (typeof(T) == typeof(long)) return (T)((object)long.Parse(m_dataArray[m_cnt++]));
                else if (typeof(T) == typeof(ulong)) return (T)((object)ulong.Parse(m_dataArray[m_cnt++]));
                else if (typeof(T) == typeof(float)) return (T)((object)float.Parse(m_dataArray[m_cnt++]));
                else if (typeof(T) == typeof(double)) return (T)((object)double.Parse(m_dataArray[m_cnt++]));
            }

            return (T)((object)0);
        }
    }
}
