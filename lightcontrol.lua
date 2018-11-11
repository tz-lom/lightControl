require('pca9685')

pca = pca9685.create(0, 0x40)
pca:init(1, 2)
pca:setMode1(0x01)
pca:setMode2(0x04)

lightTimer = tmr.create()
lightTimer:register(4000, tmr.ALARM_AUTO, function()
    collectgarbage()
    local now = getTime()


    for i,cur in pairs(settings.levels) do
        if cur.time>now then
            local prev
            if i==1 then
                prev = settings.levels[#settings.levels]    
            else
                prev = settings.levels[i-1]
            end

    print("set")
    
            for i=1,settings.maxChannel do
                if prev.time==cur.time then
                    pca:setOnOff(i, 0, cur.c[i])
                else
                    pca:setOnOff(i, 0, map(now, prev.time, cur.time, prev.c[i], cur.c[i]))
                end
            end
            break
        end
    end
    collectgarbage()
end)
lightTimer:start()


function getTime()
    local ts = rtctime.epoch2cal(rtctime.get())
    return ts["hour"]*ts["min"]
end

function map(now, tBeg, tEnd, from, to)
    return from + (to-from)*(now-tBeg)/(tEnd-tBeg)
end
