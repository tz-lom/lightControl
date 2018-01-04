return function(connection, req, args)
   if req.method == "GET" then
      dofile("httpserver-header.lc")(connection, 200, 'json')
      connection:send(sjson.encode(settings))
   elseif req.method == "POST" then
      dofile("httpserver-header.lc")(connection, 200, 'html')
      local data = req.getRequestData()
      
      if file.open("settings.json", "w") then
        print("saving:", data)
        file.write(data)
        file.close()
        loadSettings()
    end
   else
      connection:send("ERROR WTF req.method is ", req.method)
   end
end
