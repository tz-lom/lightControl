function unhex(str)
    str = string.gsub (str, "(%x%x) ?",
        function(h) return string.char(tonumber(h,16)) end)
    return str
end

dns=net.createUDPSocket()
dns:on("receive",function(s,c, port, resp_ip) 
    collectgarbage()
    local transaction_id=string.sub(c,1,2)
    local flags=string.sub(c,3,4)
    local questions=string.sub(c,5,6)

    local query = ""
    local raw_query = ""
    local j=13
    while true do
        local byte = string.sub(c,j,j)
        j=j+1
        raw_query = raw_query .. byte
        if byte:byte(1)==0x00 then --NULL marks end of the string.
            break
        end
        for i=1,byte:byte(1) do
            byte = string.sub(c,j,j)
            j=j+1
            raw_query = raw_query .. byte
            query = query .. byte
        end
        query = query .. '.'
    end
    query=query:sub(1,query:len()-1) --strip the trailing dot.
    local q_type = string.sub(c,j,j+1)
    j=j+2
    if q_type == unhex("00 01") then 
        --print("Got a type A query "..query)
        local class = string.sub(c,j,j+1)
        --ip = "192.168.4.1"
        local ip=unhex("C0 A8 64 01")
        local answers = unhex("00 01")
        local flags = unhex("81 80")
        
        resp = transaction_id..flags..questions..answers..unhex("00 00")..unhex("00 00")..raw_query..q_type..class..unhex("c0 0c")..q_type..class..unhex("00 00 00 da")..unhex("00 04")..ip
        s:send(port, resp_ip, resp)
    end
end) 
--[[
s:on("sent",function(s) 
    s:close() 
end)
--]]
dns:listen(53)
