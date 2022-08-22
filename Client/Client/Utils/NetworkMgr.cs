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

        private Dictionary<int, Action<int>> m_TimerDictionary;
        private Dictionary<UInt32, Action<NetWorkMsg>> m_NetworkDictionary;

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
            Console.WriteLine("网络消息回调成功");
        }

        public void TimerDispatch(int timer)
        {
            if (m_TimerDictionary.ContainsKey(timer))
            {
                m_TimerDictionary[timer](timer);
            }
            Console.WriteLine("定时器回调成功");
        }

        public Dictionary<UInt32, Action<NetWorkMsg>> GetNetworkDictionary()
        {
            return m_NetworkDictionary;
        }

        public Dictionary<int, Action<int>> GetTimerDictionary()
        {
            return m_TimerDictionary;
        }
    }
}
