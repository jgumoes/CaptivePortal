# This script contains the functions that changed
# the HTML in the page. They mimic how they will 
# work in the portal, so should work more or less
# the same as they will in C (the obvious exception)
# being the ServerVariables class

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

