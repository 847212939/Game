
namespace Client.Network
{
    internal class LoginMgr
    {
        public LoginMgr()
        {
            
        }

        public void Initialize(SocketMgr socketMgr)
        {
            socketMgr.RegisterTimer(TimerCmd.TimerCmd_Test, 1000, LoginTimer);
            socketMgr.RegisterNetwork(MsgCmd.MsgCmd_Login, Network);
        }

        public void LoginTimer(int timer)
        {
            Console.WriteLine("收到了 " + timer + " 定时器");
        }

        public void Network(NetWorkMsg msg)
        {
            Console.WriteLine("收到了 "+ msg.uMainID + " 协议");
        }
    }
}
