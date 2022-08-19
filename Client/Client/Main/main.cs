using System;
using Client.Utils;
using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Client.Main
{
    internal class Program
    {
        [DllImport("..\\..\\..\\..\\..\\x64\\Release\\Cxxdll.dll")]
        private static extern void InitCxxnet();

        static void Main(string[] args)
        {
            InitCxxnet();
            Console.ReadKey();
        }
    }
}