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
            if (!socketcb.InitSocket("127.0.0.1", 8888, 1))
            {
                return;
            }

            socketcb.RegisterTimer(1, 500);

            Console.ReadKey();
        }
    }
}