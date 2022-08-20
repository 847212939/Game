using System;
using Client.Utils;
using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Client.Main
{
    internal class Program
    {
        static void Main(string[] args)
        {
            SocketMgr socketcb = new SocketMgr();
            socketcb.Init();
            Console.ReadKey();
        }
    }
}