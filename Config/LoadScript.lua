ConfigMgr = ConfigMgr:new()
ConfigMgrFnTable = {}
package.path = ";;../Data/Config/?.lua"

function ReadBaseCfg()
    ConfigMgr:CoutLog("Load configuration BaseCfg begin")

    local BaseCfg = require "BaseCfg"


    if BaseCfg then
        ConfigMgr:ReadKeyCfg(BaseCfg.Skey or 0)
        ConfigMgr:ReadTimerCntCfg(BaseCfg.TimerCnt or 0)
        ConfigMgr:ReadServerIDCfg(BaseCfg.serverID or 0)
        ConfigMgr:ReadThreadCntCfg(BaseCfg.ThreadCnt or 0)
        ConfigMgr:ReadLogPrintTmCfg(BaseCfg.LogPrintfTime or 0)
        ConfigMgr:ReadMaxSocketCntCfg(BaseCfg.maxSocketCnt or 0)
        ConfigMgr:ReadExitCfg(BaseCfg.Exit or 0)

        local LogicCfg = LogicCfg:new()
        LogicCfg.ip = BaseCfg.LogicCfg.ip or 0
        LogicCfg.port = BaseCfg.LogicCfg.port or 0
        ConfigMgr:ReadLogicCfg(LogicCfg)
        LogicCfg:delete()

        local DbCfg = DbCfg:new()
        DbCfg.ip = BaseCfg.DbCfg.ip or 0
        DbCfg.user = BaseCfg.DbCfg.user or 0
        DbCfg.passwd = BaseCfg.DbCfg.passwd or 0
        DbCfg.database = BaseCfg.DbCfg.database or 0
        DbCfg.port = BaseCfg.DbCfg.port or 0
        ConfigMgr:ReadDbCfg(DbCfg)
        DbCfg:delete()
    end

    ConfigMgr:CoutLog("Load configuration BaseCfg end")
end
table.insert(ConfigMgrFnTable, ReadBaseCfg)

function ReadTestCfg()
    ConfigMgr:CoutLog("Load configuration TestCfg begin")
    ConfigMgr:CoutLog("Load configuration TestCfg end")
end
table.insert(ConfigMgrFnTable, ReadTestCfg)

function GetLength(t)
    local res = 0
    for i,v in pairs(t) do
        res = res + 1
    end
    return res
end

function LoadConfig()
    ConfigMgr:CoutLog("Load configuration begin")

	for i = 1, GetLength(ConfigMgrFnTable) do
		ConfigMgrFnTable[i]()
	end

    ConfigMgr:CoutLog("Load configuration end")
    return ConfigMgr
end