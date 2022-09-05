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
                Type type = typeof(T);
                if (type == typeof(int))
                {
                    return (T)(int.Parse(m_dataArray[m_cnt++]) as object);
                }
                else if (type == typeof(uint))
                {
                    return (T)(uint.Parse(m_dataArray[m_cnt++]) as object);
                }
                else if (type == typeof(char))
                {
                    return (T)(char.Parse(m_dataArray[m_cnt++]) as object);
                }
                else if (type == typeof(short))
                {
                    return (T)(short.Parse(m_dataArray[m_cnt++]) as object);
                }
                else if (type == typeof(ushort))
                {
                    return (T)(ushort.Parse(m_dataArray[m_cnt++]) as object);
                }
                else if (type == typeof(string))
                {
                    return (T)(m_dataArray[m_cnt++] as object);
                }
                else if (type == typeof(long))
                {
                    return (T)(long.Parse(m_dataArray[m_cnt++]) as object);
                }
                else if (type == typeof(ulong))
                {
                    return (T)(ulong.Parse(m_dataArray[m_cnt++]) as object);
                }
                else if (type == typeof(float))
                {
                    return (T)(float.Parse(m_dataArray[m_cnt++]) as object);
                }
                else if (type == typeof(double))
                {
                    return (T)(double.Parse(m_dataArray[m_cnt++]) as object);
                }
            }

            return (T)(0 as object);
        }
    }
}
