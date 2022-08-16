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
        ConfigMgr:ReadOpenServerTime(BaseCfg.OpenServerTime or 0)

        if BaseCfg.LogicCfg then
            local CLogicCfg = CLogicCfg:new()
            CLogicCfg.ip = BaseCfg.LogicCfg.ip or 0
            CLogicCfg.port = BaseCfg.LogicCfg.port or 0
            ConfigMgr:ReadLogicCfg(CLogicCfg)
            CLogicCfg:delete()
        end

        if BaseCfg.DbCfg then
            local CDbCfg = CDbCfg:new()
            CDbCfg.ip = BaseCfg.DbCfg.ip or 0
            CDbCfg.user = BaseCfg.DbCfg.user or 0
            CDbCfg.passwd = BaseCfg.DbCfg.passwd or 0
            CDbCfg.database = BaseCfg.DbCfg.database or 0
            CDbCfg.port = BaseCfg.DbCfg.port or 0
            ConfigMgr:ReadDbCfg(CDbCfg)
            CDbCfg:delete()
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
            for _,value in ipairs(ActivityCfg.ActivityList) do
                local CActivityList = CActivityList:new()
                CActivityList.id = value.id or 0
                CActivityList.type = value.type or 0
                CActivityList.sid = value.sid or 0
                CActivityList.activityBreakdown = value.activityBreakdown or 0
                if value.beginTime then
                    for k,v in ipairs(value.beginTime) do
	                    CActivityList:AddBeginTime(v or 0)
                    end
                end
                if value.endTime then
                    for k,v in ipairs(value.endTime) do
	                    CActivityList:AddEndTime(v or 0)
                    end
                end
                if value.solidTime then
                    for k,v in ipairs(value.solidTime) do
	                    CActivityList:AddSolidTime(v or 0)
                    end
                end
                if value.openServerTime then
                    for k,v in ipairs(value.openServerTime) do
	                    CActivityList:AddOpenServerTime(v or 0)
                    end
                end
                ConfigMgr:ReadActivityList(CActivityList)
                CActivityList:delete()
            end
        end

        if ActivityCfg.BrushMonsterCfg then
            for _,value in ipairs(ActivityCfg.BrushMonsterCfg) do
                local CBrushMonsterCfg = BrushMonsterCfg:new()
                CBrushMonsterCfg.id = value.id or 0
                CBrushMonsterCfg.sid = value.sid or 0
                CBrushMonsterCfg.mid = value.mid or 0
                CBrushMonsterCfg.count = value.count or 0
                CBrushMonsterCfg.delayTime = value.delayTime or 0
                CBrushMonsterCfg.refreshTime = value.refreshTime or 0
                CBrushMonsterCfg.x = value.x or 0
                CBrushMonsterCfg.y = value.y or 0
                ConfigMgr:ReadBrushMonsterCfg(CBrushMonsterCfg)
                CBrushMonsterCfg:delete()
            end
        end

        if ActivityCfg.ActivityBreakdown then
            for _,value in ipairs(ActivityCfg.ActivityBreakdown) do
                local CActivityBreakdown = CActivityBreakdown:new()
                CActivityBreakdown.id = value.id or 0
                CActivityBreakdown.dayBreakdown = value.dayBreakdown or 0
                CActivityBreakdown.hourBreakdown = value.hourBreakdown or 0
                if ActivityCfg.ActivityBreakdown.dayBreakdown > 0 then
                    if value.dayBreakdownList then
                        for index,vp in ipairs(value.dayBreakdownList) do
                            for _,vc in ipairs(vp) do
	                            CActivityBreakdown:AddDayBreakdownList(index or 0, vc or 0)
                            end
                        end
                    end
                end
                if ActivityCfg.ActivityBreakdown.hourBreakdown > 0 then
                    if value.hourBreakdownList then
                        for index,vp in ipairs(value.hourBreakdownList) do
                            for _,vc in ipairs(vp) do
	                            CActivityBreakdown:AddHourBreakdownList(index or 0, vc or 0)
                            end
                        end
                    end
                end
                ConfigMgr:ReadActivityBreakdownCfg(CActivityBreakdown)
                CActivityBreakdown:delete()
            end
        end
    end

    ConfigMgr:CoutLog(LOG_CINFO, "Load configuration ReadActivityCfg end")
end
table.insert(ConfigMgrFnTable, ReadActivityCfg)

function ReadSkillCfg()
    ConfigMgr:CoutLog(LOG_CINFO, "Load configuration ReadSkillCfg begin")
    local SkillCfg = require "SkillCfg"

    if SkillCfg then
        if SkillCfg.HeroList then
            for _,value in ipairs(SkillCfg.HeroList) do
                local CHeroList = CHeroList:new()
                CHeroList.heroId = value.heroId or 0
                CHeroList.heroType = value.heroType or 0
                CHeroList.heroName = value.heroName or 0
                if ActivityBreakdown.dayBreakdown > 0 then
                    if value.dayBreakdownList then
                        for index,vp in ipairs(value.dayBreakdownList) do
                            for _,vc in ipairs(vp) do
	                            ActivityBreakdown:AddDayBreakdownList(index or 0, vc or 0)
                            end
                        end
                    end
                end
                if ActivityBreakdown.hourBreakdown > 0 then
                    if value.hourBreakdownList then
                        for index,vp in ipairs(value.hourBreakdownList) do
                            for _,vc in ipairs(vp) do
	                            ActivityBreakdown:AddHourBreakdownList(index or 0, vc or 0)
                            end
                        end
                    end
                end
                ConfigMgr:ReadActivityBreakdownCfg(ActivityBreakdown)
                ActivityBreakdown:delete()
            end
        end
    end

    ConfigMgr:CoutLog(LOG_CINFO, "Load configuration ReadSkillCfg end")
end
table.insert(ConfigMgrFnTable, ReadSkillCfg)
