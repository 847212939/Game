ConfigMgr = ConfigMgr:new()
ConfigMgrFnTable = {}
package.path = ";;../Data/Config/?.lua"

function ReadBaseCfg()

    local BaseCfg = require "BaseCfg"

    if BaseCfg then
        ConfigMgr:ReadThreadCntCfg(BaseCfg.ThreadCnt)
    end

    local LogicCfg = LogicCfg:new()
    if BaseCfg then
        LogicCfg.ip = BaseCfg.LogicCfg.ip
        LogicCfg.port = BaseCfg.LogicCfg.port
        LogicCfg.maxSocketCnt = BaseCfg.LogicCfg.maxSocketCnt
        ConfigMgr:ReadLogicCfg(LogicCfg)
    end
    LogicCfg:delete()

    local DbCfg = DbCfg:new()
    if BaseCfg then
        DbCfg.ip = BaseCfg.DbCfg.ip
        DbCfg.user = BaseCfg.DbCfg.user
        DbCfg.passwd = BaseCfg.DbCfg.passwd
        DbCfg.database = BaseCfg.DbCfg.database
        DbCfg.port = BaseCfg.DbCfg.port
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

    print("LoadConfig is begin...")

	for i = 1, GetLength(ConfigMgrFnTable) do
		ConfigMgrFnTable[i]()
	end

    print("LoadConfig is end...")

    return ConfigMgr
end