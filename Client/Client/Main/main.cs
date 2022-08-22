using System;
using Client.Utils;
using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Client.Main
{
    internal class Program
    {
        private static SocketMgr socketMgr = new SocketMgr();
        public static SocketMgr GetSocketMgr()
        {
            return socketMgr;
        }

        static void Main(string[] args)
        {
            SocketMgr socketMgr = GetSocketMgr();
            if (!socketMgr.InitSocket("127.0.0.1", 8888, 1))
            {
                return;
            }

            LoginMgr loginMgr = new LoginMgr();

            Util cos = new Util();
            cos.WriteString("8888");
            cos.WriteString("8888");

            socketMgr.SendMsg(cos.Data, cos.Length, 7, 1, 3);

            Console.ReadKey();
        }
    }
}