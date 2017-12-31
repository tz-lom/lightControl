require('pca9685')

pca = pca9685.create(0, 0x40)
pca:init(1, 2)
pca:setMode1(0x01)
pca:setMode2(0x04)

cron.schedule("* * * * *", function(e)
    collectgarbage()
    local ts = rtctime.epoch2cal(rtctime.get())

    local now = ts["hour"]*ts["min"]

    for i,cur in settings.levels do
        if cur.time>now then
            local prev
            if i==1 then
                prev = settings.levels[#settings.levels]    
            else
                prev = settings.levels[i-1]
            end

            pca:setOnOff(0, 0, map(now, prev.time, cur.time, prev.c1, cur.c1))
            pca:setOnOff(1, 0, map(now, prev.time, cur.time, prev.c2, cur.c2))
            pca:setOnOff(2, 0, map(now, prev.time, cur.time, prev.c3, cur.c3))
            pca:setOnOff(3, 0, map(now, prev.time, cur.time, prev.c4, cur.c4))
            
        end
    end
  
end)

function map(now, tBeg, tEnd, from, to)
    return from + (to-from)*(now-tBeg)/(tEnd-tBeg)
end
