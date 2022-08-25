using Client.Network;

namespace Client.Main
{
    internal class Program
    {
        static void Main(string[] args)
        {
            int prot = 8888;

            SocketMgr socketMgr = SocketMgr.GetInstance();
            if (socketMgr.InitSocket("127.0.0.1", prot, 1) != 0)
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

            while (true)
            {
                if (!socketMgr.GetConnected())
                {
                    prot += 1;
                    socketMgr.UnInitSocket();
                    socketMgr.InitSocket("127.0.0.1", prot, 1);
                }

                Thread.Sleep(1000);
            }
        }
    }
}