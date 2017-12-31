settings = {
  levels = {
    {
      time = 0,
      c1 = 100,
      c2 = 100,
      c3 = 100,
      c4 = 100
    }
  }
}

function loadSettings()
    if file.open("settings.json") then
        local dec = sjson.decoder()
        
        for str in file.read() do
            dec:write(str)
        end

        settings = dec:result()
       
        file.close()
    end
end

loadSettings()
