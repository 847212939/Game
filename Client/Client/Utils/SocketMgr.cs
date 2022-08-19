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
    public delegate void CBEventHandle(REvent eve/*, [MarshalAs(UnmanagedType.LPStr)]StringBuilder source*/);

    internal class SocketMgr
    {
        public SocketMgr()
        {
            m_CallBackFunc = new CBEventHandle(CallBackFunc);
            RegisterCallBack(m_CallBackFunc);
        }

        [DllImport("..\\..\\..\\..\\..\\x64\\Debug\\Cxxdll.dll",CallingConvention = CallingConvention.Cdecl)]
        extern static void RegisterCallBack(CBEventHandle func);
        [DllImport("..\\..\\..\\..\\..\\x64\\Debug\\Cxxdll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitCxxnet();

        private void CallBackFunc(REvent eve)
        {
            Util cis = new Util(eve.data);

            UInt32 mainId = cis.ReadUInt32();
            Console.WriteLine(mainId);
            UInt32 asssid = cis.ReadUInt32();
            Console.WriteLine(asssid);
            UInt32 iden = cis.ReadUInt32();
            Console.WriteLine(iden);
            UInt32 size = cis.ReadUInt32();
            Console.WriteLine(size);

            string str = cis.ReadString();
            Console.WriteLine(str.Length);
            Console.WriteLine(str);
        }

        private CBEventHandle m_CallBackFunc;
    }
}
