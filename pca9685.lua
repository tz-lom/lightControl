local modname = ...   
local M = {}   
_G[modname] = M

function M.create(id, addr)
    local m = {
    
        ID = id,
        ADDR = addr,
        
        init = function(this, pSDA, pSDB)
            return i2c.SLOW == i2c.setup(this.ID, pSDA, pSDB, i2c.SLOW)
        end,

        write = function (this, reg, ...)
            i2c.start(this.ID)
            if not i2c.address(this.ID, this.ADDR, i2c.TRANSMITTER) then
                return nil
            end
            i2c.write(this.ID, reg)
            len = i2c.write(this.ID, ...)
            i2c.stop(this.ID)
            return len
        end,

        setMode1 = function(this, data)
            return this:write(0x00, data)
        end,

        setMode2 = function(this, data)
            return this:write(0x01, data)
        end,

        getChan = function(this, chan)
            return 6 + chan * 4
        end,

        setOn = function(this, chan, data)
            this:write(this:getChan(chan), bit.band(data, 0xFF))
            this:write(this:getChan(chan) + 1, bit.rshift(data, 8))
        end,
        
        setOff = function(this, chan, data)
            this:write(this:getChan(chan) + 2, bit.band(data, 0xFF))
            this:write(this:getChan(chan) + 3, bit.rshift(data, 8))
        end,

        setOnOff = function(this, chan, dataStart, dataEdn)
            this:setOn(chan, dataStart)
            this:setOff(chan, dataEdn)
        end,

        -- SCALE
    }
    return m
end

return M
