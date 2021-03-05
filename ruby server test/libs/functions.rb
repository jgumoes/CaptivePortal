# This script contains the functions that changed
# the HTML in the page. They mimic how they will 
# work in the portal, so should work more or less
# the same as they will in C (the obvious exception)
# being the ServerVariables class


def device_name_change vars
  if vars.new_name
    vars.new_name = false
    return "<b class='NameChange'>Device name changed to <em>#{vars.device_name}</em></b><br>"
  end
end

def connection_status vars
  if vars.network_name.empty?
    return "not connected to a network"
  else
    return "connected to #{vars.network_name}"
  end
end

def wrong_password? vars
  if vars.wrong_pass
    return "<label for='pwd' class=wrong>Wrong password</label><br>"
  end
end

def scan_networks (vars)
  last_ssid = @vars.last_ssid
  networks = vars.networks
  if networks.empty?
    networks = ["No Networks Found"]
    # return "<label class='radios'>No Networks Found"\
    #           "<input type='radio' name='radio' value='' required disabled>"\
    #           "<span class='checkmark'></span>"\
    #         "</label>"
  end
  page = ""
  networks.each do |ssid|
    page += "<label class='radios'>#{ssid}" +
        "<input type='radio' name='ssid' value=\"#{ssid}\""
    if ssid == last_ssid then page += 'checked="true"' end
    if networks.empty? then page += 'disabled' end
    page += "required>" +
        "<span class='checkmark'></span>" +
    "</label>"
  end
  return page
  # end
end

