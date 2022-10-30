BaseCfg= 
{
    --服务器ID
    serverID = 1005, 

    --线程池的数量
    ThreadCnt = 12,

    --定时器的个数
    TimerCnt = 1,

    --日志打印时间秒
    LogPrintfTime = 1,

    --程序退出
    Exit = "exit",

    --最大链接数量 单线程默认的32000
    maxSocketCnt = 24576,

    --开服时间
    OpenServerTime = "2022-09-01 10:38:35",
    
    --对称加密
    Skey = "LiuDaNaoDai",

    LogicCfg =
    {
        ip = "127.0.0.1",
        port = 8888, 
    },
    DBServerCfg =
    {
        ip = "127.0.0.1",
        port = 8889, 
    },
}

return BaseCfg