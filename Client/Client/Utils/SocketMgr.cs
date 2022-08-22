using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Client.Utils
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public delegate void CBTimerHandle(int timer);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public delegate void CBEventHandle(REvent eve/*, [MarshalAs(UnmanagedType.LPStr)]StringBuilder source*/);

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct REvent
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2048)]
        public string data;
    };

    internal class SocketMgr
    {
        private NetworkMgr      m_NetworkMgr;
        private CBTimerHandle   m_CBTimerHandle;
        private CBEventHandle   m_CallBackFunc;

        [DllImport("Cxxdll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void InitNetwork(string ip, int port, int timerCnt);
        [DllImport("Cxxdll", CallingConvention = CallingConvention.Cdecl)]
        public extern static bool InitCxxnet(CBEventHandle netFunc, CBTimerHandle timerFunc);
        [DllImport("Cxxdll", CallingConvention = CallingConvention.Cdecl)]
        extern static void RegisterTimers(int timerid, int uElapse);
        [DllImport("Cxxdll", CallingConvention = CallingConvention.Cdecl)]
        extern static void UnRegisterTimers(int timerid);
        [DllImport("Cxxdll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void SendData(string pData, int size, int mainID, int assistID, int uIdentification);
        
        public SocketMgr()
        {
            m_NetworkMgr = new NetworkMgr();
            m_CallBackFunc = new CBEventHandle(NetworkCallBackFunc);
            m_CBTimerHandle = new CBTimerHandle(TimerCallBackFunc);
        }
        
        public bool InitSocket(string ip, int port, int timerCnt)
        {
            InitNetwork(ip, port, timerCnt);
            if (!InitCxxnet(m_CallBackFunc, m_CBTimerHandle))
            {
                return false;
            }
            return true;
        }

        public void RegisterTimer(int timerid, int uElapse)
        {
            RegisterTimers(timerid, uElapse);
        }

        public void UnRegisterTimer(int timerid)
        {
            UnRegisterTimers(timerid);
        }

        public NetworkMgr GetNetworkMgr()
        {
            return m_NetworkMgr;
        }

        private void NetworkCallBackFunc(REvent eve)
        {
            Util cis = new Util(eve.data);
            NetWorkMsg msg = new NetWorkMsg();

            msg.uMainID = cis.ReadUInt32();
            msg.uAssistantID = cis.ReadUInt32();
            msg.uIdentification = cis.ReadUInt32();
            msg.uMessageSize = cis.ReadUInt32();
            msg.data = cis.ReadString();

            m_NetworkMgr.MessageDispatch(msg);
        }

        private void TimerCallBackFunc(int timer)
        {
            m_NetworkMgr.TimerDispatch(timer);
        }
    }
}
