settings = {
  maxChannel = 5,
  levels = {
    {
      time = 23*60+59,
      power = 50,
      c= {2048,2048,2048,2048,2048}
    }
  },
  ssid="e",
  password=""
}

function loadSettings()
    if file.open("settings.json") then
        local dec = sjson.decoder()
        
        for str in file.read do
            dec:write(str)
        end

        local new = dec:result()
        for k,v in pairs(new) do
            settings[k] = v
        end
       
        file.close()
    end
end

loadSettings()
