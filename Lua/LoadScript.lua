package.path = ";;../Data/Config/?.lua"
ConfigMgrFnTable = {}
ConfigMgr = ConfigMgr:new()

function LoadConfig()
    local cnt = GetLength(ConfigMgrFnTable)
    ConfigMgr:CoutLog(LOG_CINFO, "Load configuration begin")

	for i = 1, cnt do
		ConfigMgrFnTable[i]()
	end

    ConfigMgr:CoutLog(LOG_CINFO, "Load configuration end")
    ConfigMgr:CoutLog(LOG_CINFO, "load configurations cnt = " .. tostring(cnt))
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
    ConfigMgr:CoutLog(LOG_CINFO, "Load configuration BaseCfg begin")

    local BaseCfg = require "BaseCfg"

    if BaseCfg then
        ConfigMgr:ReadKeyCfg(BaseCfg.Skey or 0)
        ConfigMgr:ReadTimerCntCfg(BaseCfg.TimerCnt or 0)
        ConfigMgr:ReadServerIDCfg(BaseCfg.serverID or 0)
        ConfigMgr:ReadThreadCntCfg(BaseCfg.ThreadCnt or 0)
        ConfigMgr:ReadLogPrintTmCfg(BaseCfg.LogPrintfTime or 0)
        ConfigMgr:ReadMaxSocketCntCfg(BaseCfg.maxSocketCnt or 0)
        ConfigMgr:ReadExitCfg(BaseCfg.Exit or 0)

        if BaseCfg.LogicCfg then
            local LogicCfg = LogicCfg:new()
            LogicCfg.ip = BaseCfg.LogicCfg.ip or 0
            LogicCfg.port = BaseCfg.LogicCfg.port or 0
            ConfigMgr:ReadLogicCfg(LogicCfg)
            LogicCfg:delete()
        end

        if BaseCfg.DbCfg then
            local DbCfg = DbCfg:new()
            DbCfg.ip = BaseCfg.DbCfg.ip or 0
            DbCfg.user = BaseCfg.DbCfg.user or 0
            DbCfg.passwd = BaseCfg.DbCfg.passwd or 0
            DbCfg.database = BaseCfg.DbCfg.database or 0
            DbCfg.port = BaseCfg.DbCfg.port or 0
            ConfigMgr:ReadDbCfg(DbCfg)
            DbCfg:delete()
        end
    end

    ConfigMgr:CoutLog(LOG_CINFO, "Load configuration BaseCfg end")
end
table.insert(ConfigMgrFnTable, ReadBaseCfg)

function ReadActivityCfg()
    ConfigMgr:CoutLog(LOG_CINFO, "Load configuration ReadActivityCfg begin")

    local ActivityCfg = require "ActivityCfg"

    if ActivityCfg then
        if ActivityCfg.ActivityList then
            for k,value in ipairs(ActivityCfg.ActivityList) do
                local ActivityList = ActivityList:new()
                ActivityList.id = value.id or 0
                ActivityList.type = value.type or 0
                ActivityList.sid = value.sid or 0
                ActivityList.activityBreakdown = value.activityBreakdown or 0
                ActivityList.breakdown = value.breakdown or 0
                if value.beginTime then
                    for k,v in ipairs(value.beginTime) do
	                    ActivityList:AddBeginTime(v or 0)
                    end
                end
                if value.endTime then
                    for k,v in ipairs(value.endTime) do
	                    ActivityList:AddEndTime(v or 0)
                    end
                end
                if value.solidTime then
                    for k,v in ipairs(value.solidTime) do
	                    ActivityList:AddSolidTime(v or 0)
                    end
                end
                if value.openServerTime then
                    for k,v in ipairs(value.openServerTime) do
	                    ActivityList:AddOpenServerTime(v or 0)
                    end
                end
                ConfigMgr:ReadActivityList(ActivityList)
                ActivityList:delete()
            end
        end

        if ActivityCfg.BrushMonsterCfg then
            for k,value in ipairs(ActivityCfg.BrushMonsterCfg) do
                local BrushMonsterCfg = BrushMonsterCfg:new()
                BrushMonsterCfg.id = value.id or 0
                BrushMonsterCfg.sid = value.sid or 0
                BrushMonsterCfg.mid = value.mid or 0
                BrushMonsterCfg.count = value.count or 0
                BrushMonsterCfg.delayTime = value.delayTime or 0
                BrushMonsterCfg.refreshTime = value.refreshTime or 0
                BrushMonsterCfg.x = value.x or 0
                BrushMonsterCfg.y = value.y or 0
                ConfigMgr:ReadBrushMonsterCfg(BrushMonsterCfg)
                BrushMonsterCfg:delete()
            end
        end

        if ActivityCfg.ActivityBreakdown then
            for k,value in ipairs(ActivityCfg.ActivityBreakdown) do
                local ActivityBreakdown = ActivityBreakdown:new()
                ActivityBreakdown.id = value.id or 0
                ActivityBreakdown.brushMonsterCfg = value.brushMonsterCfg or 0
                if value.beginTime then
                    for k,v in ipairs(value.beginTime) do
	                    ActivityBreakdown:AddBeginTime(v or 0)
                    end
                end
                if value.endTime then
                    for k,v in ipairs(value.endTime) do
	                    ActivityBreakdown:AddEndTime(v or 0)
                    end
                end
                ConfigMgr:ReadActivityBreakdownCfg(ActivityBreakdown)
                ActivityBreakdown:delete()
            end
        end
    end

    ConfigMgr:CoutLog(LOG_CINFO, "Load configuration ReadActivityCfg end")
end
table.insert(ConfigMgrFnTable, ReadActivityCfg)
