BaseCfg={
    LogicCfg =
    {
        ip = "127.0.0.1",
        port = 8888,
        maxSocketCnt = 1024,    -- 最大链接数量 libevent 单线程默认的32000
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