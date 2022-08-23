using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Client.Network
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
        private static SocketMgr    m_SocketMgr = null!;
        private NetworkMgr          m_NetworkMgr;
        private CBTimerHandle       m_CBTimerHandle;
        private CBEventHandle       m_CallBackFunc;
        private int                 m_TimerCnt;

        [DllImport("Cxxdll", CallingConvention = CallingConvention.Cdecl)]
        private extern static bool InitNetwork(string ip, int port, int timerCnt);
        [DllImport("Cxxdll", CallingConvention = CallingConvention.Cdecl)]
        private extern static bool InitCxxnet(CBEventHandle netFunc, CBTimerHandle timerFunc);
        [DllImport("Cxxdll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void RegisterTimers(int timerid, int uElapse);
        [DllImport("Cxxdll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void UnRegisterTimers(int timerid);
        [DllImport("Cxxdll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SendData(string pData, int size, int mainID, int assistID, int uIdentification);

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
            m_NetworkMgr = new NetworkMgr();
            m_CallBackFunc = new CBEventHandle(NetworkCallBackFunc);
            m_CBTimerHandle = new CBTimerHandle(TimerCallBackFunc);
        }

        public void SendMsg(string pData, int size, MsgCmd mainID, int assistID, MsgCmd uIdentification)
        {
            if (mainID <= MsgCmd.MsgCmd_Begin ||
                mainID >= MsgCmd.MsgCmd_End ||
                uIdentification <= MsgCmd.MsgCmd_Begin ||
                uIdentification >= MsgCmd.MsgCmd_End)
            {
                return;
            }
            SendData(pData, size, (int)mainID, assistID, (int)uIdentification);
        }

        public bool InitSocket(string ip, int port, int timerCnt)
        {
            if (!InitNetwork(ip, port, timerCnt))
            {
                return false;
            }
            if (!InitCxxnet(m_CallBackFunc, m_CBTimerHandle))
            {
                return false;
            }
            m_TimerCnt = timerCnt;

            return true;
        }

        public void RegisterNetwork(MsgCmd cmd, Action<NetWorkMsg> ac)
        {
            if (cmd <= MsgCmd.MsgCmd_Begin || 
                cmd >= MsgCmd.MsgCmd_End)
            {
                return;
            }
            m_NetworkMgr.AddNetworkDictionary((UInt32)cmd, ac);
        }

        public void UnRegisterNetwork(MsgCmd cmd)
        {
            if (cmd <= MsgCmd.MsgCmd_Begin || 
                cmd >= MsgCmd.MsgCmd_End)
            {
                return;
            }
            m_NetworkMgr.DelNetworkDictionary((UInt32)cmd);
        }

        public void RegisterTimer(TimerCmd timerid, int uElapse, Action<int> ac)
        {
            if (timerid <= TimerCmd.TimerCmd_Begin || 
                timerid >= TimerCmd.TimerCmd_End ||
                m_TimerCnt <= 0)
            {
                return;
            }
            if (m_NetworkMgr.AddTimerDictionary((int)timerid, ac))
            {
                RegisterTimers((int)timerid, uElapse);
            }
        }

        public void UnRegisterTimer(TimerCmd timerid)
        {
            if (timerid <= TimerCmd.TimerCmd_Begin || 
                timerid >= TimerCmd.TimerCmd_End ||
                m_TimerCnt <= 0)
            {
                return;
            }
            UnRegisterTimers((int)timerid);
            m_NetworkMgr.DelTimerDictionary((int)timerid);
        }

        private void NetworkCallBackFunc(REvent eve)
        {
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
            if ((TimerCmd)timer <= TimerCmd.TimerCmd_Begin ||
                (TimerCmd)timer >= TimerCmd.TimerCmd_End)
            {
                return;
            }
            m_NetworkMgr.TimerDispatch(timer);
        }
    }
}
