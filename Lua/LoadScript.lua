package.path = ";;../Data/Config/?.lua"
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

function ReadActivityCfg()
    ConfigMgr:CoutLog(LOG_CINFO, "Load ReadActivityCfg")
    local ActivityCfg = require "ActivityCfg"

    if ActivityCfg then
        if ActivityCfg.ActivityList then
            for _,value in ipairs(ActivityCfg.ActivityList) do
                local config = CActivityList:new()
                config.id = value.id or 0
                config.type = value.type or 0
                config.sid = value.sid or 0
                config.activityBreakdown = value.activityBreakdown or 0
                if value.beginTime then
                    for k,v in ipairs(value.beginTime) do
	                    config:AddBeginTime(v or 0)
                    end
                end
                if value.endTime then
                    for k,v in ipairs(value.endTime) do
	                    config:AddEndTime(v or 0)
                    end
                end
                if value.solidTime then
                    for k,v in ipairs(value.solidTime) do
	                    config:AddSolidTime(v or 0)
                    end
                end
                if value.openServerTime then
                    for k,v in ipairs(value.openServerTime) do
	                    config:AddOpenServerTime(v or 0)
                    end
                end
                ConfigMgr:ReadActivityList(config)
                config:delete()
            end
        end

        if ActivityCfg.BrushMonsterCfg then
            for _,value in ipairs(ActivityCfg.BrushMonsterCfg) do
                local config = CBrushMonsterCfg:new()
                config.id = value.id or 0
                config.sid = value.sid or 0
                config.mid = value.mid or 0
                config.count = value.count or 0
                config.delayTime = value.delayTime or 0
                config.refreshTime = value.refreshTime or 0
                config.x = value.x or 0
                config.y = value.y or 0
                ConfigMgr:ReadBrushMonsterCfg(config)
                config:delete()
            end
        end

        if ActivityCfg.ActivityBreakdown then
            for _,value in ipairs(ActivityCfg.ActivityBreakdown) do
                local config = CActivityBreakdown:new()
                config.id = value.id or 0
                config.dayBreakdown = value.dayBreakdown or 0
                config.hourBreakdown = value.hourBreakdown or 0

                if value.dayBreakdown > 0 then
                    if value.dayBreakdownList then
                        for index,vp in ipairs(value.dayBreakdownList) do
                            for _,vc in ipairs(vp) do
	                            config:AddDayBreakdownList(index or 0, vc or 0)
                            end
                        end
                    end
                end

                if value.hourBreakdown > 0 then
                    if value.hourBreakdownList then
                        for index,vp in ipairs(value.hourBreakdownList) do
                            for _,vc in ipairs(vp) do
	                            config:AddHourBreakdownList(index or 0, vc or 0)
                            end
                        end
                    end
                end

                ConfigMgr:ReadActivityBreakdownCfg(config)
                config:delete()
            end
        end
    end
end
table.insert(ConfigMgrFnTable, ReadActivityCfg)

function ReadSkillCfg()
    ConfigMgr:CoutLog(LOG_CINFO, "Load ReadSkillCfg")
    local SkillCfg = require "SkillCfg"

    if SkillCfg then
        if SkillCfg.HeroList then
            for _,value in ipairs(SkillCfg.HeroList) do
                local config = CHeroList:new()
                config.heroId = value.heroId or 0
                config.heroType = value.heroType or 0
                config.heroName = value.heroName or 0
                if value.skillId then
                    for _,v in ipairs(value.skillId) do
                        config:AddskillId(v)
                    end
                end
                ConfigMgr:ReadHeroList(config)
                config:delete()
            end
        end

        if SkillCfg.SkillIdList then
            for _,value in ipairs(SkillCfg.SkillIdList) do
                local config = CSkillIdList:new()
                config.skillId = value.skillId or 0
                config.skillType = value.skillType or 0
                config.skillCd = value.skillCd or 0
                config.skillEffect = value.skillEffect or 0
                config.skillEffectCd = value.skillEffectCd or 0
                config.minSkillCd = value.minSkillCd or 0
                config.minSkillEffect = value.minSkillEffect or 0
                if value.attrs then
                    for _,v in ipairs(value.attrs) do
                        config:AddAttrs(v[1] or 0, v[2] or 0)
                    end
                end
                ConfigMgr:ReadSkillIdList(config)
                config:delete()
            end
        end
    end
end
table.insert(ConfigMgrFnTable, ReadSkillCfg)
