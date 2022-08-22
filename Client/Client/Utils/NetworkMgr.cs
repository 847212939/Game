namespace Client.Utils
{
    public struct NetWorkMsg
    {
        public UInt32 uMainID;              // 处理主类型
        public UInt32 uAssistantID;         // 辅助处理类型 ID
        public UInt32 uIdentification;      // 身份标识（不同的协议里面有不同的含义）
        public UInt32 uMessageSize;		    // 数据包大小

        public string data;                 // 数据
    }

    internal class NetworkMgr
    {

        private Dictionary<int, Action<int>>            m_TimerDictionary;
        private Dictionary<UInt32, Action<NetWorkMsg>>  m_NetworkDictionary;

        public NetworkMgr()
        {
            m_TimerDictionary = new Dictionary<int, Action<int>>();
            m_NetworkDictionary = new Dictionary<UInt32, Action<NetWorkMsg>>();
        }

        public void MessageDispatch(NetWorkMsg msg)
        {
            if (m_NetworkDictionary.ContainsKey(msg.uMainID))
            {
                m_NetworkDictionary[msg.uMainID](msg);
            }
        }

        public void AddNetworkDictionary(UInt32 cmd, Action<NetWorkMsg> ac)
        {
            if (!m_NetworkDictionary.ContainsKey(cmd))
            {
                m_NetworkDictionary.Add(cmd, ac);
            }
        }

        public void DelNetworkDictionary(UInt32 cmd)
        {
            if (m_NetworkDictionary.ContainsKey(cmd))
            {
                m_NetworkDictionary.Remove(cmd);
            }
        }

        public void TimerDispatch(int timer)
        {
            if (m_TimerDictionary.ContainsKey(timer))
            {
                m_TimerDictionary[timer](timer);
            }
        }

        public void AddTimerDictionary(int timer, Action<int> ac)
        {
            if (!m_TimerDictionary.ContainsKey(timer))
            {
                m_TimerDictionary.Add(timer, ac);
            }
        }

        public void DelTimerDictionary(int timer)
        {
            if (m_TimerDictionary.ContainsKey(timer))
            {
                m_TimerDictionary.Remove(timer);
            }
        }
    }
}
