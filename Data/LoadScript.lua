ConfigMgr = ConfigMgr:new()
ConfigMgrFnTable = {}
package.path = ";;../Data/Config/?.lua"

function ReadBaseCfg()
    print("Load configuration BaseCfg start ...")

    local BaseCfg = require "BaseCfg"

    if BaseCfg then
        ConfigMgr:ReadServerIDCfg(BaseCfg.serverID or 0)
    end

    if BaseCfg then
        ConfigMgr:ReadThreadCntCfg(BaseCfg.ThreadCnt or 0)
    end

    if BaseCfg then
        ConfigMgr:ReadLogPrintTmCfg(BaseCfg.LogPrintfTime or 0)
    end

    local LogicCfg = LogicCfg:new()
    if BaseCfg then
        LogicCfg.ip = BaseCfg.LogicCfg.ip or 0
        LogicCfg.port = BaseCfg.LogicCfg.port or 0
        LogicCfg.maxSocketCnt = BaseCfg.LogicCfg.maxSocketCnt or 0
        ConfigMgr:ReadLogicCfg(LogicCfg)
    end
    LogicCfg:delete()

    local DbCfg = DbCfg:new()
    if BaseCfg then
        DbCfg.ip = BaseCfg.DbCfg.ip or 0
        DbCfg.user = BaseCfg.DbCfg.user or 0
        DbCfg.passwd = BaseCfg.DbCfg.passwd or 0
        DbCfg.database = BaseCfg.DbCfg.database or 0
        DbCfg.port = BaseCfg.DbCfg.port or 0
        ConfigMgr:ReadDbCfg(DbCfg)
    end
    DbCfg:delete()
end
table.insert(ConfigMgrFnTable, ReadBaseCfg)

function GetLength(t)
    local res = 0
    for i,v in pairs(t) do
        res = res + 1
    end
    return res
end

function LoadConfig()

    print("Load configuration start ...")

	for i = 1, GetLength(ConfigMgrFnTable) do
		ConfigMgrFnTable[i]()
	end

    print("End of loading configuration ...")

    return ConfigMgr
end