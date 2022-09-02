using Client.Network;
using Client.Util;

namespace Client.Main
{
    internal class Program
    {
        static void Main(string[] args)
        {
            SocketMgr socketMgr = SocketMgr.GetInstance();
            if (socketMgr.InitSocket() != 0)
            {
                Console.WriteLine("初始化网络失败");
                return;
            }

            LoginMgr loginMgr = new LoginMgr();
            loginMgr.Initialize(SocketMgr.GetInstance());

            {
                Netmsg cin = new Netmsg();
                cin.Write("8888");
                cin.Write("8888");

                socketMgr.SendMsg(cin.Data, cin.Length, MsgCmd.MsgCmd_Login, 1, MsgCmd.MsgCmd_PlayerPreproces);
                
            }

            // 断线重连机制
            while (true)
            {
                if (!socketMgr.GetConnected())
                {
                    if (socketMgr.InitSocket() != 0)
                    {
                        Console.WriteLine("接连失败");
                    }
                    else
                    {
                        Thread.Sleep(10);
                        {
                            Netmsg cin = new Netmsg();
                            cin.Write("8888");
                            cin.Write("8888");

                            socketMgr.SendMsg(cin.Data, cin.Length, MsgCmd.MsgCmd_Login, 1, MsgCmd.MsgCmd_PlayerPreproces);
                        }
                    }
                }

                {
                    Netmsg cin = new Netmsg();
                    cin.Write("8888");
                    cin.Write("8888");

                    socketMgr.SendMsg(cin.Data, cin.Length, MsgCmd.MsgCmd_Login, 1, MsgCmd.MsgCmd_PlayerPreproces);
                }
                Thread.Sleep(1000);
            }
        }
    }
}