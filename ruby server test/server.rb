# require 'sinatra'
require 'sinatra/base'
require_relative 'libs/ServerVariables'
require_relative 'libs/functions'

class Portal < Sinatra::Base

  enable  :method_override
  use Rack::Session::Cookie, :key => "rack.session",
                            :path => '/',
                            :secret => 'your_secret'
    
  # before (:all) do
  #   @vars = ServerVariables.create()
  #   session[:vars] = @vars
  # end

  get "/" do
    @vars = session['vars']
    # puts session.keys
    if @vars == nil
      @vars = ServerVariables.new()
      session['vars'] = @vars
    end
    # p @vars
    # @vars = ServerVariables.instance()
    @vars.set_networks(empty=false)
    
    @device_name_change = device_name_change(@vars)

    @connection_status = connection_status(@vars)
    @wrong_password = wrong_password?(@vars)
    @wifi_ssid = scan_networks(@vars)

    # reset flags
    @vars.reset

    erb :index
  end

  post "/wifisave" do
    # @vars = ServerVariables.instance
    # p @vars
    # p session
    @vars = session['vars']
    puts session.keys
    @vars.check_network(params['ssid'], params['pwd'])
    redirect "/"
  end

  post "/change_name" do
    @vars = session['vars']
    @vars.device_name = params['dev_name']
    @vars.new_name = true
    redirect '/'
  end

  # start the server if ruby file executed directly
  run! if app_file == $0
end
