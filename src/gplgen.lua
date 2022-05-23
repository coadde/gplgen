#!/usr/bin/env lua
-- Lua 5.4

-- A GIMP palette generator
-- Copyright (C) 2022 Márcio Silva <coadde@hyperbola.info>
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.
-- If not, see <http://www.gnu.org/licenses/>.

---

-- All variables/properties and fuctions/methods
-- are created by table/metatable inheritance style,
-- to set a better code style.

-- This table (map) with "new" function is required
-- to generate a new metatable (class).
GPLGenC = {}
function GPLGenC:new_f(input, output)
    -- All tables (maps) and functions from this table (map),
    -- are created inside of this function,
    -- to allow use only on a new metatable (class)
    -- and not on this table (map).

    self.data_t = {
        colour_t = {
            type = 'rgb',
            depth = {3, 3, 2}
        },
        pmap_t = {
            title = 'GIMP Palette',
            columns = 16
        },
        copyright_t = {
            author = 'anonymous',
            years = tostring(os.date('%Y'))
        },
        error_t = {
            code = 0,
            value = 'none'
        }
    }

    function self:bchn_lgen_m(byte)
        if not byte
              or not tonumber(byte)
              or tonumber(byte) < 1
              or tonumber(byte) > 8 then
            byte = 8
        elseif math.type(tonumber(byte)) == 'float' then
            byte = math.floor(byte)
        end

        local size = 2^byte - 1

        -- Round half away from zero division for integer calcutation:
        -- https://www.calculator.net/rounding-calculator.html
        --
        -- Note: // (floor division to)
        --       %/ (round division to)
        --       == (equal to)
        --       != (not equal to)
        --       x (dividend)
        --       y (divisor)
        --       k (result)
        --       r (remainder)
        --       f (remainder float)
        --       ==> (change to)
        --       -> (such as)
        --       && (and)
        --
        -- Integers calculation:
        -- x//y == k(floor)
        -- x//y != k(round)
        -- x&/y != k(floor)
        -- x&/y == k(round)
        -- x%y == r(int)
        -- k(floor) != k(round)
        -- (x + r(int)) // y == k(round)
        -- (x + x%y) // y == k(round)
        -- (x + x%y) // y == k(round) ==> x+x%y == k(round)*y
        -- x+x%y == k(round)*y ==> x%y == k(round)*y - x
        -- x%y == x&/y*y - x
        --
        -- ((+x)+(+r)) // (+y) == +k(floor)+1
        --   -> +k >= +k(floor)+0.5 (from x/y == k) && r > 0
        -- ((+x)+(+r)) // (+y) == +k(floor)+0
        --   -> +k <  +k(floor)+0.5 (from x/y == k) && r > 0
        -- ((-x)+(+r)) // (-y) == +k(floor)+1
        --   -> +k >= +k(floor)+0.5 (from x/y == k) && r > 0
        -- ((-x)+(+r)) // (-y) == +k(floor)+0
        --   -> +k <  +k(floor)+0.5 (from x/y == k) && r > 0
        --
        -- ((+x)+(-r)) // (-y) == -k(floor)-1
        --   -> -k >= -k(floor)-0.5 (from x/y == k) && r < 0
        -- ((+x)+(-r)) // (-y) == -k(floor)-0
        --   -> -k <  -k(floor)-0.5 (from x/y == k) && r < 0
        -- ((-x)+(-r)) // (+y) == -k(floor)-1
        --   -> -k >= -k(floor)-0.5 (from x/y == k) && r < 0
        --  ((-x)+(-r)) // (+y) == -k(floor)-0
        --   -> -k <  -k(floor)-0.5 (from x/y == k) && r < 0
        --
        -- ---
        -- (x + x%y) // y == k(round)
        -- ---
        --
        -- Integers calcutation:  math.floor((x + x%y) / y)
        local channel_t = {}
        for index = 0, size, 1 do
            -- Integers calcutation:  math.floor((x + x%y) / y)
            table.insert(
                channel_t,
                math.floor((index*255 + index*255%size) / size)
            )
        end
        index, size = nil, nil

        return channel_t
    end

    function self:header_lgen_m()
        local author = self.data_t['copyright_t']['author']
        local columns = self.data_t['pmap_t']['columns']
        local title = self.data_t['pmap_t']['title']
        local years = self.data_t['copyright_t']['years']

        return (
            'GIMP Palette' .. '\n'
              .. 'Name: ' .. title .. '\n'
              .. 'Columns: ' .. columns .. '\n'
              .. '#' .. '\n'
              .. '# Written in ' .. years .. ' by ' .. author .. '\n'
              .. '# To the extent possible under law,'
              .. ' the author(s) have dedicated all copyright' .. '\n'
              .. '# and related and neighboring rights to'
              .. ' this software to the public domain' .. '\n'
              .. '# worldwide.'
              .. ' This software is distributed without any warranty.' .. '\n'
              .. '# You should have received a copy'
              .. ' of the CC0 Public Domain Dedication along' .. '\n'
              .. '# with this software. If not, see' .. '\n'
              .. '# <https://creativecommons.org/publicdomain/zero/1.0/>.'
              .. '\n'
              .. '#' .. '\n'
        )
    end

    function self:cpal_lgen_m()
        local channels = self.data_t['colour_t']['depth']
        local type = self.data_t['colour_t']['type']

        local palette_map = ''
        if type == 'g'
              or type == 'gr'
              or type == 'gry'
              or type == 'gray'
              or type == 'grey' then
            for _, key_index in ipairs(self:bchn_lgen_m(channels[1])) do
                local key_channel = ''

                if key_index <= 9 then
                    key_channel = '  ' .. tostring(key_index)
                elseif key_index <= 99 then
                    key_channel = ' ' .. tostring(key_index)
                else
                    key_channel = tostring(key_index)
                end

                palette_map = (
                    palette_map
                      .. key_channel .. ' '
                      .. key_channel .. ' '
                      .. key_channel .. '\t'
                      .. '#' .. '\n'
                )
            end
            _, key_channel, key_index = nil, nil
        else
            for _, blue_index in ipairs(self:bchn_lgen_m(channels[3])) do
                for _, green_index
                      in ipairs(self:bchn_lgen_m(channels[2])) do
                    for _, red_index
                          in ipairs(self:bchn_lgen_m(channels[1])) do
                        local blue_channel = ''
                        local green_channel = ''
                        local red_channel = ''

                        if blue_index <= 9 then
                            blue_channel = '  ' .. tostring(blue_index)
                        elseif blue_index <= 99 then
                            blue_channel = ' ' .. tostring(blue_index)
                        else
                            blue_channel = tostring(blue_index)
                        end

                        if green_index <= 9 then
                            green_channel = '  ' .. tostring(green_index)
                        elseif green_index <= 99 then
                            green_channel = ' ' .. tostring(green_index)
                        else
                            green_channel = tostring(green_index)
                        end

                        if red_index <= 9 then
                            red_channel = '  ' .. tostring(red_index)
                        elseif red_index <= 99 then
                            red_channel = ' ' .. tostring(red_index)
                        else
                            red_channel = tostring(red_index)
                        end

                        palette_map = (
                            palette_map
                              .. red_channel .. ' '
                              .. green_channel .. ' '
                              .. blue_channel .. '\t'
                              .. '#' .. '\n'
                        )
                    end
                end
            end
            _ = nil
            blue_channel, blue_index = nil, nil
            green_channel, green_index = nil, nil
            red_channel, red_index = nil, nil
        end

        return self:header_lgen_m() .. palette_map
    end

    function self:run_m(input, output)
        local config_t = {}
        if input then
            local pcall_t = {
                pcall(io.open, input, 'r')
            }
            if not pcall_t[4] then
                -- 'pcall_t[2]' contains an input file handle
                --  from "io.open(input, 'r')"
                local file = pcall_t[2]

                for line in file:lines() do
                    for key, value in line:gmatch('(%g+) = ([%g ]+)') do
                        config_t[key] = value
                    end
                end
                key, line, value = nil, nil, nil

                file:close()
                file = nil
            else
                self.data_t['error_t']['code'] = pcall_t[4]
                self.data_t['error_t']['value'] = pcall_t[3]
            end
            pcall_t = nil
        end

        if config_t then
            if config_t['type'] then
                self.data_t['colour_t']['type'] = config_t['type']
            end

            if config_t['depth'] then
                local depth_tmp = {}
                for depth_index in config_t['depth']:gmatch('%S+') do
                    if tonumber(depth_index) then
                        table.insert(depth_tmp, tonumber(depth_index, 10))
                    else
                        table.insert(depth_tmp, 8)
                    end
                end
                depth_index = nil

                self.data_t['colour_t']['depth'] = depth_tmp
                depth_tmp = nil
            end

            if config_t['title'] then
                self.data_t['pmap_t']['title'] = config_t['title']
            end

            if config_t['columns']
                  and tonumber(config_t['columns'])
                  and tonumber(config_t['columns']) >= 0 then
                self.data_t['pmap_t']['columns'] = (
                    tonumber(config_t['columns'], 10)
                )
            end

            if config_t['author'] then
                self.data_t['copyright_t']['author'] = config_t['author']
            end

            if config_t['years'] then
                if tonumber(config_t['years'])
                      and tonumber(config_t['years']) >= 1000
                      and tonumber(config_t['years'])
                        < tonumber(os.date('%Y')) then
                    self.data_t['copyright_t']['years'] = (
                        tonumber(config_t['years'], 10)
                    )
                elseif tonumber(config_t['years']) then
                else
                    self.data_t['copyright_t']['years'] = config_t['years']
                end
            end

            config_t = nil
        end

        if output then
            local pcall_t = {
                pcall(io.open, output, 'w')
            }
            if not pcall_t[4] then
                -- 'pcall_t[2]' contains an output file handle
                --  from "io.open(output, 'w')"
                local file = pcall_t[2]
                file:write(self:cpal_lgen_m())
                file:close()
                file = nil
            else
                self.data_t['error_t']['code'] = pcall_t[4]
                self.data_t['error_t']['value'] = pcall_t[3]
                print(self:cpal_lgen_m())
            end
            pcall_t = nil
        else
            print(self:cpal_lgen_m())
        end

        if self.data_t['error_t']['code'] ~= 0 then
            local code = self.data_t['error_t']['code']
            local value = self.data_t['error_t']['value']

            io.stderr:write(
                value .. ' [Errno ' .. tostring(code) .. ']' .. '\n'
            )

            code, value = nil, nil
        end
    end

    -- This metamethod (special variable) is required.
    self.__index = self

    -- This function is required to generate an metatable (class)
    -- with a new empty (and unused) table '{}' and this table.
    metatable = setmetatable({}, self)

    -- Run this metatable function
    -- to emulate the class constructor method.
    self:run_m(input, output)

    -- Return to a new metatable as class object.
    return metatable
end


GPLGenC:new_f(arg[1], arg[2])
