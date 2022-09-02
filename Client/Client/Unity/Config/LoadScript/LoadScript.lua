
function GetLength(t)
    local res = 0
    for i,v in pairs(t) do
        res = res + 1
    end
    return res
end

function ReadBaseCfg()
	local message = {}
	
    local prefabsCfg = require "PrefabsCfg"
    if prefabsCfg then
		table.insert(message, GetLength(prefabsCfg) or 0)
        for _,value in ipairs(prefabsCfg) do
			table.insert(message, value.sceneid or 0)
            if value.data then
				table.insert(message, GetLength(value.data) or 0)
                for _,v in ipairs(value.data) do
					table.insert(message, v.name or 0)
					table.insert(message, v.path or 0)
                end
            end
        end
    end
	
	table.insert(message, "\n")
    return table.concat(message,"\n")
end