BaseCfg = 
{
    --服务器ID
    serverID = 1005, 

    --线程池的数量
    ThreadCnt = 3,

    LogicCfg =
    {
        ip = "127.0.0.1",
        port = 8888,
        -- 最大链接数量 libevent 单线程默认的32000
        maxSocketCnt = 8192,    
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