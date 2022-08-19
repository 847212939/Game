using System;
using Client.Utils;
using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Client.Main
{
    internal class Program
    {
        [DllImport("..\\..\\..\\..\\..\\x64\\Debug\\Cxxdll.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void InitCxxnet();

        static void Main(string[] args)
        {
            Socketcb socketcb = new Socketcb();

            InitCxxnet();
            Console.ReadKey();
        }
    }
}