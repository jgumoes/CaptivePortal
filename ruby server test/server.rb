# require 'sinatra'
require 'sinatra/base'

class ServerStatus
  attr_accessor :device_name, :network_name, :wrong_pass, :name_change, :attempted_network

  def initialize
    @device_name = "Butt's McGee"
    @network_name = false
    @wrong_pass = false
    @name_change = false
    @attempted_network = "null"
  end

  def resetFlags
    @wrong_pass = false
    @name_change = false
  end
end
class Portal < Sinatra::Base
  
  enable  :method_override
  use Rack::Session::Cookie, :key => "rack.session",
  :path => '/',
  :secret => 'your_secret'
  before (:all) do
    puts ("new flag")
    sessions['flags'] = nil
  end
  # before (:all) do
  #   @flags = ServerVariables.create()
  #   session[:flags] = @flags
  # end

  get "/" do
    @flags = session['flags'] # this will actually be stored as a global variable
    if @flags == nil
      puts ("new flag")
      # this won't be necessary because flags will be stored as a global variable
      @flags = ServerStatus.new()
      session['flags'] = @flags
    end
    
    
    # these statements mock the String processor in ESPAsyncWebServer
    if @flags.name_change
      @device_name_change = "<b class='NameChange'>Device name changed to <em>#{@flags.device_name}</em></b><br>"
    else
      @device_name_change = ""
    end

    if @flags.network_name == false
      @connection_status = "not connected to a network"
    else
      @connection_status = "connected to #{@flags.network_name}"
    end

    if @flags.wrong_pass
      @wrong_password = "<label for='pwd' class=wrong>Wrong password</label><br>"
    else
      @wrong_password = ""
    end

    @device_name = @flags.device_name

    @wifi_ssid_list = build_network_list(scan_networks(), @flags.attempted_network )

    # reset flags
    @flags.resetFlags()

    erb :"config.html"
  end

  post "/wifisave" do
    # @flags = ServerVariables.instance
    # p @flags
    # p session
    @flags = session['flags']
    puts session.keys
    if check_network(params['ssid'], params['pwd'])
      @flags.network_name = params['ssid']
    else
      @flags.attempted_network = params['ssid']
      @flags.network_name = false
      @flags.wrong_pass = true
    end

    redirect "/"
  end

  post "/change_name" do
    @flags = session['flags']
    @flags.device_name = params['dev_name']
    @flags.name_change = true
    redirect '/'
  end

  get "/scan_networks" do
    @flags = session['flags']
    puts @flags.attempted_network
    content_type 'application/json'
    JSON.generate({
      "network_list" => scan_networks(),
      "attempted_network" => @flags.attempted_network,
      })
  end

  # start the server if ruby file executed directly
  run! if app_file == $0
end

def scan_networks(empty=false)
  return [] if empty
  ["FBI Van number 2", "Stupid Starbucks", "Uncle Touchy\'s Mystery Van"]
end

def check_network(name, password)
  if name == "Uncle Touchy\'s Mystery Van" && password == "password"
    return true
  else
    return false
  end
end

def build_network_list(ssid_list, attempted_network = "")
  if ssid_list.length == 0
    return network_html(["No Networks Found"], "disabled")
  end
  html_out = ""
  ssid_list.each do |ssid|
    html_out += network_html(ssid, ssid == attempted_network)
  end
  return html_out
end

def network_html(ssid, checked=false)
  checked_hash = {
    true => "checked ",
    false => "",
    "disabled" => "disabled "}
  
  return "<label class='radios'>#{ssid}" +
    "<input id=\"#{ssid}\" type='radio' name='ssid' value=\"#{ssid}\" " +
    checked_hash[checked] +
    "required>" +
    "<span class='checkmark'></span>" +
  "</label>"
end