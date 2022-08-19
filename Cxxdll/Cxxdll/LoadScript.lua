package.path = ";;../../../../../Cxxdll/Cxxdll/?.lua"
ConfigMgrFnTable = {}
ConfigMgr = ConfigMgr:new()

function LoadConfig()
    local cnt = GetLength(ConfigMgrFnTable)
	for i = 1, cnt do
		ConfigMgrFnTable[i]()
	end
    ConfigMgr:CoutLog(LOG_CINFO, "Load Config cnt = " .. tostring(cnt))
    return ConfigMgr
end

function GetLength(t)
    local res = 0
    for i,v in pairs(t) do
        res = res + 1
    end
    return res
end

function ReadBaseCfg()
    ConfigMgr:CoutLog(LOG_CINFO, "Load BaseCfg")
    local BaseCfg = require "BaseCfg"

    if BaseCfg then
        ConfigMgr:ReadKeyCfg(BaseCfg.Skey or 0)
        ConfigMgr:ReadTimerCntCfg(BaseCfg.TimerCnt or 0)
        ConfigMgr:ReadServerIDCfg(BaseCfg.serverID or 0)
        ConfigMgr:ReadThreadCntCfg(BaseCfg.ThreadCnt or 0)
        ConfigMgr:ReadLogPrintTmCfg(BaseCfg.LogPrintfTime or 0)
        ConfigMgr:ReadMaxSocketCntCfg(BaseCfg.maxSocketCnt or 0)
        ConfigMgr:ReadExitCfg(BaseCfg.Exit or 0)
        ConfigMgr:ReadOpenServerTime(BaseCfg.OpenServerTime or 0)

        if BaseCfg.LogicCfg then
            local config = CLogicCfg:new()
            config.ip = BaseCfg.LogicCfg.ip or 0
            config.port = BaseCfg.LogicCfg.port or 0
            ConfigMgr:ReadLogicCfg(config)
            config:delete()
        end

        if BaseCfg.DbCfg then
            local config = CDbCfg:new()
            config.ip = BaseCfg.DbCfg.ip or 0
            config.user = BaseCfg.DbCfg.user or 0
            config.passwd = BaseCfg.DbCfg.passwd or 0
            config.database = BaseCfg.DbCfg.database or 0
            config.port = BaseCfg.DbCfg.port or 0
            ConfigMgr:ReadDbCfg(config)
            config:delete()
        end
    end
end
table.insert(ConfigMgrFnTable, ReadBaseCfg)

