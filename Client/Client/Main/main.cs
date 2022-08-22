using Client.Utils;

namespace Client.Main
{
    internal class Program
    {
        static void Main(string[] args)
        {
            SocketMgr socketMgr = SocketMgr.GetInstance();
            if (!socketMgr.InitSocket("127.0.0.1", 8888, 1))
            {
                Console.WriteLine("初始化网络失败");
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