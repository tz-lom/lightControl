-- httpserver-compile.lua
-- Part of nodemcu-httpserver, compiles server code after upload.
-- Author: Marcos Kirsch

local compileAndRemoveIfNeeded = function(f)
   if file.exists(f) then
      print('Compiling:', f)
      node.compile(f)
      file.remove(f)
      collectgarbage()
   end
end

local moveToHttp = function(f)
    if file.exists(f) then
        print('Moving:', f)
        file.remove('http/'..f)
        file.rename(f, 'http/'..f)
        collectgarbage()
    end
end

local serverFiles = {
   'httpserver.lua',
   'httpserver-b64decode.lua',
   'httpserver-basicauth.lua',
   'httpserver-connection.lua',
   'httpserver-error.lua',
   'httpserver-header.lua',
   'httpserver-request.lua',
   'httpserver-static.lua',
   'httpserver-wifi.lua',
   'pca9685.lua'
}

local httpFiles = {
    'index.html',
    'd3.v4.min.js.gz',
    'settings.lua'
}

for i, f in ipairs(serverFiles) do compileAndRemoveIfNeeded(f) end

for i, f in ipairs(httpFiles) do moveToHttp(f) end

compileAndRemoveIfNeeded = nil
moveToHttp = nil
serverFiles = nil
httpFiles = nil
collectgarbage()

