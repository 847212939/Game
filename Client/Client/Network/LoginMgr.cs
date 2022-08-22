using Client.Main;

namespace Client.Utils
{
    internal class LoginMgr
    {
        public LoginMgr()
        {
            SocketMgr socketMgr = SocketMgr.GetInstance();
            socketMgr.RegisterTimer(2, 500, LoginTimer);
            socketMgr.RegisterNetwork(7, Network);
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
