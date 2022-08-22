using Client.Utils;

namespace Client.Main
{
    internal class Program
    {
        static void Main(string[] args)
        {
            SocketMgr socketMgr = SocketMgr.GetInstance();
            socketMgr.InitSocket("107.0.0.1", 888, 1);

            LoginMgr loginMgr = new LoginMgr();

            Util cos = new Util();
            cos.WriteString("8888");
            cos.WriteString("8888");

            socketMgr.SendMsg(cos.Data, cos.Length, 7, 1, 3);

            Console.ReadKey();
        }
    }
}