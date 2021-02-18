require 'sinatra'

get "/" do

    device_name_change = "This device is called {Device Name}<br>"\
    "<b class='NameChange'>Device name changed to <em>Butt's Mcgee</em></b><br>"

    connection_status = "{not }connected to {access_point / any network}"

    wifi_ssid = scan_networks

    wrong_password = password_check

    device_name = "Butt's McGee"

    erb :index
end

connected = true
def scan_networks
    if connected
        "<label class='radios'>{SSID1}"
            "<input type='radio' name='radio' value='{SSID1}'>"
            "<span class='checkmark'></span>"
        "</label>"
    else
        "<label class='radios'>No Networks Found"
            "<input type='radio' name='radio' value=''>"
            "<span class='checkmark'></span>"
        "</label>"
    end
end

wrong_pass = false
def password_check
    if wrong_pass
        return "<label for='pwd' class=wrong>Wrong password</label><br>"
    end
end