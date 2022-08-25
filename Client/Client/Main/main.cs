using Client.Network;

namespace Client.Main
{
    internal class Program
    {
        static void Main(string[] args)
        {
            SocketMgr socketMgr = SocketMgr.GetInstance();
            if (socketMgr.InitSocket("127.0.0.1", 8888, 1) != 0)
            {
                Console.WriteLine("初始化网络失败");
                return;
            }

            LoginMgr loginMgr = new LoginMgr();
            loginMgr.Initialize(SocketMgr.GetInstance());

            Netmsg cin = new Netmsg();
            cin.WriteString("8888");
            cin.WriteString("8888");

            socketMgr.SendMsg(cin.Data, cin.Length, MsgCmd.MsgCmd_Login, 1, MsgCmd.MsgCmd_PlayerPreproces);

            // 短线重连
            while (true)
            {
                if (!socketMgr.GetConnected())
                {
                    if (socketMgr.InitSocket("127.0.0.1", 8888, 1) != 0)
                    {
                        Console.WriteLine("初始化网络失败");
                    }
                }

                Thread.Sleep(1000);
            }
        }
    }
}