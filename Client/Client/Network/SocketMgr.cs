using System.Runtime.InteropServices;
using Client.Util;

namespace Client.Network
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public delegate void CBTimerHandle(int timer);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public delegate void CBEventHandle(REvent eve);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public delegate void CBCloseHandle();

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct REvent
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2048)]
        public string data;
    };

    internal class SocketMgr
    {
        private static SocketMgr    m_SocketMgr = null!;
        private NetworkMgr          m_NetworkMgr;
        private CBTimerHandle       m_CBTimerHandle;
        private CBEventHandle       m_CallBackFunc;
        private CBCloseHandle       m_CBCloseHandle;
        private int                 m_TimerCnt;
        private bool                m_Connected;

        [DllImport("Cxxdll", CharSet = CharSet.Ansi)]
        private extern static int InitNetwork(string ip, int port, int timerCnt);
        [DllImport("Cxxdll", CharSet = CharSet.Ansi)]
        private extern static int InitCxxnet(CBEventHandle netFunc, CBTimerHandle timerFunc, CBCloseHandle closeFunc);
        [DllImport("Cxxdll", CharSet = CharSet.Ansi)]
        private extern static int RegisterTimers(int timerid, int uElapse);
        [DllImport("Cxxdll", CharSet = CharSet.Ansi)]
        private extern static int UnRegisterTimers(int timerid);
        [DllImport("Cxxdll", CharSet = CharSet.Ansi)]
        private extern static int SendData(string pData, int size, int mainID, int assistID, int uIdentification);
        [DllImport("Cxxdll", CharSet = CharSet.Ansi)]
        private extern static int UnInitCxxnet();

        public bool GetConnected()
        {
            return m_Connected;
        }

        public static SocketMgr GetInstance()
        {
            if (m_SocketMgr == null)
            {
                m_SocketMgr = new SocketMgr();
            }

            return m_SocketMgr;
        }

        private SocketMgr()
        {
            m_TimerCnt = 0;
            m_Connected = false;
            m_NetworkMgr = new NetworkMgr();
            m_CallBackFunc = new CBEventHandle(NetworkCallBackFunc);
            m_CBCloseHandle = new CBCloseHandle(CloseCallBackFunc);
            m_CBTimerHandle = new CBTimerHandle(TimerCallBackFunc);
        }

        public int SendMsg(string pData, int size, MsgCmd mainID, int assistID, MsgCmd uIdentification)
        {
            if (!m_Connected)
            {
                return -1;
            }
            if (mainID <= MsgCmd.MsgCmd_Begin ||
                mainID >= MsgCmd.MsgCmd_End ||
                uIdentification <= MsgCmd.MsgCmd_Begin ||
                uIdentification >= MsgCmd.MsgCmd_End)
            {
                return -1;
            }

            return SendData(pData, size, (int)mainID, assistID, (int)uIdentification);
        }

        public int InitSocket(string ip, int port, int timerCnt)
        {
            if (m_Connected)
            {
                return -1;
            }
            if (InitNetwork(ip, port, timerCnt) != 0)
            {
                return -1;
            }
            if (InitCxxnet(m_CallBackFunc, m_CBTimerHandle, m_CBCloseHandle) != 0)
            {
                return -1;
            }
            m_TimerCnt = timerCnt;

            m_Connected = true;
            return 0;
        }

        public int UnInitSocket()
        {
            return UnInitCxxnet();
        }

        public void RegisterNetwork(MsgCmd cmd, Action<NetWorkMsg> ac)
        {
            if (!m_Connected)
            {
                return;
            }
            if (cmd <= MsgCmd.MsgCmd_Begin || 
                cmd >= MsgCmd.MsgCmd_End)
            {
                return;
            }
            m_NetworkMgr.AddNetworkDictionary((UInt32)cmd, ac);
        }

        public void UnRegisterNetwork(MsgCmd cmd)
        {
            if (!m_Connected)
            {
                return;
            }
            if (cmd <= MsgCmd.MsgCmd_Begin || 
                cmd >= MsgCmd.MsgCmd_End)
            {
                return;
            }
            m_NetworkMgr.DelNetworkDictionary((UInt32)cmd);
        }

        public int RegisterTimer(TimerCmd timerid, int uElapse, Action<int> ac)
        {
            if (!m_Connected)
            {
                return -1;
            }
            if (timerid <= TimerCmd.TimerCmd_Begin || 
                timerid >= TimerCmd.TimerCmd_End ||
                m_TimerCnt <= 0)
            {
                return -1;
            }
            if (m_NetworkMgr.AddTimerDictionary((int)timerid, ac))
            {
                return RegisterTimers((int)timerid, uElapse);
            }

            return 0;
        }

        public int UnRegisterTimer(TimerCmd timerid)
        {
            if (!m_Connected)
            {
                return -1;
            }
            if (timerid <= TimerCmd.TimerCmd_Begin || 
                timerid >= TimerCmd.TimerCmd_End ||
                m_TimerCnt <= 0)
            {
                return -1;
            }
            if (UnRegisterTimers((int)timerid) != 0)
            {
                return -1;
            }
            if (!m_NetworkMgr.DelTimerDictionary((int)timerid))
            {
                return -1;
            }

            return 0;
        }

        private void NetworkCallBackFunc(REvent eve)
        {
            if (!m_Connected)
            {
                return;
            }
            Netmsg cout = new Netmsg(eve.data);
            NetWorkMsg msg = new NetWorkMsg();

            msg.uMainID = cout.ReadUInt32();
            if ((MsgCmd)msg.uMainID <= MsgCmd.MsgCmd_Begin ||
                (MsgCmd)msg.uMainID >= MsgCmd.MsgCmd_End)
            {
                return;
            }
            msg.uAssistantID = cout.ReadUInt32();
            msg.uIdentification = cout.ReadUInt32();
            msg.uMessageSize = cout.ReadUInt32();
            msg.data = cout.ReadString();
            m_NetworkMgr.MessageDispatch(msg);
        }

        private void TimerCallBackFunc(int timer)
        {
            if (!m_Connected)
            {
                return;
            }
            if ((TimerCmd)timer <= TimerCmd.TimerCmd_Begin ||
                (TimerCmd)timer >= TimerCmd.TimerCmd_End)
            {
                return;
            }
            m_NetworkMgr.TimerDispatch(timer);
        }

        private void CloseCallBackFunc()
        {
            m_Connected = false;
        }
    }
}
