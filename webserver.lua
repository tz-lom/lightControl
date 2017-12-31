-- sntp.sync(nil, nil, nil, 1)
require("HttpServer")

app = express.new()
app:listen(80)


app:use(function(req, res, next)
    collectgarbage()
    next()
end)

if dns ~= nil then
    app:use(function(req,res,next) 
        local host = req.raw:match("Host:%s([^%s]+)")
        print(host)
        if host~="192.168.100.1" then
            res:status(303)
            res:set("Location","http://192.168.100.1")
            res:send("")          
        end
        next()
    end)
end

app:get('/settings',function(req,res)
    res:send(settings)
end)

app:post('/settings', function(req, res)
    if file.open("settings.json", "w") then
        file.write(req.raw:match("\r\n\r\n(.*)"))
        file.close()
        loadSettings()
    end
end)

app:use('/',express.static('index.html'))
--app:use('/',express.static('mui.min.css'))
collectgarbage()
--dofile("lightcontrol.lua")
