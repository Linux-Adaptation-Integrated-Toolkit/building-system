--!A cross-platform build utility based on Lua
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--
-- Copyright (C) 2015-present, TBOOX Open Source Group.
--
-- @author      ruki
-- @file        info.lua
--

-- imports
import("core.base.task")
import("core.base.option")
import("core.base.hashset")
import("core.base.json")
import("core.project.project")
import("core.package.package", {alias = "core_package"})
import("devel.git")
import("utils.archive")
import("private.action.require.impl.utils.filter")
import("private.action.require.impl.utils.url_filename")
import("private.action.require.impl.package")
import("private.action.require.impl.repository")
import("private.action.require.impl.environment")
import("private.action.require.impl.utils.get_requires")

-- from xmake/system/remote?
function _from(instance)
    local fetchinfo = instance:fetch()
    if fetchinfo then
        if instance:is_system() then
            return "system"
        else
            return "3rd"
        end
    else
        return "missing"
    end
end

-- show the given package info

function main(requires_raw)

    -- get requires and extra config
    local requires_extra = nil
    local requires, requires_extra = get_requires(requires_raw)
    if not requires or #requires == 0 then
        return
    end

    -- enter environment
    environment.enter()

    local wali_table = {}

    -- list all packages
    for _, instance in ipairs(package.load_packages(requires, {requires_extra = requires_extra})) do

        -- show package name
        local requireinfo = instance:requireinfo() or {}
        wali_table[requireinfo.originstr] = {}

        -- show optional and installed info
        if(instance:is_optional()) then
            wali_table[requireinfo.originstr]["optional"] = true
        else
            wali_table[requireinfo.originstr]["optional"] = false
        end

        if (_from(instance) == "missing") then
            wali_table[requireinfo.originstr]["installed"] = false
        else
            wali_table[requireinfo.originstr]["installed"] = true
        end

        wali_table[requireinfo.originstr]["info"] = {}

        wali_table[requireinfo.originstr]["info"]["version"] = ""

        -- show fetch info
        wali_table[requireinfo.originstr]["info"]["fetchfrom"] = _from(instance)

    end

    local wali_json = json.encode(wali_table)
    print(wali_json)
--[[
        local wali_table = {"_WALI_ERROR"}
        local wali_json = json.encode(wali_table)
        print(wali_json)
--]]
    -- leave environment
    environment.leave()
end
