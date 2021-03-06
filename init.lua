dofile("compile.lua")
dofile("settings-init.lua")
dofile("lightcontrol.lc")
captive = false;

function startAP()
    local cfg={}
    cfg.ssid="Light controller configuration"
    --cfg.pwd="12345678"
    cfg.auth=AUTH_OPEN
    cfg.channel = 6
    cfg.hidden = 0
    cfg.max=4
    cfg.beacon=100

    local ip={}
    ip.ip="192.168.100.1"
    ip.netmask="255.255.255.0"
    ip.gateway="192.168.100.1"

    local dhcp = {}
    dhcp.start = "192.168.100.2"
    
    wifi.setmode(wifi.SOFTAP)
    wifi.setphymode(wifi.PHYMODE_N)
    wifi.ap.config(cfg)
    wifi.ap.setip(ip)
    wifi.ap.dhcp.config(dhcp)
    wifi.ap.dhcp.start()

    collectgarbage()
    --dofile("captiveDns.lua")
    collectgarbage()
    --dofile("webserver.lua")
    dofile("httpserver.lc")(80)
end

---[[
if settings.ssid~="" then
    wifi.setmode(wifi.STATION)
    wifi.sta.config({ssid=settings.ssid, pwd=settings.password})
    wifi.sta.autoconnect(1)
    wifi.sta.sethostname("lightController")

    wifi.eventmon.register(wifi.eventmon.STA_CONNECTED, function(t)
        dofile("httpserver.lc")(80)
    end)

    tmr.alarm(1,60000, tmr.ALARM_SINGLE, function() 
        wifi.eventmon.unregister(wifi.eventmon.STA_CONNECTED)
        if wifi.sta.getip()==nil then 
            startAP()
        end
    end)
else
    print("Starting setup access point")
    startAP()
end
--]]
