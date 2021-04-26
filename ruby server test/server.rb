# require 'sinatra'
require 'sinatra/base'

class ServerStatus
  attr_accessor :device_name, :network_name, :wrong_pass, :name_change, :attempted_network

  def initialize
    @device_name = "Butt's McGee"
    @network_name = false
    @wrong_pass = false
    @name_change = false
    @attempted_network = ""
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

  get "/" do
    @flags = session['flags'] # this will actually be stored as a global variable
    if @flags == nil
      puts ("new flag")
      # this won't be necessary because flags will be stored as a global variable
      @flags = ServerStatus.new()
      session['flags'] = @flags
    end

    File.read("views/config.html")
  end

  post "/wifisave" do
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
    puts "changing device name"
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

  get "/server_flags" do
    @flags = session['flags']
    puts @flags.device_name
    flagJSON = JSON.generate({
      "deviceName" => @flags.device_name,
      "networkName" => @flags.network_name,
      "wrongPass" => @flags.wrong_pass,
      "nameChange" => @flags.name_change,
    })

    # reset flags
    @flags.resetFlags()
    return flagJSON
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
