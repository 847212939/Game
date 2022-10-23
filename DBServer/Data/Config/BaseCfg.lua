BaseCfg= 
{
    --线程池的数量
    ThreadCnt = 12,

    --日志打印时间秒
    LogPrintfTime = 1,

    --程序退出
    Exit = "exit",

    --最大链接数量 单线程默认的32000
    maxSocketCnt = 1024,

    ServerCfg =
    {
        ip = "127.0.0.1",
        port = 8889, 
    },

    DbCfg =
    {
        ip = "127.0.0.1",
        user = "root",
        passwd = "",
        database = "game",
        port = 3366,
    },
}

return BaseCfg