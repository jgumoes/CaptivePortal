
def device_name_change vars
  if vars.new_name
    vars.new_name = false
    return "This device is called #{vars.device_name}<br>"
            "<b class='NameChange'>Device name changed to <em>#{vars.device_name}</em></b><br>"
  end
end

def connection_status vars
  if vars.network_name.empty?
    return "not connected to a network"
  else
    return "connected to #{vars.network_name}"
  end
end

def test_password vars
  if vars.wrong_pass
    return "<label for='pwd' class=wrong>Wrong password</label><br>"
  end
end

def scan_networks (vars)
  if vars.networks.empty?
    return "<label class='radios'>No Networks Found"
              "<input type='radio' name='radio' value=''>"
              "<span class='checkmark'></span>"
            "</label>"
  else
    vars.networks.map do |ssid|
      "<label class='radios'>#{ssid}"
          "<input type='radio' name='radio' value='#{ssid}'>"
          "<span class='checkmark'></span>"
      "</label>"
    end.join
  end
end