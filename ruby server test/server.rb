# require 'sinatra'
require 'sinatra/base'
require_relative 'libs/functions'

class ServerStatus
  attr_accessor :device_name, :network_name, :wrong_pass, :name_change

  def initialize
    @device_name = "Butt's McGee"
    @network_name = false
    @wrong_pass = false
    @name_change = false
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
    
  # before (:all) do
  #   @flags = ServerVariables.create()
  #   session[:flags] = @flags
  # end

  get "/" do
    @flags = session['flags'] # this will actually be stored as a global variable
    if @flags == nil
      # this won't be necessary because flags will be stored as a global variable
      @flags = ServerStatus.new()
      session['flags'] = @flags
    end
    
    
    # these statements mock the String processor in ESPAsyncWebServer
    if @flags.name_change
      @device_name_change = "<b class='NameChange'>Device name changed to <em>#{vars.device_name}</em></b><br>"
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


    # @wifi_ssid = scan_networks(@flags)

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
    @flags.check_network(params['ssid'], params['pwd'])
    redirect "/"
  end

  post "/change_name" do
    @flags = session['flags']
    @flags.device_name = params['dev_name']
    @flags.new_name = true
    redirect '/'
  end

  # start the server if ruby file executed directly
  run! if app_file == $0
end
