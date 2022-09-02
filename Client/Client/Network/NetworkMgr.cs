namespace Client.Network
{
    public struct NetWorkMsg
    {
        public uint uMainID;              // 处理主类型
        public uint uAssistantID;         // 辅助处理类型 ID
        public uint uIdentification;      // 身份标识（不同的协议里面有不同的含义）
        public uint uMessageSize;		    // 数据包大小

        public string data;                 // 数据
    }

    internal class NetworkMgr
    {
        private Dictionary<int, Action<int>>            m_TimerDictionary;
        private Dictionary<uint, Action<NetWorkMsg>>  m_NetworkDictionary;

        public NetworkMgr()
        {
            m_TimerDictionary = new Dictionary<int, Action<int>>();
            m_NetworkDictionary = new Dictionary<uint, Action<NetWorkMsg>>();
        }

        public void MessageDispatch(NetWorkMsg msg)
        {
            if (m_NetworkDictionary.ContainsKey(msg.uMainID))
            {
                m_NetworkDictionary[msg.uMainID](msg);
            }
        }

        public void TimerDispatch(int timer)
        {
            if (m_TimerDictionary.ContainsKey(timer))
            {
                m_TimerDictionary[timer](timer);
            }
        }

        public void AddNetworkDictionary(uint cmd, Action<NetWorkMsg> ac)
        {
            if (!m_NetworkDictionary.ContainsKey(cmd))
            {
                m_NetworkDictionary.Add(cmd, ac);
            }
        }

        public void DelNetworkDictionary(uint cmd)
        {
            if (m_NetworkDictionary.ContainsKey(cmd))
            {
                m_NetworkDictionary.Remove(cmd);
            }
        }

        public bool AddTimerDictionary(int timer, Action<int> ac)
        {
            if (!m_TimerDictionary.ContainsKey(timer))
            {
                m_TimerDictionary.Add(timer, ac);
                return true;
            }
            return false;
        }

        public bool DelTimerDictionary(int timer)
        {
            if (m_TimerDictionary.ContainsKey(timer))
            {
                m_TimerDictionary.Remove(timer);
                return true;
            }

            return false;
        }
    }
}
