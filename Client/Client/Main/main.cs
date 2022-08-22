using Client.Utils;

namespace Client.Main
{
    internal class Program
    {
        private static SocketMgr socketMgr = new SocketMgr();
        private LoginMgr loginMgr;

        public Program()
        {
            loginMgr = new LoginMgr();
        }

        public static SocketMgr GetSocketMgr()
        {
            return socketMgr;
        }

        static void Main(string[] args)
        {
            Util cos = new Util();
            cos.WriteString("8888");
            cos.WriteString("8888");

            socketMgr.SendMsg(cos.Data, cos.Length, 7, 1, 3);

            Console.ReadKey();
        }
    }
}