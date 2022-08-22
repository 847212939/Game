
namespace Client.Network
{
    internal class LoginMgr
    {
        public LoginMgr()
        {
            
        }

        public void Initialize()
        {
            SocketMgr socketMgr = SocketMgr.GetInstance();
            socketMgr.RegisterTimer(TimerCmd.TimerCmd_Test, 1000, LoginTimer);
            socketMgr.RegisterNetwork(MsgCmd.MsgCmd_Login, Network);
        }

        public void LoginTimer(int timer)
        {
            Console.WriteLine("定时器" + timer + "回调了");
        }

        public void Network(NetWorkMsg msg)
        {
            Console.WriteLine("Network 回调了");
        }
    }
}
