using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Client.Utils
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct REvent
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 2048)]
        public string data;
    };

    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public delegate void CBTimerHandle();
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
    public delegate void CBEventHandle(REvent eve/*, [MarshalAs(UnmanagedType.LPStr)]StringBuilder source*/);

    internal class SocketMgr
    {
        [DllImport("..\\..\\..\\..\\..\\x64\\Debug\\Cxxdll.dll", CallingConvention = CallingConvention.Cdecl)]
        extern static void RegisterTimers(int timerid, int uElapse, CBTimerHandle func);
        [DllImport("..\\..\\..\\..\\..\\x64\\Debug\\Cxxdll.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void InitCxxnet(CBEventHandle func);
        [DllImport("..\\..\\..\\..\\..\\x64\\Debug\\Cxxdll.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void SendData(string pData, int size, int mainID, int assistID, int uIdentification);

        public SocketMgr()
        {
            m_CallBackFunc = new CBEventHandle(CallBackFunc);
            m_CBTimerHandle = new CBTimerHandle(TimerCallBackFunc);
        }
        
        public void Init()
        {
            InitCxxnet(m_CallBackFunc);
            //RegisterTimers(1, 300, m_CBTimerHandle);
        }

        private void CallBackFunc(REvent eve)
        {
            Console.WriteLine("CallBackFunc");
            //Util cis = new Util(eve.data);

            //UInt32 mainId = cis.ReadUInt32();
            //Console.WriteLine(mainId);
            //UInt32 asssid = cis.ReadUInt32();
            //Console.WriteLine(asssid);
            //UInt32 iden = cis.ReadUInt32();
            //Console.WriteLine(iden);
            //UInt32 size = cis.ReadUInt32();
            //Console.WriteLine(size);

            //string str = cis.ReadString();
            //Console.WriteLine(str.Length);
            //Console.WriteLine(str);

            //Util cos = new Util();
            //cos.WriteString("888");
            //cos.WriteString("888");

            //SendData(cos.Data, cos.Length, 7, 1, 3);
        }

        private void TimerCallBackFunc()
        {
            Console.WriteLine("TimerCallBackFunc");
        }

        private CBTimerHandle m_CBTimerHandle;
        private CBEventHandle m_CallBackFunc;
    }
}
