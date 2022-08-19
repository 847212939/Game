using System;
using Client.Utils;
using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Client.Main
{
    internal class Program
    {
        public static void UtilsTest()
        {
            Util os = new Util();
            os.WriteInt16(16);
            os.WriteInt32(32);
            os.WriteInt64(64);
            os.WriteUInt16(16);
            os.WriteUInt32(32);
            os.WriteUInt64(64);
            os.WriteString("nishis   habi");
            os.WriteFloat(3.1459266344111555f);
            os.WriteDouble(3.99941052);

            Util iss = new Util(os.Data);
            Int16 i16 = iss.ReadInt16();
            Int32 i32 = iss.ReadInt32();
            Int64 i64 = iss.ReadInt64();
            UInt16 ui16 = iss.ReadUInt16();
            UInt32 ui32 = iss.ReadUInt32();
            UInt64 ui64 = iss.ReadUInt64();
            string str = iss.ReadString();
            float f = iss.ReadFloat();
            double d = iss.ReadDouble();
        }
        public static void CallToChildThread()
        {
            Console.WriteLine("Child thread starts");
            // 线程暂停 5000 毫秒
            int sleepfor = 5000;
            Console.WriteLine("Child Thread Paused for {0} seconds", sleepfor / 1000);
            Thread.Sleep(sleepfor);
            Console.WriteLine("Child thread resumes");
        }
        public static void InitSocketThread()
        {
            ThreadStart childref = new ThreadStart(CallToChildThread);
            Console.WriteLine("In Main: Creating the Child thread");
            Thread childThread = new Thread(childref);
            childThread.Start();
            Console.ReadKey();
        }

        [DllImport("..\\..\\..\\..\\..\\x64\\Debug\\Cxxdll.dll")]
        private static extern int Add(int n1, int n2);

        [DllImport("..\\..\\..\\..\\..\\x64\\Debug\\Cxxdll.dll")]
        private static extern int Sub(int n1, int n2);
        static void Main(string[] args)
        {
            int a = Add(1, 2);
            int b = Sub(5, 10);
        }
    }
}